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

void init_test(void){

    DDRB |= (1 << PB7);
    DDRB |= (1 << PB6);

    PORTB |= (1 << PB7); // uit
    PORTB |= (1 << PB6); // uit

//    DDRB |= (1 << PB5);
//    DDRB |= (1 << PB4);

    DDRF &= ~(1 << PF1);
    DDRF &= ~(1 << PF2);
    DDRF &= ~(1 << PF3);
}

void test_module_versturen(void){ //module
    int released = 1;
    if (((PINF & (1 << PF1)) == 0) && (released == 1)) {
        NEXT_MOD_PORT |= (1 << NEXT_MOD);
        released = 0;
        ////_delay_ms(1000);
        //NEXT_MOD_PORT &= ~(1 << NEXT_MOD);
    }
}


void test_AGV_ontvang(void){ //agv
    static int test = 1;
    if ((NEXT_AGV_PIN & (1 << NEXT_AGV)) != 0 && test == 1){ //input vanuit module
        //ACK_AGV_PORT &= ~(1 << ACK_AGV);
        PORTB &= ~(1 << PB7);
        _delay_ms(1000);
        //PORTA |= (1 << PA4);

        acknowledge_module();

        //ACK_AGV_PORT |= (1 << ACK_AGV);
        test = 0;
    }
    else if((NEXT_AGV_PIN & (1 << NEXT_AGV)) == 0 && test == 0){
        PORTB |= (1 << PB7);
        test = 1;
    }
}









void IRblauwtesten (void) {
    SIGNALIR_DDR &= ~(1 << SIGNALIR);
    while(1) {
        if ((SIGNALIR_PIN & (1 << SIGNALIR))==0) {
            PORTC |= (1 << PC3);
        }
        else {
            PORTC &= ~(1 << PC3);
        }
    }
}

void IRsensortesten (void) {
    int teller = 0;
    SIGNALIR_DDR &= ~(1 << SIGNALIR);
    ENABLEIR_DDR |= (1 << ENABLEIR);
    ENABLEIR_PORT &= ~(1 << ENABLEIR);

    while (1) {
        ENABLEIR_PORT |= (1 << ENABLEIR);
        _delay_us(210);
        if ((SIGNALIR_PIN & (1 << SIGNALIR)) != 0) {
            PORTC &= ~(1 << PC3);
        }
        else {
            _delay_us(395);
            if ((SIGNALIR_PIN & (1 << SIGNALIR)) != 0) {
                PORTC &= ~(1 << PC3);
            }
            else {
                teller++;
                PORTC |= (1 << PC3);
            }
        }
        ENABLEIR_PORT &= ~(1 << ENABLEIR);
        _delay_us(300);
        display(teller, teller);


/*
        if ((SIGNALIR_PIN & (1 << SIGNALIR)) == 0) {
            PORTC |= (1 << PC3);
        }
        else {
            PORTC &= ~(1 << PC3);
        }*/
    }

}

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
    //int tagrechts = 0;
    //int taglinks = 0;
    int rechts = 0;
    int links = 0;


    while (1) { //detectiemodus
        if (((IR_R_PIN & (1 << IR_R)) == 0) && (rechts == 0)) { // rechts doosje gedetecteerd
            stoppen();
            rechts = 1;
            if (RFID_scannen(0) != 0) {// doosje bevat tag
                teller_rfid++;
                display(teller_leeg, teller_rfid);
                led_groen();
            }
            else { // doosje is leeg
                teller_leeg++;
                display(teller_leeg, teller_rfid);
                led_blauw();
            }
        }
        if (((IR_L_PIN & (1 << IR_L)) == 0)&& (links == 0)) { // links doosje gedetecteerd
            stoppen();
            links = 1;
            if (RFID_scannen(1) != 0) { //doosje bevat tag
                teller_rfid++;
                display(teller_leeg, teller_rfid);
                led_groen();
            }
            else { // doosje is leeg
                teller_leeg++;
                display(teller_leeg, teller_rfid);
                led_blauw();
            }
        }
        if (((IR_R_PIN & (1 << IR_R)) != 0) && (rechts == 1)) { // rechts doosje niet meer gedetecteerd
            rechts = 0;
            _delay_ms(50); // debouncen
        }
        if (((IR_L_PIN & (1 << IR_L)) != 0) && (links == 1)) { // links doosje niet meer gedetecteerd
            links = 0;
            _delay_ms(50); // debouncen
        }

    }

    /*
    while (1) {
        if (tag_detectedR()) {
            // Blink LED on detection
            teller_leeg++;
            display(teller_leeg, teller_rfid);
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
            teller_rfid++;
            display(teller_leeg, teller_rfid);
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
    */

}
