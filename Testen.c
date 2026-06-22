#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "Initialisatie.h"
#include "RFID_sensor.h"
#include "Display.h"
#include "Sequence.h"
#include "Testen.h"
/*
void test_initialisatie(void) {
    SIGNALIR_DDR &= ~(1 << SIGNALIR); // input
    //ENABLE_DDR &= ~(1 << ENABLE); // output
    //SIGNALIR_PORT |= (1 << SIGNALIR);


}*/

void testen(void) {
    //test_initialisatie();


   /* while(1) {
        if ((IR_L_PIN & IR_L)==0) {
            led_groen();
        }
        else {
            led_groen();
        }
    }*/


    int teller_leeg = 0;
    int teller_rfid = 0;
    RFID_opstarten();
    int tagrechts = 0;
    int taglinks = 0;

    while (1) {
        if (tag_detectedR()) {
            // Blink LED on detection
            PORTC ^= (1 << PC3);
            _delay_ms(100);
            //SPI_PORT &= ~(1 << LED_PIN);
            //_delay_ms(100);
            tagrechts = 1;
        }
        else {
            if (tagrechts == 1) {
                tagrechts = 0;
                _delay_ms(50);
            }
            else {
               // PORTC &= ~(1 << PC3);
                _delay_ms(100);
            }
            //_delay_ms(200); // Standard polling interval
        }
        if (tag_detectedL()) {
            // Blink LED on detection
            PORTC ^= (1 << PC3);
            _delay_ms(750);
            //SPI_PORT &= ~(1 << LED_PIN);
            //_delay_ms(100);
            taglinks = 1;
        }
        else {
            if (taglinks == 1) {
                taglinks = 0;
                _delay_ms(50);
            }
            else {
               // PORTC &= ~(1 << PC3);
                _delay_ms(100);
            }
            //_delay_ms(200); // Standard polling interval
        }
    }

}
