/* 
 *  main.c - Application main entry point 
 */
#include <zephyr.h>
#include <sys/printk.h>

#include "buttons.h"
#include "display.h"

#include <logging/log.h>
LOG_MODULE_REGISTER(main, 3);

#define STACKSIZE 1024
#define PRIORITY 7

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*---------------------------------------------------------------------------*/
void main_thread(void * id, void * unused1, void * unused2)
{
    LOG_INF("%s", __func__);

    buttons_init();

    display_init();
    display_play();
}

K_THREAD_DEFINE(main_id, STACKSIZE, main_thread, 
                NULL, NULL, NULL, PRIORITY, 0, 0);
