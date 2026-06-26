/*
Code voor de detectiemodule

Timer 1 wordt gebruikt voor het knipperen van de leds

De code voor de RFID-sensors is gebaseerd op code gemaakt door de AI-modus van Google Zoeken. De gebruikte prompt is
"I want to use the RC522 on an Arduino Mega 2560, without using Arduino libraries.
I want a LED to blink whenever the RC522 detects a tag."
Dit is vervolgens zelf uitgebreid naar 2 sensoren.
Ook is het knipperen van de ledjes zelf herschreven, zodat er gebruik wordt gemaakt van een timer.

*/

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
        if ((KNOP_PIN & (1 << KNOP))== 0) { // Wachten tot de knop is ingedrukt
            detectie(); // Start programma
        }
    }

    return 0;
}
