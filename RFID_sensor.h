#ifndef RFID_SENSOR_H_INCLUDED
#define RFID_SENSOR_H_INCLUDED

uint8_t spi_transfer(uint8_t data);
void RFID_write_rechts(uint8_t reg, uint8_t value);
void RFID_write_links(uint8_t reg, uint8_t value);
uint8_t RFID_read_rechts(uint8_t reg);
uint8_t RFID_read_links(uint8_t reg);
int pasje_checken_rechts(void);
int pasje_checken_links(void);
int RFID_checken(int kant);

#endif // RFID_SENSOR_H_INCLUDED
