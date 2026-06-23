#ifndef TESTEN_H_INCLUDED
#define TESTEN_H_INCLUDED


#define SIGNALIR            PL4 // pin 45
#define SIGNALIR_PIN        PINL
#define SIGNALIR_DDR        DDRL
#define SIGNALIR_PORT       PORTL

#define ENABLEIR            PH3
#define ENABLEIR_DDR        DDRH
#define ENABLEIR_PORT       PORTH

void test_module_versturen(void);
void init_test(void);
void IRblauwtesten(void);
void IRsensortesten(void);
void test_AGV_ontvang(void);

void testen(void);

#endif // TESTEN_H_INCLUDED
