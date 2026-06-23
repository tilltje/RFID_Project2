/*
Timer 1 wordt gebruikt voor het knipperen van de leds
Timer 3 wordt gebruikt voor acknowledge van module
*/

///EXTERNE INTERRUPT IN INITIALISATIE TERUG AANPASSEN

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "Initialisatie.h"
#include "RFID_sensor.h"
#include "Display.h"
#include "Sequence.h"
#include "Testen.h"

int main(void)
{

    initialisatie();
    //IRblauwtesten();
    //IRsensortesten();
    //testen();
    //init_test();
    /*
    while(1){
        //test_module_versturen();
        test_AGV_ontvang();
    }

    while(1);*/


    while(1) {
        if ((KNOP_PIN & KNOP )== 0) {
            sequence();
        }
    }

    return 0;
}
