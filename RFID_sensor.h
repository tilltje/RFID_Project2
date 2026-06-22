#ifndef RFID_SENSOR_H_INCLUDED
#define RFID_SENSOR_H_INCLUDED


int tag_detectedR();
int tag_detectedL();
void RFID_opstarten(void);
int RFID_scannen(int kant);

#endif // RFID_SENSOR_H_INCLUDED
