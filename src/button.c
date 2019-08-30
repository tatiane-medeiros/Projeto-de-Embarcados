#include "button.h"

void button_configure(struct button *btn, gpio_callback_handler_t cb){
	btn->device = device_get_binding(PORT);
	if(btn->device != NULL){
		gpio_pin_configure(btn->device, BUTTON, SW_FLAGS);
		gpio_init_callback(&btn->gpio_callback, cb, BIT(BUTTON));
		gpio_add_callback(btn->device, &btn->gpio_callback);
		gpio_pin_enable_callback(btn->device, BUTTON);
		
		gpio_pin_configure(btn->device, PIN_A, SW_FLAGS);
		gpio_pin_configure(btn->device, PIN_B, SW_FLAGS);
		
	}
}


void button_read(struct button *btn, u32_t *state){
	gpio_pin_read(btn->device, BUTTON, state);	
}

void button_read_pins(struct button *btn, u32_t *valA, u32_t *valB){
	gpio_pin_read(btn->device, PIN_A, valA);	
	gpio_pin_read(btn->device, PIN_B, valB);	
	
}

u32_t button_read_pinA(struct button *btn){
	u32_t val = 0U;
	gpio_pin_read(btn->device, PIN_A, &val);
	return val;
	
}
u32_t button_read_pinB(struct button *btn){
	u32_t val = 0U;
	gpio_pin_read(btn->device, PIN_B, &val);
	return val;
}