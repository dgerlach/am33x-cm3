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

#ifndef __HWMOD_H__
#define __HWMOD_H__

enum hwmod_id {
	HWMOD_CLKDIV32K,
	HWMOD_EMIF,
	HWMOD_EMIF_FW,
	HWMOD_GPIO0,
	HWMOD_I2C0,
	HWMOD_IEEE5000,
	HWMOD_L3,
	HWMOD_L3_INSTR,
	HWMOD_L4FW,
	HWMOD_L4HS,
	HWMOD_L4LS,
	HWMOD_MPU,
	HWMOD_OCMCRAM,
	HWMOD_OTFA_EMIF,
	HWMOD_OCPWP,

	HWMOD_COUNT,
	HWMOD_END = -1,
};

void hwmod_init(void);
void hwmod_enable(enum hwmod_id id);
void hwmod_disable(enum hwmod_id id);
int interconnect_hwmods_enable(void);
int interconnect_hwmods_disable(void);
int essential_hwmods_disable(void);
int essential_hwmods_enable(void);

#endif

