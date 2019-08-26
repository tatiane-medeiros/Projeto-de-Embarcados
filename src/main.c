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
#include "button.h"
#include "ble.h"

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

// botão
static struct button btn;
// estados: 0 - reg corrente, 1 - reg tensao, 2 - reg potencia
static u8_t state = 0;
static u32_t global_ina = 0;

static int reset(const struct shell *shell, size_t argc, char **argv){
	shell_print(shell, "Reset\n");
	state = 0;
	return 0;
}

static int setCurrent(const struct shell *shell, size_t argc, char **argv){
	shell_print(shell, "Medindo corrente\n");
	state = 0;
	return 0;
}
static int setPower(const struct shell *shell, size_t argc, char **argv){
	shell_print(shell, "Medindo potência\n");
	state = 2;
	return 0;
}
static int setVoltage(const struct shell *shell, size_t argc, char **argv){
	shell_print(shell, "Medindo tensão\n");
	state = 1;
	return 0;
}

void setStatus(const struct shell *shell, size_t argc, char **argv){
	u8_t i = atoi(argv[1]);
	if (i >=0 && i<3){
		state = i;
	}
}

SHELL_STATIC_SUBCMD_SET_CREATE(medidor_sub,
	SHELL_CMD_ARG(reset, NULL, "Reset.", reset, 1, NULL),
	SHELL_CMD_ARG(current, NULL, "Medindo corrente.", setCurrent, 1, NULL),
	SHELL_CMD_ARG(power, NULL, "Medindo potência.", setPower, 1, NULL),
	SHELL_CMD_ARG(voltage, NULL, "Medindo tensão.", setVoltage, 1, NULL),
	SHELL_CMD_ARG(change, NULL, "Muda estado para x.", setStatus, 2, NULL),
	SHELL_SUBCMD_SET_END /* Array terminated. */
);
SHELL_CMD_REGISTER(medidor, &medidor_sub, "Comandos usando shell.", NULL);

void changeStatus(){
	if(state == 0) {
		state = 1;
	}
	else if(state == 1){
		state = 2;
	}
	else{
		state = 0;	
	}
	printf("state: %d\n", state);
}


void ina1(void)
{
	ina_calibration();
	while(1){
		global_ina = ina(state);
		k_sleep(DELAY);
	}
	
}


void status(void){
	//struct device *gpiob;	
	button_configure(&btn, changeStatus);
	while (1) {
		u32_t value = 0U;
		button_read(&btn, &value);
		k_sleep(SLEEP_TIME);
	}
	
}


void init_ble(void){
	
	vnd_init();
		
	while(1){
		vnd_notify(state, global_ina);
		k_sleep(SLEEP_TIME);
	}
	
}	



// Threads

K_THREAD_DEFINE(ina_1, STACKSIZE, ina1, NULL, NULL, NULL,
		PRIORITY, 0, K_NO_WAIT);
K_THREAD_DEFINE(button, STACKSIZE, status, NULL, NULL, NULL,
		PRIORITY, 0, K_NO_WAIT);
K_THREAD_DEFINE(ble, STACKSIZE, init_ble, NULL, NULL, NULL,
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