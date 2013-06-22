/* linux/drivers/media/video/ov3640.h
 *
 *   www.xcembed.com
 *
 * Driver for OV3640 (SXGA camera) from Samsung Electronics
 * 1/6" 3.2Mp CMOS Image Sensor SoC with an Embedded Image Processor
 * supporting MIPI CSI-2
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef __OV3640_H__
#define __OV3640_H__

struct ov3640_reg {
	unsigned short addr;
	unsigned short val;
};

struct ov3640_regset_type {
	unsigned char *regset;
	int len;
};

/*
 * Macro
 */
#define REGSET_LENGTH(x)	(sizeof(x)/sizeof(ov3640_reg))

/*
 * User defined commands
 */
/* S/W defined features for tune */
#define REG_DELAY	0xFFFF	/* in ms */
#define REG_CMD		0xFFFF	/* Followed by command */

/* Following order should not be changed */
enum image_size_ov3640 {
	/* This SoC supports upto SXGA (1280*1024) */
	QQVGA, /* 160*120*/
	QCIF, /* 176*144 */
	QVGA, /* 320*240 */
	CIF, /* 352*288 */
	VGA, /* 640*480 */
	SVGA, /* 800*600 */
	HD720P, /* 1280*720 */
	SXGA, /* 1280*1024 */
};

/*
 * Following values describe controls of camera
 * in user aspect and must be match with index of ov3640_regset[]
 * These values indicates each controls and should be used
 * to control each control
 */
enum ov3640_control {
	ov3640_INIT,
	ov3640_EV,
	ov3640_AWB,
	ov3640_MWB,
	ov3640_EFFECT,
	ov3640_CONTRAST,
	ov3640_SATURATION,
	ov3640_SHARPNESS,
};

#define ov3640_REGSET(x)	{	\
	.regset = x,			\
	.len = sizeof(x)/sizeof(ov3640_reg),}

/*
 * User tuned register setting values
 */
struct ov3640_reg ov3640_init_reg[] = 
{
	{0x3012, 0x80},
	{0x304d, 0x45},
	{0x30a7, 0x5e},
	{0x3087, 0x16},
	{0x309c, 0x1a},
	{0x30a2, 0xe4},
	{0x30aa, 0x42},
	{0x30b0, 0xff},
	{0x30b1, 0xff},
	{0x30b2, 0x10},
	{0x300e, 0x32},  //39
	{0x300f, 0x21},
	{0x3010, 0x20},
	{0x3011, 0x04},
	{0x304c, 0x81},
	{0x30d7, 0x10},
	{0x30d9, 0x0d},
	{0x30db, 0x08},
	{0x3016, 0x82},
	{0x3018, 0x38},  //48
	{0x3019, 0x30},  //40
	{0x301a, 0x61},  //82
	{0x307d, 0x00},
	{0x3087, 0x02},
	{0x3082, 0x20},
	{0x3015, 0x12},
	{0x3014, 0x04},
	{0x3013, 0xf7},
	
	{0x303c, 0x08},
	{0x303d, 0x18},
	{0x303e, 0x06},
	{0x303f, 0x0c},
	{0x3030, 0x62},
	{0x3031, 0x26},
	{0x3032, 0xe6},
	{0x3033, 0x6e},
	{0x3034, 0xea},
	{0x3035, 0xae},
	{0x3036, 0xa6},
	{0x3037, 0x6a},
	
	{0x3104, 0x02},
	{0x3105, 0xfd},
	{0x3106, 0x00},
	{0x3107, 0xff},
	{0x3300, 0x12},  //13
	{0x3301, 0xde},
	{0x3302, 0xef},  //ef

	{0x3316, 0xff},  //f8
	{0x3317, 0x00},  //04
	{0x3312, 0x26},
	{0x3314, 0x42},
	{0x3313, 0x2b},
	{0x3315, 0x42},
	{0x3310, 0xd0},
	{0x3311, 0xbd},
	{0x330c, 0x18},
	{0x330d, 0x18},
	{0x330e, 0x56},
	{0x330f, 0x5c},
	{0x330b, 0x1c},
	{0x3306, 0x5c},
	{0x3307, 0x11},
	
	{0x336a, 0x52},
	{0x3370, 0x46},
	{0x3376, 0x38},
	{0x3300, 0x13},
	{0x30b8, 0x20},
	{0x30b9, 0x17},
	{0x30ba, 0x04},
	{0x30bb, 0x08},
	{0x3507, 0x06},
	{0x350a, 0x4f},
	
	{0x3100, 0x02},
	{0x3301, 0xde},
	{0x3304, 0x00},
	{0x3400, 0x00},
	{0x3404, 0x00},

	{0x335f, 0x68},
	{0x3360, 0x18},
	{0x3361, 0x0c},
	{0x3362, 0x12},
	{0x3363, 0x88},
	{0x3364, 0xe4},
	{0x3403, 0x42},
	
//	{0x3600, 0xc4},
	{0x3088, 0x02},
	{0x3089, 0x80},
	{0x308a, 0x01},
	{0x308b, 0xe0},
	{0x308d, 0x04},
	{0x3086, 0x03},
	{0x3086, 0x00},

	{0x3011, 0x00},
	{0x304c, 0x85},

};

#if 0
#define OV3640_INIT_REGS	\
	(sizeof(ov3640_init_reg) / sizeof(ov3640_init_reg[0]))

unsigned short ov3640_sleep_reg[][2] = {

};

#define OV3640_SLEEP_REGS	\
	(sizeof(ov3640_sleep_reg) / sizeof(ov3640_sleep_reg[0]))

unsigned short ov3640_wakeup_reg[][2] = {

};

#define OV3640_WAKEUP_REGS	\
	(sizeof(ov3640_wakeup_reg) / sizeof(ov3640_wakeup_reg[0]))
#endif
/*
 * EV bias
 */

static const struct ov3640_reg ov3640_ev_m6[] = {
};

static const struct ov3640_reg ov3640_ev_m5[] = {
};

static const struct ov3640_reg ov3640_ev_m4[] = {
};

static const struct ov3640_reg ov3640_ev_m3[] = {
};

static const struct ov3640_reg ov3640_ev_m2[] = {
};

static const struct ov3640_reg ov3640_ev_m1[] = {
};

static const struct ov3640_reg ov3640_ev_default[] = {
};

static const struct ov3640_reg ov3640_ev_p1[] = {
};

static const struct ov3640_reg ov3640_ev_p2[] = {
};

static const struct ov3640_reg ov3640_ev_p3[] = {
};

static const struct ov3640_reg ov3640_ev_p4[] = {
};

static const struct ov3640_reg ov3640_ev_p5[] = {
};

static const struct ov3640_reg ov3640_ev_p6[] = {
};

/* Order of this array should be following the querymenu data */
static const unsigned char *ov3640_regs_ev_bias[] = {
	(unsigned char *)ov3640_ev_m6, (unsigned char *)ov3640_ev_m5,
	(unsigned char *)ov3640_ev_m4, (unsigned char *)ov3640_ev_m3,
	(unsigned char *)ov3640_ev_m2, (unsigned char *)ov3640_ev_m1,
	(unsigned char *)ov3640_ev_default, (unsigned char *)ov3640_ev_p1,
	(unsigned char *)ov3640_ev_p2, (unsigned char *)ov3640_ev_p3,
	(unsigned char *)ov3640_ev_p4, (unsigned char *)ov3640_ev_p5,
	(unsigned char *)ov3640_ev_p6,
};

/*
 * Auto White Balance configure
 */
static const struct ov3640_reg ov3640_awb_off[] = {
};

static const struct ov3640_reg ov3640_awb_on[] = {
};

static const unsigned char *ov3640_regs_awb_enable[] = {
	(unsigned char *)ov3640_awb_off,
	(unsigned char *)ov3640_awb_on,
};

/*
 * Manual White Balance (presets)
 */
static const struct ov3640_reg ov3640_wb_tungsten[] = {

};

static const struct ov3640_reg ov3640_wb_fluorescent[] = {

};

static const struct ov3640_reg ov3640_wb_sunny[] = {

};

static const struct ov3640_reg ov3640_wb_cloudy[] = {

};

/* Order of this array should be following the querymenu data */
static const unsigned char *ov3640_regs_wb_preset[] = {
	(unsigned char *)ov3640_wb_sunny,
	(unsigned char *)ov3640_wb_cloudy,
	(unsigned char *)ov3640_wb_tungsten,
	(unsigned char *)ov3640_wb_fluorescent,
};

/*
 * Color Effect (COLORFX)
 */
static const struct ov3640_reg ov3640_color_normal[] = {
};

static const struct ov3640_reg ov3640_color_sepia[] = {
};

static const struct ov3640_reg ov3640_color_aqua[] = {
};

static const struct ov3640_reg ov3640_color_monochrome[] = {
};

static const struct ov3640_reg ov3640_color_negative[] = {
};

static const struct ov3640_reg ov3640_color_sketch[] = {
};

/* Order of this array should be following the querymenu data */
static const unsigned char *ov3640_regs_color_effect[] = {
	(unsigned char *)ov3640_color_normal,
	(unsigned char *)ov3640_color_monochrome,
	(unsigned char *)ov3640_color_sepia,
	(unsigned char *)ov3640_color_aqua,
	(unsigned char *)ov3640_color_sketch,
	(unsigned char *)ov3640_color_negative,
};

/*
 * Contrast bias
 */
static const struct ov3640_reg ov3640_contrast_m2[] = {
};

static const struct ov3640_reg ov3640_contrast_m1[] = {
};

static const struct ov3640_reg ov3640_contrast_default[] = {
};

static const struct ov3640_reg ov3640_contrast_p1[] = {
};

static const struct ov3640_reg ov3640_contrast_p2[] = {
};

static const unsigned char *ov3640_regs_contrast_bias[] = {
	(unsigned char *)ov3640_contrast_m2,
	(unsigned char *)ov3640_contrast_m1,
	(unsigned char *)ov3640_contrast_default,
	(unsigned char *)ov3640_contrast_p1,
	(unsigned char *)ov3640_contrast_p2,
};

/*
 * Saturation bias
 */
static const struct ov3640_reg ov3640_saturation_m2[] = {
};

static const struct ov3640_reg ov3640_saturation_m1[] = {
};

static const struct ov3640_reg ov3640_saturation_default[] = {
};

static const struct ov3640_reg ov3640_saturation_p1[] = {
};

static const struct ov3640_reg ov3640_saturation_p2[] = {
};

static const unsigned char *ov3640_regs_saturation_bias[] = {
	(unsigned char *)ov3640_saturation_m2,
	(unsigned char *)ov3640_saturation_m1,
	(unsigned char *)ov3640_saturation_default,
	(unsigned char *)ov3640_saturation_p1,
	(unsigned char *)ov3640_saturation_p2,
};

/*
 * Sharpness bias
 */
static const struct ov3640_reg ov3640_sharpness_m2[] = {
};

static const struct ov3640_reg ov3640_sharpness_m1[] = {
};

static const struct ov3640_reg ov3640_sharpness_default[] = {
};

static const struct ov3640_reg ov3640_sharpness_p1[] = {
};

static const struct ov3640_reg ov3640_sharpness_p2[] = {
};

static const unsigned char *ov3640_regs_sharpness_bias[] = {
	(unsigned char *)ov3640_sharpness_m2,
	(unsigned char *)ov3640_sharpness_m1,
	(unsigned char *)ov3640_sharpness_default,
	(unsigned char *)ov3640_sharpness_p1,
	(unsigned char *)ov3640_sharpness_p2,
};

#endif
