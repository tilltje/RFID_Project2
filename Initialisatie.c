#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "Initialisatie.h"
#include "RFID_sensor.h"
#include "Display.h"
#include "Sequence.h"

// Initialisatie communicatiepinnen
void initialisatie_communicatie(void) {
    // Output
    MODNUMMER_DDR |= (1 << MODNUMMER);
    NEXT_MOD_DDR |= (1 << NEXT_MOD);
    ACK_MOD_DDR |= (1 << ACK_MOD);

    // Input
    LIJN_DDR &= ~(1 << LIJN);
    NEXT_AGV_DDR &= ~(1 << NEXT_AGV);
    ACK_AGV_DDR &= ~(1 << ACK_AGV);

    // Laag zetten
    MODNUMMER_PORT &= ~(1 << MODNUMMER); //Detectiemodule = 0, dus laag
    NEXT_MOD_PORT &= ~(1 << NEXT_MOD);
    ACK_MOD_PORT &= ~(1 << ACK_MOD);

    // Pull-up weerstanden activeren
    LIJN_PORT |= (1 << LIJN);
    NEXT_AGV_PORT |= (1 << NEXT_AGV);
    ACK_AGV_PORT |= (1 << ACK_AGV);
}

// Initialisatie RFID pinnen
void initialisatie_RFID(void) {
    DDRB |= (1 << PB0); // SS-pin als output configureren voor spi
    PORTB |= (1 << PB0);


    SSR_DDR |= (1 << SSR); // SS rechts output
    SSL_DDR |= (1 << SSL); // SS links output
    SCK_DDR |= (1 << SCK); // SCK output
    MOSI_DDR |= (1 << MOSI); // MOSI output
    MISO_DDR &= ~(1 << MISO); // MISO input
    RSTR_DDR |= (1 << RSTR); // RST rechts output
    RSTL_DDR |= (1 << RSTL); // RST links output

    SSR_PORT |= (1 << SSR); // Slave rechts niet selecteren
    SSL_PORT |= (1 << SSL); // Slave links niet selecteren
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

// RFID sensor klaarmaken voor verzending
void RFID_aanzetten() {
    // Harde reset
    RSTR_PORT |= (1 << RSTR);
    _delay_ms(50);
    RSTR_PORT &= ~(1 << RSTR);
    _delay_ms(50);
    RSTR_PORT |= (1 << RSTR);
    _delay_ms(50);

    RSTL_PORT |= (1 << RSTL);
    _delay_ms(50);
    RSTL_PORT &= ~(1 << RSTL);
    _delay_ms(50);
    RSTL_PORT |= (1 << RSTL);
    _delay_ms(50);

    // Interne transceivers aanzetten
    RFID_write_rechts(ModeReg, 0x3D);
    RFID_write_rechts(TxASKReg, 0x40);

    RFID_write_links(ModeReg, 0x3D);
    RFID_write_links(TxASKReg, 0x40);

    // Antenne aanzetten
    uint8_t currentR = RFID_read_rechts(TxControlReg);
    if ((currentR & 0x03) != 0x03) {
        RFID_write_rechts(TxControlReg, currentR | 0x03);
    }

    uint8_t currentL = RFID_read_links(TxControlReg);
    if ((currentL & 0x03) != 0x03) {
        RFID_write_links(TxControlReg, currentL | 0x03);
    }
}

void initialisatie_IRsensor(void){ // Initialisatie infraroodsensoren
    // Input
    IR_R_DDR &= ~(1 << IR_R);
    IR_L_DDR &= ~(1 << IR_L);

    // Pull-up weerstanden activeren
    IR_R_PORT |= (1 << IR_R);
    IR_L_PORT |= (1 << IR_L);
}

void initialisatie_display(void){// Initialisatie display
    // Output
    CLK_DDR |= (1 << CLK);
    DIO_DDR |= (1 << DIO);

    // Initialiseer display
    display_verzending_aan();
    send_byte(0x8A);
    display_verzending_uit();

    // Zet 0 en 0 op het display
    display(0,0);

}

void initialisatie_leds(void) { // Leds initialiseren
    // Output
    LED_BLAUW_DDR |= (1 << LED_BLAUW);
    LED_GROEN_DDR |= (1 << LED_GROEN);

    // LEDs uitzetten
    LED_BLAUW_PORT &= ~(1 << LED_BLAUW);
    LED_GROEN_PORT &= ~(1 << LED_GROEN);

    // Timer voor knipperen leds initialiseren
    TCCR1B |= (1 << CS12);
    TCCR1B &= ~(1 << CS11);
    TCCR1B &= ~(1 << CS10);
}

void initialisatie_knop(void) { // Knop initialiseren
    KNOP_DDR &= ~(1 << KNOP); //INPUT

    KNOP_PORT |= (1 << KNOP); //PULL-UP
}

void initialisatie(void) { // Initialisatie
    initialisatie_communicatie();
    initialisatie_RFID();
    RFID_aanzetten();
    initialisatie_IRsensor();
    initialisatie_display();
    initialisatie_leds();
    initialisatie_knop();
    sei();
}
