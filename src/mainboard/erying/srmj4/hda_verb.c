/* SPDX-License-Identifier: GPL-2.0-only */

#include <device/azalia_device.h>
#include <device/azalia_codec/realtek.h>

static const u32 realtek_alc897_verbs[] = {
	AZALIA_SUBVENDOR(0, 0x10ec0897),

	AZALIA_PIN_CFG(0, ALC887_SPDIF_OUT2,  AZALIA_PIN_CFG_NC(0)),
	AZALIA_PIN_CFG(0, ALC887_DMIC_LR,     AZALIA_PIN_CFG_NC(0)),
	AZALIA_PIN_CFG(0, ALC887_FRONT,       AZALIA_PIN_DESC(
		AZALIA_JACK,
		AZALIA_EXTERNAL_PRIMARY_CHASSIS | AZALIA_REAR,
		AZALIA_LINE_OUT,
		AZALIA_STEREO_MONO_1_8,
		AZALIA_GREEN,
		AZALIA_JACK_PRESENCE_DETECT,
		2, 0)),
	AZALIA_PIN_CFG(0, ALC887_SURROUND,    AZALIA_PIN_CFG_NC(0)),
	AZALIA_PIN_CFG(0, ALC887_CENTER_LFE,  AZALIA_PIN_CFG_NC(0)),
	AZALIA_PIN_CFG(0, ALC887_SIDE_SURROUND, AZALIA_PIN_CFG_NC(0)),
	AZALIA_PIN_CFG(0, ALC887_MIC1,        AZALIA_PIN_DESC(
		AZALIA_JACK,
		AZALIA_EXTERNAL_PRIMARY_CHASSIS | AZALIA_REAR,
		AZALIA_MIC_IN,
		AZALIA_STEREO_MONO_1_8,
		AZALIA_PINK,
		AZALIA_JACK_PRESENCE_DETECT,
		3, 0)),
	AZALIA_PIN_CFG(0, ALC887_MIC2,        AZALIA_PIN_DESC(
		AZALIA_JACK,
		AZALIA_EXTERNAL_PRIMARY_CHASSIS | AZALIA_FRONT,
		AZALIA_MIC_IN,
		AZALIA_STEREO_MONO_1_8,
		AZALIA_PINK,
		AZALIA_JACK_PRESENCE_DETECT,
		3, 15)),
	AZALIA_PIN_CFG(0, ALC887_LINE1,       AZALIA_PIN_DESC(
		AZALIA_JACK,
		AZALIA_EXTERNAL_PRIMARY_CHASSIS | AZALIA_REAR,
		AZALIA_LINE_IN,
		AZALIA_STEREO_MONO_1_8,
		AZALIA_BLUE,
		AZALIA_JACK_PRESENCE_DETECT,
		4, 0)),
	AZALIA_PIN_CFG(0, ALC887_LINE2,       AZALIA_PIN_DESC(
		AZALIA_JACK,
		AZALIA_EXTERNAL_PRIMARY_CHASSIS | AZALIA_FRONT,
		AZALIA_HP_OUT,
		AZALIA_STEREO_MONO_1_8,
		AZALIA_GREEN,
		AZALIA_JACK_PRESENCE_DETECT,
		1, 0)),
	AZALIA_PIN_CFG(0, ALC887_CD,          AZALIA_PIN_CFG_NC(0)),
	AZALIA_PIN_CFG(0, ALC887_PC_BEEP,     0x4024c601), /* N/C, internal beep; misc/color non-standard */
	AZALIA_PIN_CFG(0, ALC887_SPDIF_OUT1,  AZALIA_PIN_CFG_NC(0)),
	AZALIA_PIN_CFG(0, ALC887_SPDIF_IN,    AZALIA_PIN_CFG_NC(0)),
};

static const u32 intel_display_audio_verbs[] = {
	AZALIA_SUBVENDOR(2, 0x80860101),
	AZALIA_PIN_CFG(2, 0x04, 0x18560010),
};

const u32 pc_beep_verbs[] = {};

struct azalia_codec mainboard_azalia_codecs[] = {
	{
		.name         = "Realtek ALC897",
		.vendor_id    = 0x10ec0897,
		.subsystem_id = 0x10ec0897,
		.address      = 0,
		.verbs        = realtek_alc897_verbs,
		.verb_count   = ARRAY_SIZE(realtek_alc897_verbs),
	},
	{
		.name         = "Intel Display Audio (HDMI/DP)",
		.vendor_id    = 0x80862812,
		.subsystem_id = 0x80860101,
		.address      = 2,
		.verbs        = intel_display_audio_verbs,
		.verb_count   = ARRAY_SIZE(intel_display_audio_verbs),
	},
	{ /* terminator */ }
};

AZALIA_ARRAY_SIZES;
