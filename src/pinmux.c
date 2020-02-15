/*
 * Copyright (c) 2018 Henrik Brix Andersen <henrik@brixandersen.dk>
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <init.h>
#include <drivers/pinmux.h>

static int board_pinmux_init2(struct device *dev)
{
	struct device * muxa = device_get_binding(DT_ATMEL_SAM0_PINMUX_PINMUX_A_LABEL);

	ARG_UNUSED(dev);

#if DT_ATMEL_SAM0_I2C_SERCOM_3_BASE_ADDRESS
	/* I2C, SCL = PA23, SDA = PA22 */
	pinmux_pin_set(muxa, 22, PINMUX_FUNC_C);
	pinmux_pin_set(muxa, 23, PINMUX_FUNC_C);
#endif

	return 0;
}

SYS_INIT(board_pinmux_init2, PRE_KERNEL_1, CONFIG_PINMUX_INIT_PRIORITY);
