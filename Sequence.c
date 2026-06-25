#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "Initialisatie.h"
#include "RFID_sensor.h"
#include "Display.h"
#include "Sequence.h"

/*
void acknowledge_module(void) {
    ACK_MOD_PORT |= (1 << ACK_MOD);
    TCNT3 = 0;
    TIFR3 = (1 << TOV3);
    TIMSK3 |= (1 << TOIE3);
}

ISR(TIMER3_OVF_vect) { // acknowledge module
    ACK_MOD_PORT &= ~(1 << ACK_MOD);
    TIMSK3 &= ~(1 << TOIE3); // interrupt weer uitzetten
}*/

/*
ISR(INT0_vect) { //AGV heeft commando geacknowledged
    NEXT_MOD_PORT &= ~(1 << NEXT_MOD);
}*/
/*
void rijden(void) {
    NEXT_MOD_PORT |= (1 << NEXT_MOD); // Volgende naar agv signaleren
}*/

void stoppen(void) {
    ACK_MOD_PORT |= (1 << ACK_MOD);
    //_delay_ms(2000);
    //ACK_MOD_PORT &= ~(1 << ACK_MOD);
}

void starten(void) {
    _delay_ms(500);
    ACK_MOD_PORT &= ~(1 << ACK_MOD);
}

void led_blauw(void) { //leeg
    TCNT1 = 0;
    TIFR1 = (1 << TOV1); // flag verwijderen
    int klaar = 0;
    LED_BLAUW_PORT |= (1 << LED_BLAUW); // led aan
    while (klaar == 0) {
        if ((TIFR1 & (1 << TOV1)) != 0) {
            LED_BLAUW_PORT &= ~(1 << LED_BLAUW); // led uit
            klaar = 1;
        }
    }
}

void led_groen(void) { // rfid
    TCNT1 = 0;
    TIFR1 = (1 << TOV1); // flag verwijderen
    int klaar = 0;
    LED_GROEN_PORT |= (1 << LED_GROEN); // led aan
    while (klaar == 0) {
        if ((TIFR1 & (1 << TOV1)) != 0) {
            LED_GROEN_PORT &= ~(1 << LED_GROEN); // led uit
            klaar = 1;
        }
    }
}

void sequence(void) {
    RFID_opstarten();
    NEXT_MOD_PORT |= (1 << NEXT_MOD); // agv moet gaan rijden
    int teller_leeg = 0;
    int teller_rfid = 0;
    int rechts = 0;
    int links = 0;
    int testen = 0;
    //rijden();
    //LED_GROEN_PORT ^= (1 << LED_GROEN);

    while ((NEXT_AGV_PIN & (1 << NEXT_AGV)) == 0) { //((NEXT_AGV_PIN & (1 << NEXT_AGV)) == 0) { //detectiemodus
        //_delay_ms(500);
        if (((IR_R_PIN & (1 << IR_R)) == 0) && (rechts == 0)) { // rechts doosje gedetecteerd
            stoppen();
            rechts = 1;
            if (RFID_scannen(0) != 0) {// doosje bevat tag
                teller_rfid++;
                display(teller_leeg, teller_rfid);
                //display(links, rechts);
                led_groen();
                _delay_ms(50);
            }
            else { // doosje is leeg
                teller_leeg++;
                display(teller_leeg, teller_rfid);
                //display(links, rechts);
                led_blauw();
                _delay_ms(50);
            }
            starten();
        }
        if (((IR_L_PIN & (1 << IR_L)) == 0)&& (links == 0)) { // links doosje gedetecteerd
            stoppen();
            links = 1;
            if (RFID_scannen(1) != 0) { //doosje bevat tag
                teller_rfid++;
                display(teller_leeg, teller_rfid);
                //display(links, rechts);
                led_groen();
                _delay_ms(50);
            }
            else { // doosje is leeg
                teller_leeg++;
                display(teller_leeg, teller_rfid);
                //display(links, rechts);
                led_blauw();
                _delay_ms(50);
            }
            starten();
        }
        if (((IR_R_PIN & (1 << IR_R)) != 0) && (rechts == 1)) { // rechts doosje niet meer gedetecteerd
            rechts = 0;
           // display(links, rechts);
            _delay_ms(50); // debouncen
        }
        if (((IR_L_PIN & (1 << IR_L)) != 0) && (links == 1)) { // links doosje niet meer gedetecteerd
            links = 0;
            //display(links, rechts);
            _delay_ms(50); // debouncen
        }
        //display(links,rechts);

    }
    //acknowledge_module();

}
