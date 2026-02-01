/* SPDX-License-Identifier: GPL-2.0-or-later */

#ifndef SUPERIO_NUVOTON_NCT6791D_CHIP_H
#define SUPERIO_NUVOTON_NCT6791D_CHIP_H

#include <stdbool.h>
#include <stdint.h>

#define NCT6791D_MAX_FANS	3	/* SYSFAN, CPUFAN, AUXFAN0 */

/* Temperature source for fan control */
enum nct6791d_fan_temp_src {
	NCT6791D_FAN_TEMP_PECI0 = 0,
	NCT6791D_FAN_TEMP_PECI0_CAL,
	NCT6791D_FAN_TEMP_SYSTIN,
	NCT6791D_FAN_TEMP_CPUTIN,
	NCT6791D_FAN_TEMP_AUXTIN,
};

/* Fan channel configuration - Smart Fan IV curve */
struct nct6791d_fan_config {
	uint8_t bank;		/* HWM bank: 1=SYSFAN, 2=CPUFAN, 3=AUXFAN0 */
	uint8_t temp_src;	/* enum nct6791d_fan_temp_src */
	bool stop_duty_en;	/* Index 00h bit7: use stop value vs zero when cooling */
	uint8_t temp[4];	/* Temperature points (C) for curve */
	uint8_t duty[4];	/* PWM duty (0-255) at each temp point */
	uint8_t temp_tolerance;
	uint8_t step_up_time;	/* Units: 0.1s */
	uint8_t step_down_time;
	uint8_t duty_per_step_up;
	uint8_t duty_per_step_down;
	uint8_t crit_temp;
	uint8_t crit_duty_en;
	uint8_t crit_duty;
	uint8_t crit_temp_tolerance;
};

struct superio_nuvoton_nct6791d_config {
	/* PECI enabled for Intel CPU temperature (required for fan control) */
	bool peci_enable;

	/* PECI Bank 7: Index 02h timing (default 0x32 per vendor), Index F8h/FAh calibration */
	uint8_t peci_timing;		/* default 0x32: TN_Extend, 750kHz, datasheet 9.316 */
	uint8_t peci_agent0_cal_0;	/* Bank 4 Index F8h, default 0x1c per vendor */
	uint8_t peci_agent0_cal_1;	/* Bank 4 Index FAh, default 0x23 per vendor */

	/* Fan configurations; unused fans have bank=0 */
	struct nct6791d_fan_config fans[NCT6791D_MAX_FANS];
};

#define FAN1	fans[0]
#define FAN2	fans[1]
#define FAN3	fans[2]

#endif /* SUPERIO_NUVOTON_NCT6791D_CHIP_H */
