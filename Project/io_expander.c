#include "io_expander.h"
// Skeleton code from eeprom.c
//*****************************************************************************
// Initialize the I2C peripheral
//*****************************************************************************
bool io_expander_init(void) {
  if(gpio_enable_port(IO_EXPANDER_GPIO_BASE) == false) {
    return false;
  }
  // Configure SCL 
  if(gpio_config_digital_enable(IO_EXPANDER_GPIO_BASE, IO_EXPANDER_I2C_SCL_PIN) == false) {
    return false;
  }
  if(gpio_config_alternate_function(IO_EXPANDER_GPIO_BASE, IO_EXPANDER_I2C_SCL_PIN) == false){
    return false;
  }
  if(gpio_config_port_control(IO_EXPANDER_GPIO_BASE, IO_EXPANDER_I2C_SCL_PCTL_M, IO_EXPANDER_I2C_SCL_PIN_PCTL) == false) {
    return false;
  }
  
  // Configure SDA 
  if(gpio_config_digital_enable(IO_EXPANDER_GPIO_BASE, IO_EXPANDER_I2C_SDA_PIN) == false) {
    return false;
  }
  if(gpio_config_open_drain(IO_EXPANDER_GPIO_BASE, IO_EXPANDER_I2C_SDA_PIN) == false) {
    return false;
  }
  if(gpio_config_alternate_function(IO_EXPANDER_GPIO_BASE, IO_EXPANDER_I2C_SDA_PIN) == false) {
    return false;
  }
  if(gpio_config_port_control(IO_EXPANDER_GPIO_BASE, IO_EXPANDER_I2C_SDA_PCTL_M, IO_EXPANDER_I2C_SDA_PIN_PCTL) == false) {
    return false;
  }
  
  //  Initialize I2C
  if( initializeI2CMaster(IO_EXPANDER_I2C_BASE) != I2C_OK)
   return false;
  
  return true;
}

void io_expander_write_reg(uint8_t reg, uint8_t data) {
  
  // Before doing anything, make sure the I2C device is idle
  while(I2CMasterBusy(IO_EXPANDER_I2C_BASE));
	
	i2cSetSlaveAddr(IO_EXPANDER_I2C_BASE, MCP23017_DEV_ID, I2C_WRITE);
	i2cSendByte(IO_EXPANDER_I2C_BASE, reg, I2C_MCS_START | I2C_MCS_RUN);
	i2cSendByte(IO_EXPANDER_I2C_BASE, data, I2C_MCS_RUN | I2C_MCS_STOP);
}

uint8_t io_expander_read_reg(uint8_t reg) {
  uint8_t data; 
  // Before doing anything, make sure the I2C device is idle
  while(I2CMasterBusy(IO_EXPANDER_I2C_BASE));
		
  i2cSetSlaveAddr(IO_EXPANDER_I2C_BASE, MCP23017_DEV_ID, I2C_WRITE);
  i2cSendByte(IO_EXPANDER_I2C_BASE, reg, I2C_MCS_START | I2C_MCS_RUN);
  i2cSetSlaveAddr(IO_EXPANDER_I2C_BASE, MCP23017_DEV_ID, I2C_READ);
  i2cGetByte(IO_EXPANDER_I2C_BASE, &data, I2C_MCS_START | I2C_MCS_STOP | I2C_MCS_RUN);
	
  return data;
}
