/*
 * @file main.c
 * @author Maria Tatiane Medeiros dos Santos
 * @author Glauber de Arruda Braga
 * 
 */
#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <zephyr.h>
#include <gpio.h>
#include <shell/shell.h>
#include <stdlib.h>
#include <misc/__assert.h>
#include <device.h>
#include <i2c.h>

#include "ina.h"

/* Sleep time */
#define SLEEP_TIME	500

/* size of stack area used by each thread */
#define STACKSIZE 1024

/* scheduling priority used by each thread */
#define PRIORITY 7

// endereço do INA219
#define I2C_SLV_ADDR	0x40
// calibração
#define CAL_VAL		(4096)

static int reset(const struct shell *shell, size_t argc, char **argv){
	shell_print(shell, "Reset\n");
	return 0;
}

SHELL_STATIC_SUBCMD_SET_CREATE(medidor_sub,
	SHELL_CMD_ARG(reset, NULL, "Reset.", reset, 1, NULL),
	SHELL_SUBCMD_SET_END /* Array terminated. */
);
SHELL_CMD_REGISTER(medidor, &medidor_sub, "Comandos usando shell.", NULL);


void ina1(void)
{
	ina();
}

// Threads

K_THREAD_DEFINE(ina_1, STACKSIZE, ina1, NULL, NULL, NULL,
		PRIORITY, 0, K_NO_WAIT);



/*
int main(void) {
    
    while(1){
        u32_t value = 0U;
	    k_sleep(SLEEP_TIME);
    }

    return 0;
}
*/