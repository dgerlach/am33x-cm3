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

#include <device_common.h>
#include <io.h>
#include <prcm_core.h>
#include <hwmod.h>
#include <ddr.h>
#include <msg.h>

/*
 * Values recommended by the HW team. These change the pulls
 * on certain IOs of DATA and CMD macros
 */
#define SUSP_IO_PULL_DATA	0x3FF00003
#define SUSP_IO_PULL_CMD1	0xFFE0018B
#define SUSP_IO_PULL_CMD2	0xFFA0098B
#define RESUME_IO_PULL_DATA	0x18B
#define RESUME_IO_PULL_CMD	0x18B

void ddr_io_suspend(void)
{
	unsigned int var;
	/* mddr mode selection required only for PG1.0 */
	if (soc_id == AM335X_SOC_ID && soc_rev == AM335X_REV_ES1_0) {
		var = __raw_readl(DDR_IO_CTRL_REG);
		var |= DDR_IO_MDDR_SEL;
		__raw_writel(var, DDR_IO_CTRL_REG);
	}

	/* HACK: Enable dynamic power down in CTRL_SDRAM_CONFIG_EXT reg */

	var = __raw_readl(0x44E11460);
	var |= 0x100;
	__raw_writel(var, 0x44E11460);

	/* Weak pull down for DQ, DM */
	__raw_writel(SUSP_IO_PULL_DATA, DDR_DATA0_IOCTRL);
	__raw_writel(SUSP_IO_PULL_DATA, DDR_DATA1_IOCTRL);
	__raw_writel(SUSP_IO_PULL_DATA, DDR_DATA2_IOCTRL);
	__raw_writel(SUSP_IO_PULL_DATA, DDR_DATA3_IOCTRL);

	/* Different sleep sequences for DDR2 and DDR3 */
		/* Weak pull down for macro CMD0/1 */
//		__raw_writel(SUSP_IO_PULL_CMD1, DDR_CMD0_IOCTRL);
//		__raw_writel(SUSP_IO_PULL_CMD1, DDR_CMD1_IOCTRL);

		/*
		 * Weak pull down for macro CMD2
		 * exception: keep DDR_RESET pullup
		 */
//		__raw_writel(SUSP_IO_PULL_CMD1, DDR_CMD2_IOCTRL);

}

void ddr_io_resume(void)
{
	unsigned int var;

	/* mddr mode selection required only for PG1.0 */
	if (soc_id == AM335X_SOC_ID && soc_rev == AM335X_REV_ES1_0) {
		var = __raw_readl(DDR_IO_CTRL_REG);
		var &= ~DDR_IO_MDDR_SEL;
		/* Take out IO of mDDR mode */
		__raw_writel(var, DDR_IO_CTRL_REG);
	}

	/* Different sleep sequences for DDR2 and DDR3 */
	/* Disable the pull for CMD2/1/0 */
//		__raw_writel(RESUME_IO_PULL_CMD, DDR_CMD2_IOCTRL);
//		__raw_writel(RESUME_IO_PULL_CMD, DDR_CMD1_IOCTRL);
//		__raw_writel(RESUME_IO_PULL_CMD, DDR_CMD0_IOCTRL);

	/* Disable the pull for DATA1/0 */

//	__raw_writel(RESUME_IO_PULL_DATA, DDR_DATA3_IOCTRL);
//	__raw_writel(RESUME_IO_PULL_DATA, DDR_DATA2_IOCTRL);
//	__raw_writel(RESUME_IO_PULL_DATA, DDR_DATA1_IOCTRL);
//	__raw_writel(RESUME_IO_PULL_DATA, DDR_DATA0_IOCTRL);
}

/* same offsets for SA and Aegis */
void vtp_enable(void)
{
	unsigned int var;

	/* clear the register */
	__raw_writel(0x0, VTP0_CTRL_REG);

	/* write the filter value */
	__raw_writel(0x6, VTP0_CTRL_REG);

	/* set the VTP enable bit */
	var = __raw_readl(VTP0_CTRL_REG);
	__raw_writel((var | VTP_CTRL_ENABLE), VTP0_CTRL_REG);

	/* toggle the CLRZ bit */
	var = __raw_readl(VTP0_CTRL_REG);

	__raw_writel((var & VTP_CTRL_START_EN), VTP0_CTRL_REG);
	__raw_writel((var | VTP_CTRL_START_EN), VTP0_CTRL_REG);

	/* poll for VTP ready */
	while (!(__raw_readl(VTP0_CTRL_REG) & VTP_CTRL_READY));
}

/* same offsets for SA and Aegis */
void vtp_disable(void)
{
	if (mem_type == MEM_TYPE_DDR2)
		__raw_writel(VTP_CTRL_VAL_DDR2, VTP0_CTRL_REG);
	else
		__raw_writel(VTP_CTRL_VAL_DDR3, VTP0_CTRL_REG);
}

void vtt_high(void)
{
	if (vtt_toggle == false)
		return;

	hwmod_enable(HWMOD_GPIO0);

	__raw_writel((1 << vtt_gpio_pin), GPIO0_BASE + GPIO_SETDATAOUT);

	hwmod_disable(HWMOD_GPIO0);
}

void vtt_low(void)
{
	if (vtt_toggle == false)
		return;

	hwmod_enable(HWMOD_GPIO0);

	__raw_writel((1 << vtt_gpio_pin), GPIO0_BASE + GPIO_CLEARDATAOUT);

	hwmod_disable(HWMOD_GPIO0);
}

/* RESET line is applicable only to DDR3 */
void set_ddr_reset(void)
{
	unsigned int var;

	/*
	 * TODO: Make this a one-time change in MPU code itself
	 */
	if (mem_type == MEM_TYPE_DDR3) {
		/* hold DDR_RESET high via control module */
		var = __raw_readl(DDR_IO_CTRL_REG);
		var |= DDR3_RST_DEF_VAL;
		__raw_writel(var, DDR_IO_CTRL_REG);
	}
}

void clear_ddr_reset(void)
{
	unsigned int var;

	/*
	 * TODO: Make this a one-time change in MPU code itself
	 */
	if (mem_type == MEM_TYPE_DDR3) {
		/* make DDR_RESET low via control module */
		var = __raw_readl(DDR_IO_CTRL_REG);
		var &= ~DDR3_RST_DEF_VAL;
		__raw_writel(var, DDR_IO_CTRL_REG);
	}
}

