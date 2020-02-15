/*
 *   buttons.c
 */
#include <zephyr.h>
#include <device.h>
#include <drivers/gpio.h>
#include <sys/util.h>
#include <sys/printk.h>
#include <inttypes.h>

#include "board.h"

#include <logging/log.h>
LOG_MODULE_REGISTER(buttons, 3);

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*---------------------------------------------------------------------------*/

#define SW_A        7  //EXT_D5_GPIO_PIN
#define SW_B        20 //EXT_D6_GPIO_PIN
#define SW_C        15 //EXT_D9_GPIO_PIN
#define EDGE        (GPIO_INT_EDGE | GPIO_INT_ACTIVE_LOW)
#define PULL_UP     GPIO_PUD_PULL_UP

#define SLEEP_TIME	1000

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*---------------------------------------------------------------------------*/

typedef struct {
    u8_t    id;
    u8_t    pin;
    u32_t   bit;
    char  * name;
} button_info_t; 

static const button_info_t button_info [] = {
    { .id = 1, .pin = 7,  .bit = 0x00000080, .name = "SW_A" },
    { .id = 2, .pin = 20, .bit = 0x00100000, .name = "SW_B" },
    { .id = 3, .pin = 15, .bit = 0x00008000, .name = "SW_C" },
};
#define BUTTONS_COUNT (sizeof(button_info)/sizeof(button_info_t))

static const button_info_t unknown = {.id=0, .pin=0 , .bit= 0, .name= "???"};

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*---------------------------------------------------------------------------*/
static struct device * gpiob;
static struct gpio_callback gpio_cb_a;
static struct gpio_callback gpio_cb_b;
static struct gpio_callback gpio_cb_c;

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*---------------------------------------------------------------------------*/
static const button_info_t * button_get_info(u32_t pins)
{
    for (int i=0; i < BUTTONS_COUNT; i++) {
        if (button_info[i].bit & pins) {
            return &button_info[i];
        }
    }
    return &unknown;
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*---------------------------------------------------------------------------*/
void button_event(struct device * gpiob, struct gpio_callback * cb, u32_t pins)
{
    static const button_info_t * info;

    info = button_get_info(pins);

    LOG_INF("Button: %s  pin(%d)", info->name, info->pin);
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*---------------------------------------------------------------------------*/
void buttons_init(void)
{
    LOG_INF("%s", __func__);

	gpiob = device_get_binding(SW_GPIO_NAME);
	if (!gpiob) {
		LOG_ERR("error");
		return;
	}

	gpio_pin_configure(gpiob, SW_A, GPIO_DIR_IN | GPIO_INT |  PULL_UP | EDGE);
	gpio_pin_configure(gpiob, SW_B, GPIO_DIR_IN | GPIO_INT |  PULL_UP | EDGE);
	gpio_pin_configure(gpiob, SW_C, GPIO_DIR_IN | GPIO_INT |  PULL_UP | EDGE);

	gpio_init_callback(&gpio_cb_a, button_event, BIT(SW_A));
	gpio_init_callback(&gpio_cb_b, button_event, BIT(SW_B));
	gpio_init_callback(&gpio_cb_c, button_event, BIT(SW_C));

	gpio_add_callback(gpiob, &gpio_cb_a);
	gpio_add_callback(gpiob, &gpio_cb_b);
	gpio_add_callback(gpiob, &gpio_cb_c);

	gpio_pin_enable_callback(gpiob, SW_A);
	gpio_pin_enable_callback(gpiob, SW_B);
	gpio_pin_enable_callback(gpiob, SW_C);
}