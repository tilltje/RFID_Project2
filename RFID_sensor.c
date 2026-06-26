/// INCLUDE ///
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "Initialisatie.h"
#include "RFID_sensor.h"
#include "Display.h"
#include "Sequence.h"

/// FUNCTIES ///

// SPI Basic Transceiver
uint8_t spi_transfer(uint8_t data) {
    SPDR = data;
    while (!(SPSR & (1 << SPIF)));
    return SPDR;
}

// Naar sensor schrijven rechts
void RFID_write_rechts(uint8_t reg, uint8_t value) {
    SSR_PORT &= ~(1 << SSR); //ss laag is slave geactiveerd
    spi_transfer((reg << 1) & 0x7E);
    spi_transfer(value);
    SSR_PORT |= (1 << SSR);
}

// Naar sensor schrijven links
void RFID_write_links(uint8_t reg, uint8_t value) {
    SSL_PORT &= ~(1 << SSL);
    spi_transfer((reg << 1) & 0x7E);
    spi_transfer(value);
    SSL_PORT |= (1 << SSL);
}

// Sensor uitlezen rechts
uint8_t RFID_read_rechts(uint8_t reg) {
    uint8_t value;
    SSR_PORT &= ~(1 << SSR);
    spi_transfer(((reg << 1) & 0x7E) | 0x80);
    value = spi_transfer(0x00);
    SSR_PORT |= (1 << SSR);
    return value;
}

// Sensor uitlezen links
uint8_t RFID_read_links(uint8_t reg) {
    uint8_t value;
    SSL_PORT &= ~(1 << SSL);
    spi_transfer(((reg << 1) & 0x7E) | 0x80);
    value = spi_transfer(0x00);
    SSL_PORT |= (1 << SSL);
    return value;
}

// Scannen voor pasje rechts
int pasje_checken_rechts() {
    RFID_write_rechts(CommandReg, PCD_IDLE);
    RFID_write_rechts(ComIrqReg, 0x7F);
    RFID_write_rechts(FIFOLevelReg, 0x80); // Flush FIFO

    RFID_write_rechts(FIFODataReg, PICC_REQIDL);
    RFID_write_rechts(CommandReg, PCD_TRANSCEIVE);
    RFID_write_rechts(BitFramingReg, 0x87); // Start communicatie

    // Wacht tot pasje gedetecteerd is of er een timeout is
    uint16_t timeout = 1000;
    while (timeout > 0) {
        uint8_t irq = RFID_read_rechts(ComIrqReg);
        if (irq & 0x30) { // Kijken of Rx and Idle IRQs hoog zijn, als dat zo is dan is er een pasje gedetecteerd
            break;
        }
        timeout--;
        _delay_us(10);
    }

    if (timeout == 0)  { // Reactie duurt te lang, dus geen pasje
        return 0;
    }

    // Error checken
    uint8_t error = RFID_read_rechts(ErrorReg);
    if (error & 0x13) {
        return 0;
    }

    return 1; //  Wel pasje
}

// Scannen voor pasje links
int pasje_checken_links() {
    RFID_write_links(CommandReg, PCD_IDLE);
    RFID_write_links(ComIrqReg, 0x7F);
    RFID_write_links(FIFOLevelReg, 0x80); // Flush FIFO

    RFID_write_links(FIFODataReg, PICC_REQIDL);
    RFID_write_links(CommandReg, PCD_TRANSCEIVE);
    RFID_write_links(BitFramingReg, 0x87); // Start communicatie

    // Wacht tot pasje gedetecteerd is of er een timeout is
    uint16_t timeout = 1000;
    while (timeout > 0) {
        uint8_t irq = RFID_read_links(ComIrqReg);
        if (irq & 0x30) { // Kijken of Rx and Idle IRQs hoog zijn, als dat zo is dan is er een pasje gedetecteerd
            break;
        }
        timeout--;
        _delay_us(10);
    }

    if (timeout == 0) { // Reactie duurt te lang, dus geen pasje
        return 0;
    }

    // Error checken
    uint8_t error = RFID_read_links(ErrorReg);
    if (error & 0x13) {
        return 0;
    }

    return 1; // Wel pasje
}

int RFID_checken(int kant){ // Rechts = 0, links = 1
    if (kant == 0) { // Pakket rechts checken
        if (pasje_checken_rechts()) { // Tag gedetecteerd
            return 1;
        }
        else { // Geen tag
            return 0;
        }
    }
    else { // Pakket links checken
        if (pasje_checken_links()) { // Tag gedetecteerd
            return 1;
        }
        else { // Geen tag
            return 0;
        }
    }
}

