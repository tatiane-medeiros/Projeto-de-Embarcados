#include "button.h"

void button_configure(struct button *btn, gpio_callback_handler_t cb){
	btn->device = device_get_binding(PORT);
	if(btn->device != NULL){
		gpio_pin_configure(btn->device, BUTTON, SW_FLAGS);
		gpio_init_callback(&btn->gpio_callback, cb, BIT(BUTTON));
		gpio_add_callback(btn->device, &btn->gpio_callback);
		gpio_pin_enable_callback(btn->device, BUTTON);
	}
}


void button_read(struct button *btn, u32_t *state){
	gpio_pin_read(btn->device, BUTTON, state);	
}

void changeState(){
	if(state == 0) state = 1;
	else if(state == 1) state = 2;
	else state = 0;	
}

void setState(u8_t val){
	if(val >= 0 && val <3) state = val;
}
