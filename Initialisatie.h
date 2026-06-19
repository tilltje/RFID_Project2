#ifndef INITIALISATIE_H_INCLUDED
#define INITIALISATIE_H_INCLUDED

//Communicatiepinnen
#define MODNUMMER           PG2
#define MODNUMMER_DDR       DDRG
#define MODNUMMER_PORT      PORTG

#define VOLGENDE_MOD        PD7
#define VOLGENDE_MOD_DDR    DDRD
#define VOLGENDE_MOD_PORT   PORTD

#define ACK_MOD             PC0
#define ACK_MOD_DDR         DDRC
#define ACK_MOD_PORT        PORTC

#define LIJN                PC1
#define LIJN_DDR            DDRC
#define LIJN_PIN            PINC

#define VOLGENDE_AGV        PC2
#define VOLGENDE_AGV_DDR    DDRC
#define VOLGENDE_AGV_PIN    PINC

#define ACK_AGV             PC3
#define ACK_AGV_DDR             DDRC
#define ACK_AGV_PIN             PINC

//RFID protocol
#define PCD_IDLE              0x00
#define PCD_TRANSCEIVE        0x0C
#define PICC_REQIDL           0x26

#define CommandReg            0x01
#define ComIEnReg             0x02
#define ComIrqReg             0x04
#define ErrorReg              0x06
#define FIFODataReg           0x09
#define FIFOLevelReg          0x0A
#define BitFramingReg         0x0D
#define ModeReg               0x11
#define TxControlReg          0x14
#define TxASKReg              0x15

//RFID pinnen
#define SSR                 PL0 // Arduino Pin 49
#define SSR_DDR             DDRL
#define SSR_PORT            PORTL

#define SSL                 PL2 // Arduino Pin 47
#define SSL_DDR             DDRL
#define SSL_PORT            PORTL

#define SCK                 PB1 // Arduino Pin 52
#define SCK_DDR             DDRB
#define SCK_PORT            PORTB

#define MOSI                PB2 // Arduino Pin 51
#define MOSI_DDR            DDRB
#define MOSI_PORT           PORTB

#define MISO                PB3 // Arduino Pin 50
#define MISO_DDR            DDRB
#define MISO_PIN            PINB

#define RSTR                PL1 // Arduino Pin 48
#define RSTR_DDR            DDRL
#define RSTR_PORT           PORTL

#define RSTL                PL3 // Arduino Pin 46
#define RSTL_DDR            DDRL
#define RSTL_PORT           PORTL

//IR sensor pinnen
#define IR_R                PL4 // Arduino Pin 45
#define IR_R_DDR            DDRL
#define IR_R_PIN            PINL

#define IR_L                PL5 // Arduino Pin 44
#define IR_L_DDR            DDRL
#define IR_L_PIN            PINL

//Display
#define CLK                 PL6 // Arduino pin 43
#define CLK_DDR             DDRL
#define CLK_PORT            PORTL

#define DIO                 PL7 // Arduino pin 42
#define DIO_DDR             DDRL
#define DIO_PORT            PORTL

#define CLK_LOW()    (CLK_PORT &= ~(1 << CLK))
#define CLK_HIGH()   (CLK_PORT |= (1 << CLK))
#define DIO_LOW()    (DIO_PORT &= ~(1 << DIO))
#define DIO_HIGH()   (DIO_PORT |= (1 << DIO))
#define DIO_OUTPUT() (DIO_DDR  |= (1 << DIO))
#define DIO_INPUT()  (DIO_DDR  &= ~(1 << DIO))

//LEDS
#define LED_BLAUW           PG0 // Arduino pin 41
#define LED_BLAUW_DDR       DDRG
#define LED_BLAUW_PORT      PORTG

#define LED_GROEN           PG1 // Arduino pin 40
#define LED_GROEN_DDR       DDRG
#define LED_GROEN_PORT      PORTG

// Knop
#define KNOP                PC4
#define KNOP_DDR            DDRC
#define KNOP_PIN            PINC

void send_byte(uint8_t data);
void tm_start();
void tm_stop();

void initialisatie_communicatie(void);
void initialisatie_RFID(void);
void initialisatie_IRsensor(void);
void initialisatie_display(void);
void initialisatie_leds(void);
void initialisatie_knop(void);

void initialisatie(void);

#endif // INITIALISATIE_H_INCLUDED
