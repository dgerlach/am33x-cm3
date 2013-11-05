/*
 * AM33XX-CM3 firmware
 *
 * Cortex-M3 (CM3) firmware for power management on Texas Instruments' AM33XX series of SoCs
 *
 * Copyright (C) 2011 Texas Instruments Incorporated - http://www.ti.com/
 *
 *  This software is licensed under the  standard terms and conditions in the Texas Instruments  Incorporated
 *  Technology and Software Publicly Available Software License Agreement , a copy of which is included in the
 *  software download.
*/

#include <cm335x.h>
#include <hwmod.h>
#include <hwmod_335x.h>

const unsigned int am335x_hwmods[HWMOD_COUNT] = {
	[HWMOD_CLKDIV32K]	= AM335X_CM_PER_CLKDIV32K_CLKCTRL,
	[HWMOD_EMIF]		= AM335X_CM_PER_EMIF_CLKCTRL,
	[HWMOD_EMIF_FW]		= AM335X_CM_PER_EMIF_FW_CLKCTRL,
	[HWMOD_GPIO0]		= AM335X_CM_WKUP_GPIO0_CLKCTRL,
	[HWMOD_I2C0]		= AM335X_CM_WKUP_I2C0_CLKCTRL,
	[HWMOD_IEEE5000]	= AM335X_CM_PER_IEEE5000_CLKCTRL,
	[HWMOD_L3]		= AM335X_CM_PER_L3_CLKCTRL,
	[HWMOD_L3_INSTR]	= AM335X_CM_PER_L3_INSTR_CLKCTRL,
	[HWMOD_L4FW]		= AM335X_CM_PER_L4FW_CLKCTRL,
	[HWMOD_L4HS]		= AM335X_CM_PER_L4HS_CLKCTRL,
	[HWMOD_L4LS]		= AM335X_CM_PER_L4LS_CLKCTRL,
	[HWMOD_MPU]		= AM335X_CM_MPU_MPU_CLKCTRL,
	[HWMOD_OCMCRAM]		= AM335X_CM_PER_OCMCRAM_CLKCTRL,
};

const enum hwmod_id am335x_essential_hwmods[] = {
	HWMOD_CLKDIV32K,
	HWMOD_IEEE5000,
	HWMOD_EMIF_FW,
	HWMOD_OCMCRAM,
	HWMOD_END,
};

const enum hwmod_id am335x_interconnect_hwmods[] = {
	HWMOD_L4LS,
	HWMOD_L4HS,
	HWMOD_L4FW,
	HWMOD_L3_INSTR,
	HWMOD_L3,
	HWMOD_END,
};

