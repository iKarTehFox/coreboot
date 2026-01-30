/* SPDX-License-Identifier: GPL-2.0-only */

#include <soc/meminit.h>
#include <soc/romstage.h>

static const struct mb_cfg mem_config = {
	.type = MEM_TYPE_DDR5,
	.ect = true,
	.UserBd = BOARD_TYPE_DESKTOP_1DPC,
	.LpDdrDqDqsReTraining = 1,
	.ddr_config = { .dq_pins_interleaved = true },
};

static const struct mem_spd spd_info = {
	.topo = MEM_TOPO_DIMM_MODULE,
	.smbus = {
		[0] = { .addr_dimm[0] = 0x50, },
		[1] = { .addr_dimm[0] = 0x52, },
	},
};

void mainboard_memory_init_params(FSPM_UPD *mupd)
{
	mupd->FspmConfig.EnableAbove4GBMmio = 1;
	mupd->FspmConfig.GttSize = 3;
	mupd->FspmConfig.ApertureSize = 3;
	mupd->FspmConfig.IgdDvmt50PreAlloc = 2;

	memcfg_init(mupd, &mem_config, &spd_info, false);
}
