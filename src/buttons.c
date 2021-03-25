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

#define SW_A   DT_GPIO_PIN(DT_ALIAS(swa), gpios)
#define SW_B   DT_GPIO_PIN(DT_ALIAS(swb), gpios)
#define SW_C   DT_GPIO_PIN(DT_ALIAS(swc), gpios)

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*---------------------------------------------------------------------------*/

typedef struct {
    uint8_t    id;
    uint8_t    pin;
    uint32_t   bit;
    char  * name;
} button_info_t; 

static const button_info_t button_info [] = {
    { .id = 1, .pin = SW_A, .bit = 0x00000080, .name = "SW_A" },
    { .id = 2, .pin = SW_B, .bit = 0x00100000, .name = "SW_B" },
    { .id = 3, .pin = SW_C, .bit = 0x00008000, .name = "SW_C" },
};
#define BUTTONS_COUNT (sizeof(button_info)/sizeof(button_info_t))

static const button_info_t unknown = {.id=0, .pin=0 , .bit= 0, .name= "???"};

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*---------------------------------------------------------------------------*/
static const struct device * gpiob;
static struct gpio_callback gpio_cb_a;
static struct gpio_callback gpio_cb_b;
static struct gpio_callback gpio_cb_c;

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*---------------------------------------------------------------------------*/
static const button_info_t * button_get_info(uint32_t pins)
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
void button_event(const struct device * gpiob, struct gpio_callback * cb, uint32_t pins)
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

    /* Init Button Interrupt */
    int flags = (GPIO_INPUT      |
                 GPIO_ACTIVE_LOW |
                 GPIO_PULL_UP    |
                 GPIO_INT_EDGE);

    gpio_pin_configure(gpiob, SW_A, flags);
    gpio_pin_configure(gpiob, SW_B, flags);
    gpio_pin_configure(gpiob, SW_C, flags);

    gpio_pin_interrupt_configure(gpiob, SW_A, GPIO_INT_EDGE_TO_ACTIVE);
    gpio_pin_interrupt_configure(gpiob, SW_B, GPIO_INT_EDGE_TO_ACTIVE);
    gpio_pin_interrupt_configure(gpiob, SW_C, GPIO_INT_EDGE_TO_ACTIVE);

    gpio_init_callback(&gpio_cb_a, button_event, BIT(SW_A));
    gpio_init_callback(&gpio_cb_b, button_event, BIT(SW_B));
    gpio_init_callback(&gpio_cb_c, button_event, BIT(SW_C));

    gpio_add_callback(gpiob, &gpio_cb_a);
    gpio_add_callback(gpiob, &gpio_cb_b);
    gpio_add_callback(gpiob, &gpio_cb_c);
}