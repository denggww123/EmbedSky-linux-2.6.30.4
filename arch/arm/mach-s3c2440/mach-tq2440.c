/* linux/arch/arm/mach-s3c2440/mach-tq2440.c
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
*/

#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/interrupt.h>
#include <linux/list.h>
#include <linux/timer.h>
#include <linux/init.h>
#include <linux/serial_core.h>
#include <linux/platform_device.h>
#include <linux/io.h>

#include <asm/mach/arch.h>
#include <asm/mach/map.h>
#include <asm/mach/irq.h>

#include <mach/hardware.h>
#include <asm/irq.h>
#include <asm/mach-types.h>

#include <plat/regs-serial.h>
#include <mach/regs-gpio.h>
#include <mach/regs-lcd.h>

#include <mach/idle.h>
#include <mach/fb.h>
#include <plat/iic.h>

#include <plat/s3c2410.h>
#include <plat/s3c2440.h>
#include <plat/clock.h>
#include <plat/devs.h>
#include <plat/cpu.h>

#include <plat/common-EmbedSky.h>

#include <linux/dm9000.h>

#include <plat/udc.h>

#include <sound/s3c24xx_uda134x.h>

static struct map_desc tq2440_iodesc[] __initdata = {
};

#define UCON S3C2410_UCON_DEFAULT | S3C2410_UCON_UCLK
#define ULCON S3C2410_LCON_CS8 | S3C2410_LCON_PNONE | S3C2410_LCON_STOPB
#define UFCON S3C2410_UFCON_RXTRIG8 | S3C2410_UFCON_FIFOMODE

static struct s3c2410_uartcfg tq2440_uartcfgs[] __initdata = {
	[0] = {
		.hwport	     = 0,
		.flags	     = 0,
		.ucon	     = 0x3c5,
		.ulcon	     = 0x03,
		.ufcon	     = 0x51,
	},
	[1] = {
		.hwport	     = 1,
		.flags	     = 0,
		.ucon	     = 0x3c5,
		.ulcon	     = 0x03,
		.ufcon	     = 0x51,
	},
	[2] = {
		.hwport	     = 2,
		.flags	     = 0,
		.ucon	     = 0x3c5,
		.ulcon	     = 0x03,
		.ufcon	     = 0x51,
	}
};

/* LCD driver info */

static struct s3c2410fb_display tq2440_lcd_cfg __initdata = {

#if defined(CONFIG_FB_S3C24X0_TFT1024768)
	.lcdcon5	= S3C2410_LCDCON5_FRM565 |
			  S3C2410_LCDCON5_HWSWP,
#else
	.lcdcon5	= S3C2410_LCDCON5_FRM565 |
			  S3C2410_LCDCON5_INVVLINE |
			  S3C2410_LCDCON5_INVVFRAME |
			  S3C2410_LCDCON5_PWREN |
			  S3C2410_LCDCON5_HWSWP,
#endif
	.type		= S3C2410_LCDCON1_TFT,

#if defined(CONFIG_FB_S3C24X0_T240320)
	.width		= 240,
	.height		= 320,

	.pixclock	= 100000, /* HCLK 100 MHz, divisor 4 */
	.setclkval	= 0x4,
	.xres		= 240,
	.yres		= 320,
	.bpp		= 16,
	.left_margin	= 3,	/* for HFPD*/
	.right_margin	= 6,	/* for HBPD*/
	.hsync_len	= 1,	/* for HSPW*/
	.upper_margin	= 2,	/* for VFPD*/
	.lower_margin	= 1,	/* for VBPD*/
	.vsync_len	= 1,	/* for VSPW*/

#elif	defined(CONFIG_FB_S3C24X0_S320240)
	.width		= 320,
	.height		= 240,

	.pixclock	= 80000, /* HCLK 100 MHz, divisor 3 */
	.setclkval	= 0x3,
	.xres		= 320,
	.yres		= 240,
	.bpp		= 16,
	.left_margin	= 15,	/* for HFPD*/
	.right_margin	= 5,	/* for HBPD*/
	.hsync_len	= 8,	/* for HSPW*/
	.upper_margin	= 5,	/* for VFPD*/
	.lower_margin	= 3,	/* for VBPD*/
	.vsync_len	= 15,	/* for VSPW*/

#elif defined(CONFIG_FB_S3C24X0_W320240)
	.width		= 320,
	.height		= 240,

	.pixclock	= 80000, /* HCLK 100 MHz, divisor 3 */
	.setclkval	= 0x3,
	.xres		= 320,
	.yres		= 240,
	.bpp		= 16,
	.left_margin	= 28,	/* for HFPD*/
	.right_margin	= 24,	/* for HBPD*/
	.hsync_len	= 42,	/* for HSPW*/
	.upper_margin	= 6,	/* for VFPD*/
	.lower_margin	= 2,	/* for VBPD*/
	.vsync_len	= 12,	/* for VSPW*/

#elif defined(CONFIG_FB_S3C24X0_TFT480272)
	.width		= 480,
	.height		= 272,

	.pixclock	= 40000, /* HCLK 100 MHz, divisor 1 */
	.setclkval	= 0x4,
	.xres		= 480,
	.yres		= 272,
	.bpp		= 16,
  	.left_margin	= 19,	/* for HFPD*/
  	.right_margin	= 10,	/* for HBPD*/
  	.hsync_len	= 30,	/* for HSPW*/
  	.upper_margin	= 4,	/* for VFPD*/
  	.lower_margin	= 2,	/* for VBPD*/
  	.vsync_len	= 8,	/* for VSPW*/

#elif defined(CONFIG_FB_S3C24X0_TFT640480)
	.width		= 640,
	.height		= 480,

	.pixclock	= 40000, /* HCLK 100 MHz, divisor 1 */
	.setclkval	= 0x1,
	.xres		= 640,
	.yres		= 480,
	.bpp		= 16,
	.left_margin	= 40,	/* for HFPD*/
	.right_margin	= 67,	/* for HBPD*/
	.hsync_len	= 31,	/* for HSPW*/
	.upper_margin	= 5,	/* for VFPD*/
	.lower_margin	= 25,	/* for VBPD*/
	.vsync_len	= 1,	/* for VSPW*/

#elif defined(CONFIG_FB_S3C24X0_TFT800480)
	.width		= 800,
	.height		= 480,

	.pixclock	= 40000, /* HCLK 100 MHz, divisor 1 */
	.setclkval	= 0x1,
	.xres		= 800,
	.yres		= 480,
	.bpp		= 16,
	.left_margin	= 15,	/* for HFPD*/
	.right_margin	= 47,	/* for HBPD*/
	.hsync_len	= 95,	/* for HSPW*/
	.upper_margin	= 9,	/* for VFPD*/
	.lower_margin	= 5,	/* for VBPD*/
	.vsync_len	= 1,	/* for VSPW*/

#elif defined(CONFIG_FB_S3C24X0_TFT800600)
	.width		= 800,
	.height		= 600,

	.pixclock	= 40000, /* HCLK 100 MHz, divisor 1 */
	.setclkval	= 0x1,
	.xres		= 800,
	.yres		= 600,
	.bpp		= 16,
	.left_margin	= 15,	/* for HFPD*/
	.right_margin	= 47,	/* for HBPD*/
	.hsync_len	= 95,	/* for HSPW*/
	.upper_margin	= 9,	/* for VFPD*/
	.lower_margin	= 5,	/* for VBPD*/
	.vsync_len	= 1,	/* for VSPW*/

#elif defined(CONFIG_FB_S3C24X0_TFT1024768)
	.width		= 1024,
	.height		= 768,

	.pixclock	= 40000, /* HCLK 100 MHz, divisor 1 */
	.setclkval	= 0x3,
	.xres		= 1024,
	.yres		= 768,
	.bpp		= 16,
	.left_margin	= 199,	/* for HFPD*/
	.right_margin	= 15,	/* for HBPD*/
	.hsync_len	= 15,	/* for HSPW*/
	.upper_margin	= 1,	/* for VFPD*/
	.lower_margin	= 1,	/* for VBPD*/
	.vsync_len	= 1,	/* for VSPW*/

#endif
};

static struct s3c2410fb_mach_info tq2440_fb_info __initdata = {
	.displays	= &tq2440_lcd_cfg,
	.num_displays	= 1,
	.default_display = 0,

#if defined(CONFIG_FB_S3C24X0_TFT1024768)   
	/* currently setup by downloader */
	.gpccon		= 0xaa955699,
	.gpccon_mask	= 0xffc003cc,
	.gpcup		= 0x0000ffff,
	.gpcup_mask	= 0xffffffff,
	.gpdcon		= 0xaa95aaa1,
	.gpdcon_mask	= 0xffffffff,
	.gpdup		= 0x0000faff,
	.gpdup_mask	= 0xffffffff,

	.lpcsel		= 0xf82,
#endif
};

/* DM9000 */
#ifdef CONFIG_DM9000
static struct resource s3c_dm9k_resource[] = {
	[0] = {
		.start	= S3C2410_CS4,
		.end	= S3C2410_CS4 + 3,
		.flags	= IORESOURCE_MEM,
	},
	[1] = {
		.start	= S3C2410_CS4 + 4,
		.end	= S3C2410_CS4 + 4 + 3,
		.flags	= IORESOURCE_MEM,
	},
	[2] = {
		.start	= IRQ_EINT7,
		.end	= IRQ_EINT7,
		.flags	= IORESOURCE_IRQ | IRQF_TRIGGER_RISING,
	}

};

static struct dm9000_plat_data s3c_dm9k_platdata = {
	.flags	= DM9000_PLATF_16BITONLY,
};

struct platform_device s3c_device_dm9000 = {
	.name		= "dm9000",
	.id			= 0,
	.num_resources	= ARRAY_SIZE(s3c_dm9k_resource),
	.resource		= s3c_dm9k_resource,
	.dev			= {
		.platform_data = &s3c_dm9k_platdata,
	}
};
#endif

/* USB Device (Gadget)*/
static void EmbedSky_udc_pullup(enum s3c2410_udc_cmd_e cmd)
{
	printk(KERN_DEBUG "EmbedSky udc: pullup(%d)\n",cmd);
	switch (cmd)
	{
		case S3C2410_UDC_P_ENABLE :
			s3c2410_gpio_setpin(S3C2410_GPG12, 1);
			break;
		case S3C2410_UDC_P_DISABLE :
			s3c2410_gpio_setpin(S3C2410_GPG12, 0);
			break;
		case S3C2410_UDC_P_RESET :
			break;
		default:
			break;
	}
}

static struct s3c2410_udc_mach_info EmbedSky_udc_cfg = {
	.udc_command		= EmbedSky_udc_pullup,
};

/* UDA1341 */
static struct s3c24xx_uda134x_platform_data s3c24xx_uda134x_data = {
	.l3_clk = S3C2410_GPB4,
	.l3_data = S3C2410_GPB3,
	.l3_mode = S3C2410_GPB2,
	.model = UDA134X_UDA1341,
};

static struct platform_device s3c_device_uda134x = {
	.name = "s3c24xx_uda134x",
	.dev = {
		.platform_data	= &s3c24xx_uda134x_data,
	}
};

static struct platform_device *tq2440_devices[] __initdata = {
	&s3c_device_usb,
	&s3c_device_lcd,
	&s3c_device_wdt,
	&s3c_device_i2c0,
	&s3c_device_iis,
	&s3c_device_rtc,
#ifdef CONFIG_DM9000
	&s3c_device_dm9000,
#endif
	&s3c_device_usbgadget,
	&s3c_device_uda134x,
};

static void __init tq2440_map_io(void)
{
	s3c24xx_init_io(tq2440_iodesc, ARRAY_SIZE(tq2440_iodesc));
	s3c24xx_init_clocks(12000000);
	s3c24xx_init_uarts(tq2440_uartcfgs, ARRAY_SIZE(tq2440_uartcfgs));
}

static void __init tq2440_machine_init(void)
{
	s3c24xx_fb_set_platdata(&tq2440_fb_info);
	s3c_i2c0_set_platdata(NULL);

	platform_add_devices(tq2440_devices, ARRAY_SIZE(tq2440_devices));
	EmbedSky_machine_init();
	s3c2410_gpio_setpin(S3C2410_GPG12, 0);
	s3c2410_gpio_cfgpin(S3C2410_GPG12, S3C2410_GPIO_OUTPUT);
	s3c24xx_udc_set_platdata(&EmbedSky_udc_cfg);
}

MACHINE_START(S3C2440, "TQ2440")
	.phys_io	= S3C2410_PA_UART,
	.io_pg_offst	= (((u32)S3C24XX_VA_UART) >> 18) & 0xfffc,
	.boot_params	= S3C2410_SDRAM_PA + 0x100,

	.init_irq	= s3c24xx_init_irq,
	.map_io		= tq2440_map_io,
	.init_machine	= tq2440_machine_init,
	.timer		= &s3c24xx_timer,
MACHINE_END
