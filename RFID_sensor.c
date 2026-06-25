#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "Initialisatie.h"
#include "RFID_sensor.h"
#include "Display.h"
#include "Sequence.h"

// SPI Basic Transceiver
uint8_t spi_transfer(uint8_t data) {
    SPDR = data;
    while (!(SPSR & (1 << SPIF))); /// HIER ZIT EEN FOUT///
    return SPDR;
}

// Write to RC522 Register rechts
void write_reg_R(uint8_t reg, uint8_t value) {
    SSR_PORT &= ~(1 << SSR); //ss laag is slave geactiveerd
    spi_transfer((reg << 1) & 0x7E);
    spi_transfer(value);
    SSR_PORT |= (1 << SSR);
}

// Read from RC522 Register rechts
uint8_t read_reg_R(uint8_t reg) {
    uint8_t value;
    SSR_PORT &= ~(1 << SSR);
    spi_transfer(((reg << 1) & 0x7E) | 0x80);
    value = spi_transfer(0x00);
    SSR_PORT |= (1 << SSR);
    return value;
}

// Write to RC522 Register links
void write_reg_L(uint8_t reg, uint8_t value) {
    SSL_PORT &= ~(1 << SSL);
    spi_transfer((reg << 1) & 0x7E);
    spi_transfer(value);
    SSL_PORT |= (1 << SSL);
}

// Read from RC522 Register links
uint8_t read_reg_L(uint8_t reg) {
    uint8_t value;
    SSL_PORT &= ~(1 << SSL);
    spi_transfer(((reg << 1) & 0x7E) | 0x80);
    value = spi_transfer(0x00);
    SSL_PORT |= (1 << SSL);
    return value;
}


// Initialize the RC522
void rc522_init() {
    // Perform a hard reset by cycling the RST pin

    RSTR_PORT |= (1 << RSTR);
    _delay_ms(50);
    RSTR_PORT &= ~(1 << RSTR);
    _delay_ms(50);
    RSTR_PORT |= (1 << RSTR);
    _delay_ms(50);

    RSTL_PORT |= (1 << RSTL);
    _delay_ms(50);
    RSTL_PORT &= ~(1 << RSTL);
    _delay_ms(50);
    RSTL_PORT |= (1 << RSTL);
    _delay_ms(50);

    // Default configuration to wake up internal transceivers
    write_reg_R(ModeReg, 0x3D);
    write_reg_R(TxASKReg, 0x40);

    write_reg_L(ModeReg, 0x3D);
    write_reg_L(TxASKReg, 0x40);

    // Turn on the reading antenna
    uint8_t currentR = read_reg_R(TxControlReg);
    if ((currentR & 0x03) != 0x03) {
        write_reg_R(TxControlReg, currentR | 0x03);
    }

    uint8_t currentL = read_reg_L(TxControlReg);
    if ((currentL & 0x03) != 0x03) {
        write_reg_L(TxControlReg, currentL | 0x03);
    }
}

// Scan for a passive RFID tag rechts
int tag_detectedR() {
    write_reg_R(CommandReg, PCD_IDLE);
    write_reg_R(ComIrqReg, 0x7F);
    write_reg_R(FIFOLevelReg, 0x80); // Flush FIFO

    write_reg_R(FIFODataReg, PICC_REQIDL);
    write_reg_R(CommandReg, PCD_TRANSCEIVE);
    write_reg_R(BitFramingReg, 0x87); // Start communication

    // Wait for the operations to complete or timeout
    uint16_t timeout = 1000;
    while (timeout > 0) {
        uint8_t irq = read_reg_R(ComIrqReg);
        if (irq & 0x30) { // Check if Rx and Idle IRQs are set
            break;
        }
        timeout--;
        _delay_us(10);
    }

    if (timeout == 0) return 0;

    // Check for operational or buffer errors
    uint8_t error = read_reg_R(ErrorReg);
    if (error & 0x13) return 0;

    return 1;
}

// Scan for a passive RFID tag links
int tag_detectedL() {
    write_reg_L(CommandReg, PCD_IDLE);
    write_reg_L(ComIrqReg, 0x7F);
    write_reg_L(FIFOLevelReg, 0x80); // Flush FIFO

    write_reg_L(FIFODataReg, PICC_REQIDL);
    write_reg_L(CommandReg, PCD_TRANSCEIVE);
    write_reg_L(BitFramingReg, 0x87); // Start communication

    // Wait for the operations to complete or timeout
    uint16_t timeout = 1000;
    while (timeout > 0) {
        uint8_t irq = read_reg_L(ComIrqReg);
        if (irq & 0x30) { // Check if Rx and Idle IRQs are set
            break;
        }
        timeout--;
        _delay_us(10);
    }

    if (timeout == 0) return 0;

    // Check for operational or buffer errors
    uint8_t error = read_reg_L(ErrorReg);
    if (error & 0x13) return 0;

    return 1;
}

void RFID_opstarten(void) {
    initialisatie_RFID();
    rc522_init();
/*
    int tag = 0;

    while (1) {
        if (tag_detectedR()) {
            LED_BLAUW_PORT |= (1 << LED_BLAUW);
            _delay_ms(100);
            tag = 1;
        }
        else {
            if (tag == 1) {
                tag = 0;
            }
            else {
                LED_BLAUW_PORT &= ~(1 << LED_BLAUW);
            }
        }
    }*/
}

// hoe zorg je dat je hier na 2 sec weer uit bent? Is 1 keer checken genoeg?
int RFID_scannen(int kant){ // kant = 0 = rechts, kant = 1 = links
    if (kant == 0) { //doosje rechts
        if (tag_detectedR()) { //tag gedetecteerd
            _delay_ms(5);
            return 1;
        }
        else { // geen tag
            _delay_ms(5);
            return 0;
        }
    }
    else { // doosje links
        if (tag_detectedL()) { // tag gedetecteerd
            _delay_ms(5);
            return 1;
        }
        else { // geen tag
            _delay_ms(5);
            return 0;
        }
    }
}

