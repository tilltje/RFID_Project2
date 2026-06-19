#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "Initialisatie.h"
#include "RFID_sensor.h"
#include "Display.h"
#include "Sequence.h"

int main(void)
{

    initialisatie();

    while(1) {
        if ((KNOP_PIN & KNOP )== 0) {
            sequence();
        }
    }

    return 0;
}
