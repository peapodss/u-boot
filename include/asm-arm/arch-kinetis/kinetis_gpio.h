/*
 * (C) Copyright 2011
 *
 * Alexander Potashev, Emcraft Systems, aspotashev@emcraft.com
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

#ifndef _KINETIS_GPIO_H_
#define _KINETIS_GPIO_H_

/*
 * Bits and bit groups inside the PCR registers (Pin Control Registers)
 */
/* Pin Mux Control (selects pin function) */
#define KINETIS_GPIO_CONFIG_MUX_BITS	8
/* Pull Enable (pull-down by default) */
#define KINETIS_GPIO_CONFIG_PE_BIT	1
#define KINETIS_GPIO_CONFIG_PE_MSK	(1 << KINETIS_GPIO_CONFIG_PE_BIT)
/* Pull Select (0=pull-down, 1=pull-up) */
#define KINETIS_GPIO_CONFIG_PS_BIT	0
#define KINETIS_GPIO_CONFIG_PS_MSK	(1 << KINETIS_GPIO_CONFIG_PS_BIT)
/* Drive Strength Enable (high drive strength) */
#define KINETIS_GPIO_CONFIG_DSE_MSK	(1 << 6)

/*
 * These macros should be used to compute the value for the second argument of
 * `kinetis_gpio_config()` (`regval`). This value will be copied into a PCR
 * register.
 */
/* The simplest macro that only allow to configure the MUX bits */
#define KINETIS_GPIO_CONFIG_MUX(mux) \
	(mux << KINETIS_GPIO_CONFIG_MUX_BITS)
/* Also enable the pull-down register */
#define KINETIS_GPIO_CONFIG_PULLDOWN(mux) \
	(KINETIS_GPIO_CONFIG_MUX(mux) | KINETIS_GPIO_CONFIG_PE_MSK)
/* Also enable the pull-up register */
#define KINETIS_GPIO_CONFIG_PULLUP(mux) \
	(KINETIS_GPIO_CONFIG_MUX(mux) | \
	KINETIS_GPIO_CONFIG_PE_MSK | KINETIS_GPIO_CONFIG_PS_MSK)
/* Also enable high drive strength */
#define KINETIS_GPIO_CONFIG_DSE(mux) \
	(KINETIS_GPIO_CONFIG_MUX(mux) | KINETIS_GPIO_CONFIG_DSE_MSK)
/*
 * TBD: similar macros with more options
 */

/*
 * Number of pins in all ports
 */
#define KINETIS_GPIO_PORT_PINS		32

/*
 * GPIO ports
 */
#define KINETIS_GPIO_PORT_A	0
#define KINETIS_GPIO_PORT_B	1
#define KINETIS_GPIO_PORT_C	2
#define KINETIS_GPIO_PORT_D	3
#define KINETIS_GPIO_PORT_E	4
/* PORT F on K70 */
#if KINETIS_GPIO_PORTS > 5
#define KINETIS_GPIO_PORT_F	5
#endif /* KINETIS_GPIO_PORTS > 5 */

#if KINETIS_GPIO_PORTS < 5 || KINETIS_GPIO_PORTS > 6
#error KINETIS_GPIO_PORTS is 5 or 6 on all supported Kinetis MCUs
#endif

/*
 * GPIO configuration registers
 */
#define KINETIS_GPIO_BASE	0x400FF000
#define KINETIS_GPIO(a)		((volatile struct kinetis_gpio *) \
				(KINETIS_GPIO_BASE + (a)*0x40))

struct kinetis_gpio {
	u32 pdor; /* data output */
	u32 psor; /* set output */
	u32 pcor; /* clear output */
	u32 ptor; /* toggle output */
	u32 pdir; /* data input */
	u32 pddr; /* data direction */
};

/*
 * GPIO descriptor
 */
struct kinetis_gpio_dsc {
	unsigned int port;	/* GPIO port */
	unsigned int pin;	/* GPIO pin */
};

struct kinetis_gpio_pin_config {
	struct kinetis_gpio_dsc dsc;
	u32 regval;	/* Value for writing into the PCR register */
};

/*
 * Configure the specified GPIO pin.
 * Returns 0 on success, -EINVAL otherwise.
 */
int kinetis_gpio_config(const struct kinetis_gpio_dsc *dsc, u32 regval);

/*
 * Configure a set of GPIO pins using the given configuration table.
 * Returns 0 on success.
 */
extern int kinetis_gpio_config_table(
	const struct kinetis_gpio_pin_config *table, unsigned int len);

#endif /* _KINETIS_GPIO_H_ */
