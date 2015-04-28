# 添加板相关代码 #
为新板子在arch/arm/mach-xxx添加Kconfig和Makefile项目：
```
Index: arch/arm/mach-s3c2410/Kconfig
===================================================================
--- arch/arm/mach-s3c2410/Kconfig	(revision 87)
+++ arch/arm/mach-s3c2410/Kconfig	(working copy)
@@ -139,4 +139,10 @@
 	help
 	   Say Y here if you are using the Armzone QT2410
 
+config MACH_SKYEYE2410
+	bool "skyeye 2410 board"
+	select CPU_S3C2410
+	help
+	   Say Y here if you are using the skyeye to simulate S3C2410
+
 endmenu
Index: arch/arm/mach-s3c2410/Makefile
===================================================================
--- arch/arm/mach-s3c2410/Makefile	(revision 87)
+++ arch/arm/mach-s3c2410/Makefile	(working copy)
@@ -29,6 +29,7 @@
 obj-$(CONFIG_MACH_TCT_HAMMER)	+= mach-tct_hammer.o
 obj-$(CONFIG_MACH_VR1000)	+= mach-vr1000.o usb-simtec.o
 obj-$(CONFIG_MACH_QT2410)	+= mach-qt2410.o
+obj-$(CONFIG_MACH_SKYEYE2410)	+= mach-skyeye2410.o
 
 # Common bits of machine support
```
添加新的板文件arch/arm/mach-s3c2410/mach-skyeye2410.c：
```
/* linux/arch/arm/mach-s3c2410/mach-skyeye2410.c
 *
 * linux/arch/arm/mach-s3c2410/mach-skyeye2410.c
 *
 * Copyright (C) 2010 by Barry Song
 * All rights reserved.
 *
 * @Author: Barry Song
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 *
 * @History:
 * derived from linux/arch/arm/mach-s3c2410/mach-bast.c, written by
 * Ben Dooks <ben@simtec.co.uk>
 *
 ***********************************************************************/

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
#include <plat/iic.h>

#include <plat/devs.h>
#include <plat/cpu.h>

#include <plat/common-smdk.h>

#include <mach/regs-lcd.h>
#include <mach/fb.h>

static struct map_desc skyeye2410_iodesc[] __initdata = {
	/* nothing here yet */
	{
		.virtual=  0xe0000000,
		.pfn= __phys_to_pfn(0x19000000),
		.length= SZ_1M,
		.type= MT_DEVICE
	},
};

#define LCD_WIDTH 320
#define LCD_HEIGHT 240
#define LCD_PIXCLOCK 40000
#define LCD_RIGHT_MARGIN 67
#define LCD_LEFT_MARGIN 40
#define LCD_HSYNC_LEN 31
#define LCD_UPPER_MARGIN 25
#define LCD_LOWER_MARGIN 5
#define LCD_VSYNC_LEN 

static struct s3c2410fb_display skyeye2410_lcd_cfg __initdata = {
	.lcdcon5        = S3C2410_LCDCON5_FRM565 |
					  S3C2410_LCDCON5_INVVLINE |
		              S3C2410_LCDCON5_INVVFRAME |
		              S3C2410_LCDCON5_PWREN,
	.width          = LCD_WIDTH,
	.height         = LCD_HEIGHT,
	.xres           = LCD_WIDTH,
	.yres           = LCD_HEIGHT,
	.bpp            = 16,
	.left_margin    = LCD_LEFT_MARGIN + 1,
	.right_margin   = LCD_RIGHT_MARGIN + 1,
	.hsync_len      = LCD_HSYNC_LEN + 1,
	.upper_margin   = LCD_UPPER_MARGIN + 1,
	.lower_margin   = LCD_LOWER_MARGIN + 1,
	.vsync_len      = LCD_VSYNC_LEN + 1,
	.pixclock       = LCD_PIXCLOCK,
	.type           = S3C2410_LCDCON1_TFT,
};

static struct s3c2410fb_mach_info s3c2410_lcd_info __initdata = {
	.displays       = &skyeye2410_lcd_cfg,
	.num_displays   = 1,
	.default_display = 0,
	.gpdcon =       0xaa95aaa1,
	.gpdcon_mask =  0xffc0fff0,
	.gpdup =        0x0000faff,
	.gpdup_mask =   0xffffffff,
	.lpcsel         = 0xf82,
};

static void __init skyeye2410_lcd_init(void)
{
    s3c24xx_fb_set_platdata(&s3c2410_lcd_info);
}

#define UCON S3C2410_UCON_DEFAULT
#define ULCON S3C2410_LCON_CS8 | S3C2410_LCON_PNONE | S3C2410_LCON_STOPB
#define UFCON S3C2410_UFCON_RXTRIG8 | S3C2410_UFCON_FIFOMODE

static struct s3c2410_uartcfg skyeye2410_uartcfgs[] __initdata = {
	[0] = {
		.hwport	     = 0,
		.flags	     = 0,
		.ucon	     = UCON,
		.ulcon	     = ULCON,
		.ufcon	     = UFCON,
	},
	[1] = {
		.hwport	     = 1,
		.flags	     = 0,
		.ucon	     = UCON,
		.ulcon	     = ULCON,
		.ufcon	     = UFCON,
	},
	[2] = {
		.hwport	     = 2,
		.flags	     = 0,
		.ucon	     = UCON,
		.ulcon	     = ULCON,
		.ufcon	     = UFCON,
	}
};

static struct platform_device *skyeye2410_devices[] __initdata = {
	/*&s3c_device_usb,*/
	&s3c_device_lcd,
	&s3c_device_wdt,
	/*&s3c_device_i2c0,*/
	/*&s3c_device_iis,*/
};

static void __init skyeye2410_map_io(void)
{
	s3c24xx_init_io(skyeye2410_iodesc, ARRAY_SIZE(skyeye2410_iodesc));
	s3c24xx_init_clocks(0);
	s3c24xx_init_uarts(skyeye2410_uartcfgs, ARRAY_SIZE(skyeye2410_uartcfgs));
}

static void __init skyeye2410_init(void)
{
	s3c_i2c0_set_platdata(NULL);
	platform_add_devices(skyeye2410_devices, ARRAY_SIZE(skyeye2410_devices));
	skyeye2410_lcd_init();
	smdk_machine_init();
}

MACHINE_START(SMDK2410, "skyeye2410") /* @TODO: request a new identifier and switch
				    * to skyeye2410 */
	/* Maintainer: Barry Song */
	.phys_io	= S3C2410_PA_UART,
	.io_pg_offst	= (((u32)S3C24XX_VA_UART) >> 18) & 0xfffc,
	.boot_params	= S3C2410_SDRAM_PA + 0x100,
	.map_io		= skyeye2410_map_io,
	.init_irq	= s3c24xx_init_irq,
	.init_machine	= skyeye2410_init,
	.timer		= &s3c24xx_timer,
MACHINE_END
```

# 配置内核并编译 #
配置内核，选择skyeye2410板:
```
System Type  ---> 
	S3C2410 Machines  --->
                 [ ] SMDK2410/A9M2410    
                 [ ] IPAQ H1940          
                 [ ] Acer N30 family          
                 [ ] Simtec Electronics BAST (EB2410ITX)       
                 [ ] NexVision OTOM Board                                                               
                 [ ] AML M5900 Series                                                   
                 [ ] TCT Hammer Board                                                                 
                 [ ] Thorcom VR1000                                     
                 [ ] QT2410    
		 [*] skyeye 2410 board
```

编译内核。得到的vmlinux可以直接在skyeye运行：
```
bhsong@bhsong-laptop:~/develop/training/skyeye/training-simulation-new-board$ sudo skyeye -e vmlinux -c skyeye-standalone.conf 
big_endian is false.
arch: arm
cpu info: armv4, arm920t, 41009200, ff00fff0, 2 
mach info: name s3c2410x, mach_init addr 0x805f030
ethmod num=1, mac addr=0:4:3:2:1:f, hostip=10.0.0.1
lcd_mod:1
uart_mod:0, desc_in:, desc_out:, converter:
SKYEYE: use arm920t mmu ops
Loaded RAM   ./initrd.img
start addr is set to 0xc0008000 by exec file.
Linux version 2.6.31.6-svn79 (bhsong@bhsong-laptop) (gcc version 4.2.2) #34 Sat Jan 16 18:38:55 CST 2010
CPU: ARM920T [41009200] revision 0 (ARMvundefined/unknown), cr=00003177
CPU: VIVT data cache, VIVT instruction cache
Machine: skyeye2410
...
```

另外，我们也可以为skyeye2410电路板分配一个新的machine ID,这样就不再需要在`MACHINE_START(SMDK2410, "skyeye2410")`等处使用smdk2410的ID了。


# 体验通过修改platform data来变更驱动属性 #
将arch/arm/mach-s3c2410/mach-skyeye2410.c中的
```
#define LCD_WIDTH 320
#define LCD_HEIGHT 240
```
变更为
```
#define LCD_WIDTH 240
#define LCD_HEIGHT 160
```
重新编译内核，在skyeye运行，观察LCD窗口的变化。