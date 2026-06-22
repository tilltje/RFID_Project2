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
    int rechts = 0;
    int links = 0;
    RFID_opstarten();
    int rechtsdetectie = 0;
    int linksdetectie = 1;




    while(1) {
        /*PORTC ^= (1 << PC3);
        _delay_ms(500);
        PORTC ^= (1 << PC3);
        _delay_ms(500);
        PORTC ^= (1 << PC3);
        _delay_ms(500);
        PORTC ^= (1 << PC3);
        _delay_ms(500);*/

        rechtsdetectie = RFID_scannen(0);
        linksdetectie =  RFID_scannen(1);

        if ((rechtsdetectie != 0) && (rechts == 0)) {
            //teller_leeg++;
            //display(teller_leeg, teller_rfid);
            LED_GROEN_PORT &= ~(1 << LED_GROEN);
            rechts = 1;
        }
        if ((linksdetectie != 0) && (links == 0)) {
            //teller_rfid++;
            //display(teller_leeg, teller_rfid);
            LED_GROEN_PORT &= ~(1 << LED_GROEN);
            links = 1;
        }
        if ((rechtsdetectie == 0) && (rechts == 1)) {
            rechts = 0;
            LED_GROEN_PORT |= (1 << LED_GROEN);
        }
        if ((linksdetectie == 0) && (links == 1)) {
            links = 0;
            LED_GROEN_PORT |= (1 << LED_GROEN);
        }

    }
}
