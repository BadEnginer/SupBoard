#include <ExpanderGPIO/PCF8575.h>


void PCF_GPIO_Init(){
	I2C_WritePort(0);
}

void I2C_WritePort(uint16_t data){
    HAL_I2C_Master_Transmit(&hi2c1, I2C_ADDRESS, (uint8_t*)&data, sizeof(data), I2C_TIMEOUT);
}

uint16_t I2C_ReadPort(){
    uint16_t data = 0;
    HAL_I2C_Master_Receive (&hi2c1, I2C_ADDRESS, (uint8_t*)&data, sizeof(data), I2C_TIMEOUT);
    return data;
}

#if _LCD_USE_HAL==0
void PCF_GPIO_WritePin(PCF_ALL_PIN pin, PCF_STATE value){
    uint16_t data = 0;
    data = I2C_ReadPort(); // что бы не изменять состояние других пинов читаем весь порт и меняем только один бит который хотим.
    if(value == PIN_SET)
    	SetBit(data, pin);
    else
    	ClearBit(data, pin);
    I2C_WritePort(data);
}

uint8_t PCF_GPIO_ReadPin(PCF_ALL_PIN pin){ // подробней почитать про чтнение так все сложно, сначала нужно установить пин в 1 и только потом его читать.
    uint16_t data = 0;
    data = I2C_ReadPort(); // читаем состояние портов
    return ((uint8_t)bitRead(data, pin)); // Возвращаем только состояние одного пина
}
#endif

#if _LCD_USE_HAL==1

void PCF_GPIO_WritePin(PCF_PORT port, PCF_PIN pin, PCF_STATE value){
    uint16_t data = 0;
    data = I2C_ReadPort();
    if(value == PIN_SET)
    	SetBit(data, ((8 * port) + pin));
    else
    	ClearBit(data, ((8 * port) + pin));
    I2C_WritePort(data);
}

uint8_t PCF_GPIO_ReadPin(PCF_PORT port, PCF_PIN pin){
    uint16_t data = 0;
    data = I2C_ReadPort(); // читаем состояние портов
    return ((uint8_t)bitRead(data, ((8 * port) + pin))); // Возвращаем только состояние одного пина
}

#endif
/*
 * uint8_t I2C_ReadPin(PCF_PIN pin){
    uint16_t data = 0;
    data = I2C_ReadPort(); // читаем состояние портов
    return ((uint8_t)bitRead(data, pin));
}
 * uint8_t I2C_ReadBit(PCF_PIN pin){
    uint16_t data = 0;
    HAL_I2C_Master_Receive (&hi2c1, I2C_ADDRESS, (uint8_t*)&data, sizeof(data), I2C_TIMEOUT);
    return ((uint8_t)bitRead(data, pin)); // поправка с 8 бит на 16 бит
}
uint8_t I2C_Read1Bit(PORT port ,PIN pin){
    uint16_t data = 0;
    HAL_I2C_Master_Receive (&hi2c1, I2C_ADDRESS, (uint8_t*)&data, sizeof(data), I2C_TIMEOUT);
    return ((uint8_t)bitRead(data, (8 * port) + pin)); // поправка с 8 бит на 16 бит
}

void PCF_GPIO_WritePin(PCF_PIN pin, STATE value){
    uint16_t data = 0;
    data = I2C_Read2Bit(pin);
    if(value == PIN_SET)
    	SetBit(data, pin);
    else
    	ClearBit(data, pin);
    I2C_Write2Byte(data);
}

void PCF_GPIO_WritePin(PORT port ,PIN pin, STATE value){
    uint16_t data = 0;
    data = I2C_ReadBit(port, pin);
    if(value == PIN_SET)
    	SetBit(data, (8 * port) + pin);
    else
    	ClearBit(data, (8 * port) + pin);
    I2C_Write2Byte(data);
}

uint8_t PCF_GPIO_ReadPin(PORT port ,PIN pin){
    return I2C_ReadBit(port, pin);
}

void PCF_GPIO_WritePin(PCF_PIN pin, STATE value){
    uint16_t data = 0;
    data = I2C_ReadBit(port, pin);
    if(value == PIN_SET)
    	SetBit(data, (8 * port) + pin);
    else
    	ClearBit(data, (8 * port) + pin);
    I2C_Write2Byte(data);
}

uint8_t PCF_GPIO_ReadPin(PCF_PIN pin){
    return I2C_ReadBit(port, pin);
}
*/
