/// INCLUDE ///
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "Initialisatie.h"
#include "RFID_sensor.h"
#include "Display.h"
#include "Sequence.h"

/// FUNCTIES ///

// Signaleren naar AGV dat hij moet stoppen
void stopsignaal_hoog(void) {
    ACK_MOD_PORT |= (1 << ACK_MOD);
}

// Stopsignaal naar AGV weer laag zetten
void stopsignaal_laag(void) {
    _delay_ms(500);
    ACK_MOD_PORT &= ~(1 << ACK_MOD);
}

// Blauwe led knipperen omdat pakket leeg is
void led_blauw(void) {
    TCNT1 = 0;
    TIFR1 = (1 << TOV1); // Flag verwijderen
    int klaar = 0;
    LED_BLAUW_PORT |= (1 << LED_BLAUW); // Led aan
    while (klaar == 0) { // Wacht tot led weer uit moet
        if ((TIFR1 & (1 << TOV1)) != 0) {
            LED_BLAUW_PORT &= ~(1 << LED_BLAUW); // Led uit
            klaar = 1;
        }
    }
}

// Groene led knipperen omdat pakket RFID pasje bevat
void led_groen(void) {
    TCNT1 = 0;
    TIFR1 = (1 << TOV1); // Flag verwijderen
    int klaar = 0;
    LED_GROEN_PORT |= (1 << LED_GROEN); // Led aan
    while (klaar == 0) { // Wacht tot led weer uit moet
        if ((TIFR1 & (1 << TOV1)) != 0) {
            LED_GROEN_PORT &= ~(1 << LED_GROEN); // Led uit
            klaar = 1;
        }
    }
}

// Loop die pakketten detecteert
void detectie(void) {
    NEXT_MOD_PORT |= (1 << NEXT_MOD); // Signaleren naar AGV dat hij moet gaan rijden
    int teller_leeg = 0;
    int teller_rfid = 0;
    int rechts = 0; // Zorgt dat pakketten niet dubbel geteld worden
    int links = 0; // Zorgt dat pekketten niet dubbel geteld worden

    while ((NEXT_AGV_PIN & (1 << NEXT_AGV)) == 0) { // Detecteren zolang AGV niet heeft gesignaleerd dat er gestopt moet worden
        if (((IR_R_PIN & (1 << IR_R)) == 0) && (rechts == 0)) { // Rechts pakket gedetecteerd
            stopsignaal_hoog(); // Naar AGV signaleren dat die moet stoppen met rijden
            rechts = 1;
            if (RFID_checken(0) != 0) { // Pakket bevat pasje
                teller_rfid++;
                display(teller_leeg, teller_rfid);
                led_groen();
                _delay_ms(50); // Debounce
            }
            else { // Pakket is leeg
                teller_leeg++;
                display(teller_leeg, teller_rfid);
                led_blauw();
                _delay_ms(50); // Debounce
            }
            stopsignaal_laag(); // Stopsignaal weer laag zetten
        }
        if (((IR_L_PIN & (1 << IR_L)) == 0)&& (links == 0)) { // Links pakket gedetecteerd
            stopsignaal_hoog(); // Naar AGV signaleren dat die moet stoppen met rijden
            links = 1;
            if (RFID_checken(1) != 0) { // Pakket bevat pasje
                teller_rfid++;
                display(teller_leeg, teller_rfid);
                led_groen();
                _delay_ms(50);
            }
            else { // Pakket is leeg
                teller_leeg++;
                display(teller_leeg, teller_rfid);
                led_blauw();
                _delay_ms(50);
            }
            stopsignaal_laag(); // Stopsignaal weer laag zetten
        }
        if (((IR_R_PIN & (1 << IR_R)) != 0) && (rechts == 1)) { // Rechts pakket niet meer gedetecteerd
            rechts = 0;
            _delay_ms(50); // Debouncen
        }
        if (((IR_L_PIN & (1 << IR_L)) != 0) && (links == 1)) { // Links pakket niet meer gedetecteerd
            links = 0;
            _delay_ms(50); // Debouncen
        }
    }
}
