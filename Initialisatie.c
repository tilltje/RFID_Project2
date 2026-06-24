#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "Initialisatie.h"
#include "RFID_sensor.h"
#include "Display.h"
#include "Sequence.h"

ISR(INT0_vect) { ///gecheckt, werkt!
    NEXT_MOD_PORT &= ~(1 << NEXT_MOD);
    PORTB &= ~(1 << PB7);
    _delay_ms(500);
    PORTB |= (1 << PB7);
}

void initialisatie_communicatie(void) {
    MODNUMMER_DDR |= (1 << MODNUMMER); // output
    NEXT_MOD_DDR |= (1 << NEXT_MOD);
    ACK_MOD_DDR |= (1 << ACK_MOD);
    LIJN_DDR &= ~(1 << LIJN); //input
    NEXT_AGV_DDR &= ~(1 << NEXT_AGV);
    ACK_AGV_DDR &= ~(1 << ACK_AGV);

    MODNUMMER_PORT &= ~(1 << MODNUMMER); //Laag zetten, want detectiemodule
    NEXT_MOD_PORT &= ~(1 << NEXT_MOD); // dit was modnummer, foutje
    ACK_MOD_PORT &= ~(1 << ACK_MOD);

    LIJN_PORT |= (1 << LIJN); // PULL-UPS ACTIVEREN
    NEXT_AGV_PORT |= (1 << NEXT_AGV);
    ACK_AGV_PORT |= (1 << ACK_AGV);

    // externe interrupt vanuit agv
    EIMSK |= (1 << INT0); // interrupt 0 (pin 21) aanzetten
    EICRA |= (1 << ISC01) | (1 << ISC00);

    TCCR3B &= ~(1 << CS32);
    TCCR3B |= (1 << CS31);
    TCCR3B &= ~(1 << CS30);
}

void initialisatie_RFID(void) {
    DDRB |= (1 << PB0); // SS-pin als output configureren voor spi
    PORTB |= (1 << PB0);


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

void initialisatie_display(void){// initialisatie display
    // Stel pinnen in als output
    CLK_DDR |= (1 << CLK);
    DIO_DDR |= (1 << DIO);

    // Initialiseer display: Commando 0x8A zet display AAN en helderheid op een gemiddeld niveau
    tm_start();
    send_byte(0x8A);
    tm_stop();
    // start met 0 op het display
    display(0,0);

}

void initialisatie_leds(void) {
    LED_BLAUW_DDR |= (1 << LED_BLAUW);
    LED_GROEN_DDR |= (1 << LED_GROEN);

    LED_BLAUW_PORT &= ~(1 << LED_BLAUW);
    LED_GROEN_PORT &= ~(1 << LED_GROEN);

    TCCR1B |= (1 << CS12);
    TCCR1B &= ~(1 << CS11);
    TCCR1B &= ~(1 << CS10);
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
