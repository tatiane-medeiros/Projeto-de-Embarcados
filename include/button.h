
#include <zephyr.h>
#include <device.h>
#include <gpio.h>

#define PORT SW0_GPIO_CONTROLLER
#define BUTTON SW0_GPIO_PIN
#define INTERRUPT   GPIO_INT
#define FALL_EDGE   (GPIO_INT_EDGE | GPIO_INT_ACTIVE_LOW)
#define DEBOUNCE    GPIO_INT_DEBOUNCE
#define PULL_UP     GPIO_PUD_PULL_UP

#define SW_FLAGS    (GPIO_DIR_IN | INTERRUPT | FALL_EDGE | DEBOUNCE | PULL_UP)


// estados
static u8_t state = 0;

struct button{
    struct device *device;
    struct gpio_callback gpio_callback;
};


void button_configure(struct button *btn, gpio_callback_handler_t cb);
void button_read(struct button *btn, u32_t *state);
void changeState();
void setState(u8_t val);
