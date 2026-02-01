/* SPDX-License-Identifier: GPL-2.0-or-later */

/*
 * NCT6791D Hardware Monitor initialization.
 * Programs PECI (Intel CPU temperature) and Smart Fan IV curves for
 * SYSFAN, CPUFAN, and AUXFAN0.
 */

#include <console/console.h>
#include <device/device.h>
#include <superio/conf_mode.h>
#include <superio/hwm5_conf.h>

#include "nct6791d_hwm.h"
#include "chip.h"
#include "nct6791d.h"
#include "../common/hwm.h"

/* Map config temp_src enum to hardware register value.
 * Bit 7 (Stopduty_En) from stop_duty_en: use stop value vs zero when cooling. */
static uint8_t nct6791d_fan_src_to_reg(enum nct6791d_fan_temp_src src, bool stop_duty_en)
{
	uint8_t reg;
	switch (src) {
	case NCT6791D_FAN_TEMP_PECI0:
		reg = NCT6791D_FAN_SRC_PECI0;
		break;
	case NCT6791D_FAN_TEMP_PECI0_CAL:
		reg = NCT6791D_FAN_SRC_PECI0_CAL;
		break;
	case NCT6791D_FAN_TEMP_SYSTIN:
		reg = NCT6791D_FAN_SRC_SYSTIN;
		break;
	case NCT6791D_FAN_TEMP_CPUTIN:
		reg = NCT6791D_FAN_SRC_CPUTIN;
		break;
	case NCT6791D_FAN_TEMP_AUXTIN:
		reg = NCT6791D_FAN_SRC_AUXTIN;
		break;
	default:
		reg = NCT6791D_FAN_SRC_PECI0;
		break;
	}
	return reg | (stop_duty_en ? NCT6791D_FAN_SRC_STOPDUTY_EN : 0);
}

void nct6791d_hwm_init(uint16_t hwm_base,
		       const struct superio_nuvoton_nct6791d_config *conf)
{
	if (!conf || !conf->peci_enable)
		return;

	printk(BIOS_DEBUG, "NCT6791D: HWM init at 0x%x\n", hwm_base);

	nuvoton_hwm_select_bank(hwm_base, NCT6791D_HWM_BANK_PECI_SETUP);
	pnp_write_hwm5_index(hwm_base, NCT6791D_PECI_CTRL_0,
			     NCT6791D_PECI_CTRL_0_PECI_EN |
			     NCT6791D_PECI_CTRL_0_IS_PECI30 |
			     NCT6791D_PECI_CTRL_0_ROUTINE_EN);
	pnp_write_hwm5_index(hwm_base, NCT6791D_PECI_CTRL_1,
			     conf->peci_timing ? conf->peci_timing : NCT6791D_PECI_TIMING_DEFAULT);
	pnp_write_hwm5_index(hwm_base, NCT6791D_PECI_CTRL_2,
			     NCT6791D_PECI_CTRL_2_AGT_AGENT0);
	pnp_write_hwm5_index(hwm_base, NCT6791D_PECI_CTRL_3, NCT6791D_PECI_CTRL_3_DEFAULT);
	pnp_write_hwm5_index(hwm_base, NCT6791D_PECI_CTRL_9, NCT6791D_TBASE0_DEFAULT);

	nuvoton_hwm_select_bank(hwm_base, NCT6791D_HWM_BANK_GLOBAL);
	pnp_write_hwm5_index(hwm_base, NCT6791D_PECI_TEMP_ENABLE, 1);

	nuvoton_hwm_select_bank(hwm_base, NCT6791D_HWM_BANK_PECI_CAL);
	pnp_write_hwm5_index(hwm_base, NCT6791D_PECI_AGENT0_CAL_0,
			     conf->peci_agent0_cal_0 ? conf->peci_agent0_cal_0 :
			     NCT6791D_PECI_AGENT0_CAL_0_DEFAULT);
	pnp_write_hwm5_index(hwm_base, NCT6791D_PECI_AGENT0_CAL_1,
			     conf->peci_agent0_cal_1 ? conf->peci_agent0_cal_1 :
			     NCT6791D_PECI_AGENT0_CAL_1_DEFAULT);

	/* Program fan curves */
	for (int i = 0; i < NCT6791D_MAX_FANS; i++) {
		const struct nct6791d_fan_config *fan = &conf->fans[i];
		if (fan->bank == 0)
			continue;

		printk(BIOS_DEBUG, "NCT6791D: programming fan bank %u\n", fan->bank);

		nuvoton_hwm_select_bank(hwm_base, fan->bank);

		pnp_write_hwm5_index(hwm_base, NCT6791D_FAN_SOURCE,
				     nct6791d_fan_src_to_reg((enum nct6791d_fan_temp_src)fan->temp_src,
							      fan->stop_duty_en));

		for (int j = 0; j < 4; j++)
			pnp_write_hwm5_index(hwm_base, NCT6791D_FAN_TEMP(j),
					     fan->temp[j]);
		for (int j = 0; j < 4; j++)
			pnp_write_hwm5_index(hwm_base, NCT6791D_FAN_DUTY(j),
					     fan->duty[j]);

		pnp_write_hwm5_index(hwm_base, NCT6791D_FAN_CRIT_TEMP,
				     fan->crit_temp);
		pnp_write_hwm5_index(hwm_base, NCT6791D_FAN_CRIT_DUTY_EN,
				     fan->crit_duty_en);
		pnp_write_hwm5_index(hwm_base, NCT6791D_FAN_CRIT_DUTY,
				     fan->crit_duty);
		pnp_write_hwm5_index(hwm_base, NCT6791D_FAN_CRIT_TEMP_TOL,
				     fan->crit_temp_tolerance);

		pnp_write_hwm5_index(hwm_base, NCT6791D_FAN_STEP_UP_TIME,
				     fan->step_up_time);
		pnp_write_hwm5_index(hwm_base, NCT6791D_FAN_STEP_DOWN_TIME,
				     fan->step_down_time);
		pnp_write_hwm5_index(hwm_base, NCT6791D_FAN_DUTY_PER_STEP,
				     (fan->duty_per_step_up << 4) |
				     fan->duty_per_step_down);

		pnp_write_hwm5_index(hwm_base, NCT6791D_FAN_MODE_TOLERANCE,
				     (NCT6791D_FAN_MODE_SFIV << 4) |
				     fan->temp_tolerance);
	}
}
