#include <zephyr.h>
#include <gpio.h>
#include <shell/shell.h>
#include <stdlib.h>

/* Sleep time */
#define SLEEP_TIME	500



static int reset(const struct shell *shell, size_t argc, char **argv){
	shell_print(shell, "Reset\n");
	return 0;
}

SHELL_STATIC_SUBCMD_SET_CREATE(medidor_sub,
	SHELL_CMD_ARG(reset, NULL, "Reset.", reset, 1, NULL),
	SHELL_SUBCMD_SET_END /* Array terminated. */
);
SHELL_CMD_REGISTER(medidor, &medidor_sub, "Comandos usando shell.", NULL);


int main(void) {
    
    while(1){
        u32_t value = 0U;
	    k_sleep(SLEEP_TIME);
    }

    return 0;
}
