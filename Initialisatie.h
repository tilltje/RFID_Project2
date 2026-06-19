#ifndef INITIALISATIE_H_INCLUDED
#define INITIALISATIE_H_INCLUDED

//Communicatiepinnen
#define MODNUMMER           PG0 // Arduino Pin 41
#define MODNUMMER_DDR       DDRG
#define MODNUMMER_PORT      PORTG

#define NEXT_MOD            PG1 // Arduino Pin 40
#define NEXT_MOD_DDR        DDRG
#define NEXT_MOD_PORT       PORTG

#define ACK_MOD             PG2 // Arduino Pin 39
#define ACK_MOD_DDR         DDRG
#define ACK_MOD_PORT        PORTG

#define LIJN                PD7 // Arduino Pin 38
#define LIJN_DDR            DDRD
#define LIJN_PIN            PIND
#define LIJN_PORT           PORTD

#define NEXT_AGV            PC0 // Arduino Pin 37
#define NEXT_AGV_DDR        DDRC
#define NEXT_AGV_PIN        PINC
#define NEXT_AGV_PORT       PORTC

#define ACK_AGV             PD0 //Arduino Pin 21 (externe interrupt)
#define ACK_AGV_DDR         DDRD
#define ACK_AGV_PIN         PIND
#define ACK_AGV_PORT        PORTD

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
#define IR_L                PL4 // Arduino Pin 45
#define IR_L_DDR            DDRL
#define IR_L_PIN            PINL
#define IR_L_PORT           PORTL

#define IR_R                PL5 // Arduino Pin 44
#define IR_R_DDR            DDRL
#define IR_R_PIN            PINL
#define IR_R_PORT           PORTL

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
#define LED_BLAUW           PC2 // Arduino pin 35
#define LED_BLAUW_DDR       DDRC
#define LED_BLAUW_PORT      PORTC

#define LED_GROEN           PC3 // Arduino pin 34
#define LED_GROEN_DDR       DDRC
#define LED_GROEN_PORT      PORTC

// Knop
#define KNOP                PC4 // Arduino pin 33
#define KNOP_DDR            DDRC
#define KNOP_PIN            PINC
#define KNOP_PORT           PORTC

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
