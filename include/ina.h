#include <zephyr.h>
#include <stdio.h>
#include <device.h>
#include <i2c.h>

#include <device.h>
#include <i2c.h>
#include <gpio.h>
#include <misc/__assert.h>
#include <string.h>


// endereço do INA219
#define I2C_SLV_ADDR	0x40
// calibração
#define CAL_VAL		(4096)


/* With default calibration above,
 * Each current LSB is 100 uA == 0.1 mA == 0.0001 A.
 * Each power LSB is 2000 uW == 2 mW = 0.002W.
 */
#define CUR_LSB		100
#define PWR_LSB		2000

/* Sleep time */
#define SLEEP_TIME	500



int read_reg16(struct device *i2c_dev, u8_t reg_addr, u8_t *data);

int write_reg16(struct device *i2c_dev, u8_t reg_addr, u8_t *data);

void ina(void);

