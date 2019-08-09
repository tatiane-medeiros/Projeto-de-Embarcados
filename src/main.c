#include <zephyr.h>
#include <gpio.h>
#include <shell/shell.h>
#include <stdlib.h>

/* Sleep time */
#define SLEEP_TIME	500


int main(void) {
    
    while(1){
        u32_t value = 0U;
	    k_sleep(SLEEP_TIME);
    }

    return 0;
}
