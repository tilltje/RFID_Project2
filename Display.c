/// INCLUDE ///
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "Initialisatie.h"
#include "RFID_sensor.h"
#include "Display.h"
#include "Sequence.h"

/// FUNCTIES ///

// Bitjes voor elk cijfer
const uint8_t segmentMap[] = {
    0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F
};

// Getal of commando naar display sturen
void send_byte(uint8_t data) {
    for (uint8_t i = 0; i < 8; i++) { // Geef bitjes een voor een door
        CLK_LOW();
        if (data & 0x01) {
            DIO_HIGH();
        }
        else {
            DIO_LOW();
        }
        _delay_us(50);
        CLK_HIGH();
        _delay_us(50);
        data >>= 1;
    }
    // Wachten op acknowledge
    CLK_LOW();
    DIO_INPUT();
    _delay_us(50);
    CLK_HIGH();
    _delay_us(50);
    CLK_LOW();
    DIO_OUTPUT();
}

// Display klaarmaken om te ontvangen
void display_verzending_aan(void) {
    DIO_OUTPUT();
    DIO_HIGH();
    CLK_HIGH();
    _delay_us(50);
    DIO_LOW();
    _delay_us(50);
    CLK_LOW();
}

// Ontvangen weer uitzetten
void display_verzending_uit(void) {
    CLK_LOW();
    DIO_LOW();
    _delay_us(50);
    CLK_HIGH();
    _delay_us(50);
    DIO_HIGH();
}

// Print cijfers op display. De eerste twee cijfers geven het aantal lege pakketten aan.
// De laatste twee cijfers geven het aantal pakketten met pasje aan.
void display(uint16_t teller_leeg, uint16_t teller_rfid) {
    uint8_t digits[4];
    digits[0] = teller_leeg / 10;
    digits[1] = teller_leeg % 10;
    digits[2] = teller_rfid / 10;
    digits[3] = teller_rfid % 10;

    display_verzending_aan();
    send_byte(0x40);
    display_verzending_uit(); // Data command (auto increment)

    display_verzending_aan();
    send_byte(0xC0);            // Start adres (eerste digit)

    if (digits[0] == 0) { // Als linker cijfer 0 is dat cijfer niet laten zien
        send_byte(0x00);
    }
    else { // Print eerste cijfer
        send_byte(segmentMap[digits[0]]);
    }
    send_byte(segmentMap[digits[1]]); // Print tweede cijfer
    if (digits[2] == 0) { // Als linker cijfer 0 is dat cijfer niet laten zien
        send_byte(0x00);
    }
    else { // Print derde cijfer
        send_byte(segmentMap[digits[2]]);
    }
    send_byte(segmentMap[digits[3]]); // Print vierde cijfer
    display_verzending_uit();
}
