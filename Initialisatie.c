#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "Initialisatie.h"
#include "RFID_sensor.h"
#include "Display.h"
#include "Sequence.h"

void initialisatie_communicatie(void) {
    MODNUMMER_DDR |= (1 << MODNUMMER); // output
    NEXT_MOD_DDR |= (1 << NEXT_MOD);
    ACK_MOD_DDR |= (1 << ACK_MOD);
    LIJN_DDR &= ~(1 << LIJN); //input
    NEXT_AGV_DDR &= ~(1 << NEXT_AGV);
    ACK_AGV_DDR &= ~(1 << ACK_AGV);

    MODNUMMER_PORT &= ~(1 << MODNUMMER); //Laag zetten, want detectiemodule
    NEXT_MOD_PORT &= ~(1 << MODNUMMER);
    ACK_MOD_PORT &= ~(1 << ACK_MOD);

    LIJN_PORT |= (1 << LIJN); // PULL-UPS ACTIVEREN
    NEXT_AGV_PORT |= (1 << NEXT_AGV);
    ACK_AGV_PORT |= (1 << ACK_AGV);
}

void initialisatie_RFID(void) {
    SSR_DDR |= (1 << SSR);//SS output
    SSL_DDR |= (1 << SSL);
    SCK_DDR |= (1 << SCK);//SCK output
    MOSI_DDR |= (1 << MOSI);//MOSI output
    MISO_DDR &= ~(1 << MISO);//MISO input
    RSTR_DDR |= (1 << RSTR);//RST output
    RSTL_DDR |= (1 << RSTL);

    SSR_PORT |= (1 << SSR); // slave niet selecten
    SSL_PORT |= (1 << SSL);
    RSTR_PORT |= (1 << RSTR);
    RSTL_PORT |= (1 << RSTL);

    SPCR |= (1 << SPE);
    SPCR &= ~(1 << DORD);
    SPCR |= (1 << MSTR);
    SPCR &= ~(1 << CPOL);
    SPCR &= ~(1 << CPHA);
    SPCR &= ~(1 << SPR1);
    SPCR &= ~(1 << SPR0);

    SPSR &= ~(1 << SPI2X);
}

void initialisatie_IRsensor(void){
    IR_R_DDR &= ~(1 << IR_R); //input
    IR_L_DDR &= ~(1 << IR_L); //input

    IR_R_PORT |= (1 << IR_R);
    IR_L_PORT |= (1 << IR_L);
}


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

void initialisatie_display(void){// initialisatie display
    // Stel pinnen in als output
    CLK_DDR |= (1 << CLK);
    DIO_DDR |= (1 << DIO);

    // Initialiseer display: Commando 0x8A zet display AAN en helderheid op een gemiddeld niveau
    tm_start();
    send_byte(0x8A);
    tm_stop();
    // start met 0 op het display
    //tm1637_showNumber(0);

}

void initialisatie_leds(void) {
    LED_BLAUW_DDR |= (1 << LED_BLAUW);
    LED_GROEN_DDR |= (1 << LED_GROEN);

    LED_BLAUW_PORT &= ~(1 << LED_BLAUW);
    LED_GROEN_PORT &= ~(1 << LED_GROEN);
}

void initialisatie_knop(void) {
    KNOP_DDR &= ~(1 << KNOP); //INPUT

    KNOP_PORT |= (1 << KNOP); //PULL-UP
}

void initialisatie(void) { //pinnen initialiseren
    initialisatie_communicatie();
    initialisatie_RFID();
    initialisatie_IRsensor();
    initialisatie_display();
    initialisatie_leds();
    initialisatie_knop();
    sei();
}
