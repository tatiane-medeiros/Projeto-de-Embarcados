#include "ina.h"

struct device *ina_dev;

u8_t data[2];
u32_t shunt_volt;
u32_t bus_volt;
u32_t current;
u32_t power;

int read_reg16(struct device *i2c_dev, u8_t reg_addr,
	       u8_t *data)
{
	u8_t wr_addr;
	struct i2c_msg msgs[2];

	/* Register address */
	wr_addr = reg_addr;

	/* Setup I2C messages */

	/* Send the address to read */
	msgs[0].buf = &wr_addr;
	msgs[0].len = 1U;
	msgs[0].flags = I2C_MSG_WRITE;

	/* Read from device. RESTART as neededm and STOP after this. */
	msgs[1].buf = data;
	msgs[1].len = 2U;
	msgs[1].flags = I2C_MSG_READ | I2C_MSG_RESTART | I2C_MSG_STOP;

	return i2c_transfer(i2c_dev, &msgs[0], 2, I2C_SLV_ADDR);
}

int write_reg16(struct device *i2c_dev, u8_t reg_addr,
		u8_t *data)
{
	u8_t wr_addr;
	struct i2c_msg msgs[2];

	/* Register address */
	wr_addr = reg_addr;

	/* Setup I2C messages */

	/* Send the address to read */
	msgs[0].buf = &wr_addr;
	msgs[0].len = 1U;
	msgs[0].flags = I2C_MSG_WRITE;

	/* Read from device. RESTART as neededm and STOP after this. */
	msgs[1].buf = data;
	msgs[1].len = 2U;
	msgs[1].flags = I2C_MSG_WRITE | I2C_MSG_STOP;

	return i2c_transfer(i2c_dev, &msgs[0], 2, I2C_SLV_ADDR);
}


void ina_calibration(void){
	ina_dev = device_get_binding("I2C_0");
	if (!ina_dev) {
		printk("I2C: Device not found.\n");
		return;
	}

	/* Configure the chip using default values */
	data[0] = 0x03;
	data[1] = 0x99;
	write_reg16(ina_dev, 0x00, data);

	/* Write the calibration value */
	data[0] = (CAL_VAL & 0xFF00) >> 8;
	data[1] = CAL_VAL & 0xFF;
	write_reg16(ina_dev, 0x05, data);
	
}


u32_t ina(u8_t st)
{
		

	u32_t aux;
	
		/* Read bus voltage */
	read_reg16(ina_dev, 0x02, data);
	bus_volt = (data[0] << 8) | data[1];
	bus_volt >>= 3; /* 3 LSBs are not data */
	bus_volt *= 4U; /* each LSB is 4 mV */
	//printk("Bus Voltage: %d mV\n", bus_volt);

	/* Read shunt voltage */
	read_reg16(ina_dev, 0x01, data);
	shunt_volt = (data[0] << 8) | data[1];
	shunt_volt *= 10U; /* to uV since each LSB is 0.01 mV */
	printk("Voltage: %d uV\n", shunt_volt);
	
	aux = shunt_volt - bus_volt/100;
	
	/* current by shunt */
	current = aux/100;
	printk("Current: %d mA\n", current);
	
	/* power */
	power = current * (aux/1000);
	printk("Power: %d uW\n", power);
	
	if(st == 0){
		return current;
	}
	else if(st == 1){
		return aux;
	}
	else return power;
	
}