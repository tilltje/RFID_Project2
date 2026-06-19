#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "Initialisatie.h"
#include "RFID_sensor.h"
#include "Display.h"
#include "Sequence.h"

ISR(INT0_vect) { //AGV heeft commando geacknowledged
    NEXT_MOD_PORT &= ~(1 << NEXT_MOD);
}



void rijden(void) {
    NEXT_MOD_PORT |= (1 << NEXT_MOD); // Volgende naar agv signaleren
}

void stoppen(void) {
    NEXT_MOD_PORT |= (1 << NEXT_MOD);
}

void sequence(void) {
    RFID_opstarten();
    rijden();
    while ((NEXT_AGV_PIN & NEXT_AGV) == 0) { //detectiemodus
        if ( ((IR_L_PIN & IR_L) == 0) || ((IR_R_PIN & IR_R) == 0)) { //doosje gedetecteerd
            stoppen();
            // if RFID detecteert ding
        }
    }

}
