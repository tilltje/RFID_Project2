#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "Initialisatie.h"
#include "RFID_sensor.h"
#include "Display.h"
#include "Sequence.h"

//Segment array display
const uint8_t segmentMap[] = {
    0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F
};

// Display
void send_byte(uint8_t data) {
    for (uint8_t i = 0; i < 8; i++) {
        CLK_LOW();
        if (data & 0x01) DIO_HIGH(); else DIO_LOW();
        _delay_us(50);
        CLK_HIGH();
        _delay_us(50);
        data >>= 1;
    }
    // Wachten op ACK (Acknowledgement)
    CLK_LOW();
    DIO_INPUT();
    _delay_us(50);
    CLK_HIGH();
    _delay_us(50);
    CLK_LOW();
    DIO_OUTPUT();
}

// Display
void tm_start() {
    DIO_OUTPUT(); DIO_HIGH(); CLK_HIGH(); _delay_us(50);
    DIO_LOW(); _delay_us(50); CLK_LOW();
}

// Display
void tm_stop() {
    CLK_LOW(); DIO_LOW(); _delay_us(50);
    CLK_HIGH(); _delay_us(50); DIO_HIGH();
}

// Print cijfers op display
void display(uint16_t teller_leeg, uint16_t teller_rfid) {
    uint8_t digits[4];
    digits[0] = teller_leeg / 10;
    digits[1] = teller_leeg % 10;
    digits[2] = teller_rfid / 10;
    digits[3] = teller_rfid % 10;

    tm_start(); send_byte(0x40); tm_stop(); // Data command (auto increment)
    tm_start(); send_byte(0xC0);            // Start adres (eerste digit)

    if (digits[0] == 0) {
        send_byte(0x00);
    }
    else {
        send_byte(segmentMap[digits[0]]);
    }
    send_byte(segmentMap[digits[1]]);
    if (digits[2] == 0) {
        send_byte(0x00);
    }
    else {
        send_byte(segmentMap[digits[2]]);
    }
    send_byte(segmentMap[digits[3]]);
    tm_stop();
}
