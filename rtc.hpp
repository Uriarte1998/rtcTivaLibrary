/*
rtc.h  V 1.0
Author: Jorge Uriarte
this code was developmented to use RTC-DS3231 with tiva launchpad TM4C1294XL
It Enables I2C0 peripheral and establishes the communication with the device
*/
#include <stdint.h>

class RTC
{
private:
    uint8_t rtcDir = 104;
    uint8_t reg[8] = {0, 1, 2, 3, 4, 5, 6};
public:
    //RTC();
    uint8_t segundos;
    uint8_t minuto;
    uint8_t hora;
    uint8_t fecha;
    uint8_t mes;
    uint8_t anio;
    uint8_t am_pm;
    void i2cInic();
    uint8_t leerSeg();
    void escSeg(uint8_t x);
    uint8_t leerMin();
    void escMin(uint8_t x);
    uint8_t leerHr();
    void escHr(uint8_t x);
    uint8_t leerDia();
    void escDia(uint8_t x);
    uint8_t leerFecha();
    void escFecha(uint8_t x);
    uint8_t leerMes();
    void escMes(uint8_t x);
    uint8_t leerAnio();
    void escAnio(uint8_t x);

    void lectura();
    void escrituraPm(int seg, int min, int hr, int fec, int mes, int an);
    void escrituraAm(int seg, int min, int hr, int fec, int mes, int an);

    uint8_t intTobcd(uint8_t x);
    uint8_t bcdToint(uint8_t x);
    uint8_t bcdTointHr(uint8_t x);
    uint8_t intTobcdHrAm(uint8_t x);
    uint8_t intTobcdHrPm(uint8_t x);
};
