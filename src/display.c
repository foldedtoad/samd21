/*
 * Copyright (c) 2018 PHYTEC Messtechnik GmbH
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <zephyr.h>
#include <device.h>
#include <display/cfb.h>
#include <stdio.h>

#include <logging/log.h>
LOG_MODULE_REGISTER(display, 3);

#define DISPLAY_DRIVER      DT_LABEL(DT_INST(0, solomon_ssd1306fb))

#define MAX_FONTS  42

#define SELECTED_FONT_INDEX  0

static const struct device * dev;
static uint16_t rows;
static uint8_t ppt;
static uint8_t font_width;
static uint8_t font_height;

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*---------------------------------------------------------------------------*/
void display_play(void) 
{
    if (rows == 0) return;

    while (1) {

        for (int i = 0; i < rows; i++) {
            
            cfb_framebuffer_clear(dev, false);

            if (cfb_print(dev, "0123456789mMgj!\"§$%&/()=", 0, i * ppt)) {
                LOG_ERR("Failed to print a string");
                continue;
            }

            cfb_framebuffer_finalize(dev);
            
            k_sleep(K_MSEC(200));
        }
    }
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*---------------------------------------------------------------------------*/
void display_init(void)
{
    dev = device_get_binding(DISPLAY_DRIVER);

    if (dev == NULL) {
        LOG_ERR("Device not found: %s", DISPLAY_DRIVER);
        return;
    }

    if (display_set_pixel_format(dev, PIXEL_FORMAT_MONO10) != 0) {
        LOG_ERR("Failed to set required pixel format");
        return;
    }

    LOG_INF("Initialized %s", DISPLAY_DRIVER);

    if (cfb_framebuffer_init(dev)) {
        LOG_INF("Framebuffer initialization failed!");
        return;
    }

    cfb_framebuffer_clear(dev, true);

    display_blanking_off(dev);

    rows = cfb_get_display_parameter(dev, CFB_DISPLAY_ROWS);
    ppt  = cfb_get_display_parameter(dev, CFB_DISPLAY_PPT);

    for (int idx = 0; idx < MAX_FONTS; idx++) {

        if (cfb_get_font_size(dev, idx, &font_width, &font_height)) {
            break;  // end of font list, so exit loop.
        }

        LOG_INF("index[%d] font width %d, font height %d",
                idx, font_width, font_height);
    }

    cfb_framebuffer_set_font(dev, SELECTED_FONT_INDEX);

    LOG_INF("selected font: index[%d]", SELECTED_FONT_INDEX);

    LOG_INF("x_res %d, y_res %d, ppt %d, rows %d, cols %d",
            cfb_get_display_parameter(dev, CFB_DISPLAY_WIDTH),
            cfb_get_display_parameter(dev, CFB_DISPLAY_HEIGH),
            ppt,
            rows,
            cfb_get_display_parameter(dev, CFB_DISPLAY_COLS));
}