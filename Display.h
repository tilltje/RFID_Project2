#ifndef DISPLAY_H_INCLUDED
#define DISPLAY_H_INCLUDED


void send_byte(uint8_t data);
void tm_start();
void tm_stop();
void display(uint16_t teller_leeg, uint16_t teller_rfid);

#endif // DISPLAY_H_INCLUDED
