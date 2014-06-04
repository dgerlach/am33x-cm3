/*
 * AM33XX-CM3 firmware
 *
 * Cortex-M3 (CM3) firmware for power management on Texas Instruments' AM33XX series of SoCs
 *
 * Copyright (C) 2014 Texas Instruments Incorporated - http://www.ti.com/
 *
 *  This software is licensed under the  standard terms and conditions in the Texas Instruments  Incorporated
 *  Technology and Software Publicly Available Software License Agreement , a copy of which is included in the
 *  software download.
 */

#include <stdint.h>
#include <stddef.h>

#define CM3_RSC_TBL_VERSION	0x1
#define CM3_RSC_NUM		0x3

/*
 * These values and structs must match those in include/linux/remoteproc.h
 * within Linux, otherwise remoteproc will not be able to parse the
 * resource table properly. Refer to kernel for remoteproc documentation.
 */

#define RSC_CARVEOUT	0
#define RSC_DEVMEM	1
#define RSC_TRACE	2
#define RSC_VDEV	3
#define RSC_INTMEM	4

struct resource_table {
	uint32_t ver;
	uint32_t num;
	uint32_t reserved[2];
	uint32_t offset[0];
} __attribute__ ((__packed__));

struct fw_rsc_hdr {
	uint32_t type;
	uint8_t data[0];
} __attribute__ ((__packed__));

struct fw_rsc_trace {
	uint32_t da;
	uint32_t len;
	uint32_t reserved;
	uint8_t name[32];
} __attribute__ ((__packed__));

struct fw_rsc_intmem {
	uint32_t version;
	uint32_t da;
	uint32_t pa;
	uint32_t len;
	uint32_t reserved[2];
	uint8_t name[32];
} __attribute__ ((__packed__));

struct cm3_resource_table {
	struct resource_table base;

	uint32_t offset[CM3_RSC_NUM]; /* Should match 'num' in actual definition */
	struct fw_rsc_hdr cm3_umem_hdr;
	struct fw_rsc_intmem cm3_umem;
	struct fw_rsc_hdr cm3_dmem_hdr;
	struct fw_rsc_intmem cm3_dmem;
	struct fw_rsc_hdr cm3_trace_hdr;
	struct fw_rsc_trace cm3_trace;
} __attribute__ ((__packed__));

struct cm3_resource_table rsc_tab __attribute__ ((section (".resource_table"))) = {
	{
		CM3_RSC_TBL_VERSION,	/* Version */
		CM3_RSC_NUM,		/* Number of resources */
		{0,0,},			/* Reserved */
	},

	/* List of offsets to each resource entry header */
	{
		__builtin_offsetof(struct cm3_resource_table, cm3_umem_hdr),
		__builtin_offsetof(struct cm3_resource_table, cm3_dmem_hdr),
		__builtin_offsetof(struct cm3_resource_table, cm3_trace_hdr),
	},

	{RSC_INTMEM,},
	{
		0x1,		/* intmem version */
		0x0,		/* Device address */
		0x44d00000,	/* Physical address */
		0x4000,		/* Length */
		{0, 0,},	/* Reserved */
		"umem",		/* Name */
	},

	{RSC_INTMEM,},
	{
		0x1,		/* intmem version */
		0x80000,	/* Device address */
		0x44d80000,	/* Physical address */
		0x2000,		/* Length */
		{0, 0,},	/* Reserved */
		"dmem",		/* Name */
	},

	{RSC_TRACE,},
	{
		0x81000,	/* Device address */
		0x1000,		/* Length */
		0,		/* Reserved */
		"log_buf",	/* Name */
	},
};
