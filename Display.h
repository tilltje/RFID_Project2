#ifndef DISPLAY_H_INCLUDED
#define DISPLAY_H_INCLUDED


void send_byte(uint8_t data);
void display_verzending_aan(void);
void display_verzending_uit(void);
void display(uint16_t teller_leeg, uint16_t teller_rfid);

#endif // DISPLAY_H_INCLUDED
