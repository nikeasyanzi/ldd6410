/*
 * (C) Copyright 2002
 * David Mueller, ELSOFT AG, d.mueller@elsoft.ch
 *
 * See file CREDITS for list of people who contributed to this
 * project.
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
 */

/* This code should work for both the S3C2400 and the S3C2410
 * as they seem to have the same I2C controller inside.
 * The different address mapping is handled by the s3c24xx.h files below.
 */

#include <common.h>

#ifdef CONFIG_S3C24XX_I2C

#include <regs.h>

#include <i2c.h>
#include <asm/io.h>

#ifdef CONFIG_HARD_I2C

#define	I2C_WRITE	0
#define I2C_READ	1

#define I2C_OK		0
#define I2C_NOK		1
#define I2C_NACK	2
#define I2C_NOK_LA	3		/* Lost arbitration */
#define I2C_NOK_TOUT	4		/* time out */

#define I2CSTAT_BSY	(1<<5)		/* Busy bit */
#define I2CSTAT_NACK	(1<<0)		/* Nack bit */

#define I2CCON_IRPND	(1<<4)		/* Interrupt pending bit */

#define I2C_MODE_MT	0xC0		/* Master Transmit Mode */
#define I2C_MODE_MR	0x80		/* Master Receive Mode */
#define I2C_START_STOP	(1<<5)		/* START / STOP */
#define I2C_TXRX_ENA	(1<<4)		/* I2C Tx/Rx enable */

#define I2C_TIMEOUT 1			/* 1 second */


static int GetI2CSDA(void)
{
#ifdef CONFIG_S3C24A0
	S3C24X0_GPIO * const gpio = S3C24X0_GetBase_GPIO();
	return (gpio->PGDAT & 0x0020) >> 5;
#else
	uint val;

//	printf("GetI2CSDA()\n");

	val = readl(ELFIN_GPIO_BASE+GPEDAT_OFFSET);
	return ((val>>15)&0x00000001);
#endif
}

#if 0
static void SetI2CSDA(int x)
{
	rGPEDAT = (rGPEDAT & ~0x8000) | (x&1) << 15;
}
#endif

static void SetI2CSCL(int x)
{
#ifdef CONFIG_S3C24A0
	S3C24X0_GPIO * const gpio = S3C24X0_GetBase_GPIO();
	gpio->PGDAT = (gpio->PGDAT & ~0x0040) | (x&1) << 6;
#else
	uint val = 0;

//	printf("SetI2CSCL()\n");

	val = readl(ELFIN_GPIO_BASE+GPEDAT_OFFSET);
	val &= ~(1<<14);
	val |= (x&0x00000001)<<14;
	writel(val, ELFIN_GPIO_BASE+GPEDAT_OFFSET);
#endif
}


static int WaitForXfer (void)
{
	S3C24X0_I2C *const i2c = S3C24X0_GetBase_I2C ();
	int i, status;

//	printf("WaitForXfer()\n");

	i = I2C_TIMEOUT * 10000;
	status = i2c->IICCON;
	while ((i > 0) && !(status & I2CCON_IRPND)) {
		udelay (100);
		status = i2c->IICCON;
		i--;
	}

	return (status & I2CCON_IRPND) ? I2C_OK : I2C_NOK_TOUT;
}

static int IsACK (void)
{
	S3C24X0_I2C *const i2c = S3C24X0_GetBase_I2C ();

//	printf("IsACK()\n");

	return (!(i2c->IICSTAT & I2CSTAT_NACK));
}

static void ReadWriteByte (void)
{
	S3C24X0_I2C *const i2c = S3C24X0_GetBase_I2C ();

//	printf("ReadWriteByte()\n");

	i2c->IICCON &= ~I2CCON_IRPND;
}

void i2c_init (int speed, int slaveadd)
{
	S3C24X0_I2C * const i2c = (S3C24X0_I2C *)ELFIN_I2C_BASE;
//	S3C24X0_GPIO * const gpio = (S3C24X0_GPIO *)ELFIN_GPIO_BASE;
	ulong freq, pres = 16, div;
	int i, status;

//	printf("i2c_init()\n");

#ifndef CONFIG_S3C24A0
	/* set GPIO value for i2c */
	writel(0xa0000000, ELFIN_GPIO_BASE+GPECON_OFFSET);
#endif

	/* wait for some time to give previous transfer a chance to finish */

	i = I2C_TIMEOUT * 1000;
	status = i2c->IICSTAT;
	while ((i > 0) && (status & I2CSTAT_BSY)) {
		udelay (1000);
		status = i2c->IICSTAT;
		i--;
	}

	if ((status & I2CSTAT_BSY) || GetI2CSDA () == 0) {
		/* bus still busy probably by (most) previously interrupted transfer */
#ifdef CONFIG_S3C24A0
		ulong old_gpecon = gpio->PGCON;
		/* set I2CSDA and I2CSCL (PG5, PG6) to GPIO */
		gpio->PGCON = (gpio->PGCON & ~0x00003c00) | 0x00000c00;
#endif

		/* toggle I2CSCL until bus idle */
		SetI2CSCL (0);
		udelay (1000);
		i = 10;
		while ((i > 0) && (GetI2CSDA () != 1)) {
			SetI2CSCL (1);
			udelay (1000);
			SetI2CSCL (0);
			udelay (1000);
			i--;
		}
		SetI2CSCL (1);
		udelay (1000);

		/* restore pin functions */
#ifdef CONFIG_S3C24A0
		gpio->PGCON = old_gpecon;
#endif
	}

	/* calculate prescaler and divisor values */
	freq = get_PCLK ();
	if ((freq / pres / (16 + 1)) > speed)
		/* set prescaler to 512 */
		pres = 512;

	div = 0;
	while ((freq / pres / (div + 1)) > speed)
		div++;

	/* set prescaler, divisor according to freq, also set
	 * ACKGEN, IRQ */
	i2c->IICCON = (div & 0x0F) | 0xA0 | ((pres == 512) ? 0x40 : 0);

	/* init to SLAVE REVEIVE and set slaveaddr */
	i2c->IICSTAT = 0;
	i2c->IICADD = slaveadd;
	/* program Master Transmit (and implicit STOP) */
	i2c->IICSTAT = I2C_MODE_MT | I2C_TXRX_ENA;

}

/*
 * cmd_type is 0 for write, 1 for read.
 *
 * addr_len can take any value from 0-255, it is only limited
 * by the char, we could make it larger if needed. If it is
 * 0 we skip the address write cycle.
 */
static int i2c_transfer (unsigned char cmd_type,
		  unsigned char chip,
		  unsigned char addr[],
		  unsigned char addr_len,
		  unsigned char data[], unsigned short data_len)
{
	S3C24X0_I2C *const i2c = S3C24X0_GetBase_I2C ();
	int i, status, result;

//	printf("i2c_transfer(%02x, %d)\n", cmd_type, chip);

	if (data == 0 || data_len == 0) {
		/*Don't support data transfer of no length or to address 0 */
		printf ("i2c_transfer: bad call\n");
		return I2C_NOK;
	}

	/* Check I2C bus idle */
	i = I2C_TIMEOUT * 1000;
	status = i2c->IICSTAT;
//	printf("1: status %08x\n", status);
	while ((i > 0) && (status & I2CSTAT_BSY)) {
		udelay (1000);
		status = i2c->IICSTAT;
		i--;
	}

	if (status & I2CSTAT_BSY)
		return I2C_NOK_TOUT;

	i2c->IICCON |= 0x80;
	result = I2C_OK;

	switch (cmd_type) {
	case I2C_WRITE:
//		printf("2: write\n");
		if (addr && addr_len) {
			i2c->IICDS = chip;
			/* send START */
			i2c->IICSTAT = I2C_MODE_MT | I2C_TXRX_ENA | I2C_START_STOP;
			i = 0;
			while ((i < addr_len) && (result == I2C_OK)) {
				result = WaitForXfer ();
				i2c->IICDS = addr[i];
				ReadWriteByte ();
				i++;
			}
			i = 0;
			while ((i < data_len) && (result == I2C_OK)) {
				result = WaitForXfer ();
				i2c->IICDS = data[i];
				ReadWriteByte ();
				i++;
			}
		}
		else {
			i2c->IICDS = chip;
			/* send START */
			i2c->IICSTAT = I2C_MODE_MT | I2C_TXRX_ENA | I2C_START_STOP;
			i = 0;
			while ((i < data_len) && (result = I2C_OK)) {
				result = WaitForXfer ();
				i2c->IICDS = data[i];
				ReadWriteByte ();
				i++;
			}
		}

		if (result == I2C_OK)
			result = WaitForXfer ();

		/* send STOP */
		i2c->IICSTAT = I2C_MODE_MR | I2C_TXRX_ENA;
		ReadWriteByte ();
		break;

	case I2C_READ:
//		printf("2: read\n");
		if (addr && addr_len) {
			i2c->IICSTAT = I2C_MODE_MT | I2C_TXRX_ENA;
			i2c->IICDS = chip;
			/* send START */
			i2c->IICSTAT |= I2C_START_STOP;
			result = WaitForXfer ();
			if (IsACK ()) {
				i = 0;
				while ((i < addr_len) && (result == I2C_OK)) {
					i2c->IICDS = addr[i];
					ReadWriteByte ();
					result = WaitForXfer ();
					i++;
				}

				i2c->IICDS = chip;
				/* resend START */
				i2c->IICSTAT =  I2C_MODE_MR | I2C_TXRX_ENA |
						I2C_START_STOP;
				ReadWriteByte ();
				result = WaitForXfer ();
				i = 0;
				while ((i < data_len) && (result == I2C_OK)) {
					/* disable ACK for final READ */
					if (i == data_len - 1)
						i2c->IICCON &= ~0x80;
					ReadWriteByte ();
					result = WaitForXfer ();
					data[i] = i2c->IICDS;
					i++;
				}
			} else {
				result = I2C_NACK;
			}

		}
		else {
			i2c->IICSTAT = I2C_MODE_MR | I2C_TXRX_ENA;
			i2c->IICDS = chip;
			/* send START */
			i2c->IICSTAT |= I2C_START_STOP;
			result = WaitForXfer ();

			if (IsACK ()) {
				i = 0;
				while ((i < data_len) && (result == I2C_OK)) {
					/* disable ACK for final READ */
					if (i == data_len - 1)
						i2c->IICCON &= ~0x80;
					ReadWriteByte ();
					result = WaitForXfer ();
					data[i] = i2c->IICDS;
					i++;
				}
			} else {
				result = I2C_NACK;
			}
		}

		/* send STOP */
		i2c->IICSTAT = I2C_MODE_MR | I2C_TXRX_ENA;
		ReadWriteByte ();
		break;

	default:
		printf ("i2c_transfer: bad call\n");
		result = I2C_NOK;
		break;
	}

	return result;
}

int i2c_probe (uchar chip)
{
	uchar buf[1];

	buf[0] = 0;

//	printf("i2c_probe(%d)\n", chip);

	/*
	 * What is needed is to send the chip address and verify that the
	 * address was <ACK>ed (i.e. there was a chip at that address which
	 * drove the data line low).
	 */
	return (i2c_transfer (I2C_READ, chip << 1, 0, 0, buf, 1) != I2C_OK);
}

int i2c_read (uchar chip, uint addr, int alen, uchar * buffer, int len)
{
	uchar xaddr[4];
	int ret;

//	printf("i2c_read(%d)\n", chip);

	if (alen > 4) {
		printf ("I2C read: addr len %d not supported\n", alen);
		return 1;
	}

	if (alen > 0) {
		xaddr[0] = (addr >> 24) & 0xFF;
		xaddr[1] = (addr >> 16) & 0xFF;
		xaddr[2] = (addr >> 8) & 0xFF;
		xaddr[3] = addr & 0xFF;
	}

#ifdef CFG_I2C_EEPROM_ADDR_OVERFLOW
	/*
	 * EEPROM chips that implement "address overflow" are ones
	 * like Catalyst 24WC04/08/16 which has 9/10/11 bits of
	 * address and the extra bits end up in the "chip address"
	 * bit slots. This makes a 24WC08 (1Kbyte) chip look like
	 * four 256 byte chips.
	 *
	 * Note that we consider the length of the address field to
	 * still be one byte because the extra address bits are
	 * hidden in the chip address.
	 */
	if (alen > 0)
		chip |= ((addr >> (alen * 8)) & CFG_I2C_EEPROM_ADDR_OVERFLOW);
#endif
	if ((ret = i2c_transfer (I2C_READ, chip << 1, &xaddr[4 - alen], alen, buffer, len)) != 0) {
		printf ("I2c read: failed %d\n", ret);
		return 1;
	}
	return 0;
}

int i2c_write (uchar chip, uint addr, int alen, uchar * buffer, int len)
{
	uchar xaddr[4];

//	printf("i2c_write(%d)\n", chip);

	if (alen > 4) {
		printf ("I2C write: addr len %d not supported\n", alen);
		return 1;
	}

	if (alen > 0) {
		xaddr[0] = (addr >> 24) & 0xFF;
		xaddr[1] = (addr >> 16) & 0xFF;
		xaddr[2] = (addr >> 8) & 0xFF;
		xaddr[3] = addr & 0xFF;
	}
#ifdef CFG_I2C_EEPROM_ADDR_OVERFLOW
	/*
	 * EEPROM chips that implement "address overflow" are ones
	 * like Catalyst 24WC04/08/16 which has 9/10/11 bits of
	 * address and the extra bits end up in the "chip address"
	 * bit slots. This makes a 24WC08 (1Kbyte) chip look like
	 * four 256 byte chips.
	 *
	 * Note that we consider the length of the address field to
	 * still be one byte because the extra address bits are
	 * hidden in the chip address.
	 */
	if (alen > 0)
		chip |= ((addr >> (alen * 8)) & CFG_I2C_EEPROM_ADDR_OVERFLOW);
#endif
	return (i2c_transfer (I2C_WRITE, chip << 1, &xaddr[4 - alen], alen, buffer, len) != 0);
}
#endif	/* CONFIG_HARD_I2C */

#endif /* CONFIG_DRIVER_S3C24X0_I2C */