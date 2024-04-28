#ifndef INC_MACROS_BITS_H_
#define INC_MACROS_BITS_H_

#define bitRead(value, bit) ((value) >> (bit)) & 0x01
#define   SetBit(reg, bit)     reg |= (1<<bit)
#define   ClearBit(reg, bit)   reg &= (~(1<<bit))
#define   InvBit(reg, bit)     reg ^= (1<<bit)
#define   BitIsSet(reg, bit)   ((reg & (1<<bit)) != 0)
#define   BitIsClear(reg, bit) ((reg & (1<<bit)) == 0)

#endif /* INC_MACROS_BITS_H_ */
