#include "rtc.hpp"
#include <stdint.h>
#include "inc/hw_i2c.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/i2c.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"


#define RTC_DIR 108

void RTC::i2cInic() // Initialize I2C peripheral
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_I2C0));
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOB));
    GPIOPinConfigure(GPIO_PB2_I2C0SCL);
    GPIOPinConfigure(GPIO_PB3_I2C0SDA);
    GPIOPinTypeI2C(GPIO_PORTB_BASE, GPIO_PIN_3);
    GPIOPinTypeI2CSCL(GPIO_PORTB_BASE, GPIO_PIN_2);
    I2CMasterInitExpClk(I2C0_BASE, 120000000, false); // 100kbps
    I2CMasterGlitchFilterConfigSet(I2C0_BASE, I2C_MASTER_GLITCH_FILTER_DISABLED);
}
uint8_t RTC::bcdToint(uint8_t x){ // Function with the purpose to transform bcd to int values
    uint8_t  dec1 = x >> 4;
    uint8_t un1 = x << 4;
    uint8_t un2 = un1 >> 4;
    uint8_t resF = (dec1*10) + un2;
    return resF;
}
uint8_t RTC::intTobcd(uint8_t x){ // Function with the purpose to transform int to bcd values
    uint8_t  dec = x/10;
    uint8_t  un =  x%10;
    uint8_t  numero = dec << 4 | un;
    return numero;
}
uint8_t RTC::bcdTointHr(uint8_t hr){
    uint8_t un1 = hr << 4;
    uint8_t un2 = un1 >> 4;
    uint8_t  dec1 = hr << 3;
    uint8_t  dec2 = dec1 >> 7;
    uint8_t resF = (dec2*10) + un2;
    return resF;
}
uint8_t RTC::intTobcdHrAm(uint8_t hr){
  uint8_t  dec = hr/10;
  uint8_t  un =  hr%10;
  uint8_t  numero = dec << 4 | un;
  uint8_t resF = 64 | numero;;
  return resF;
}
uint8_t RTC::intTobcdHrPm(uint8_t hr){
  uint8_t  dec = hr/10;
  uint8_t  un =  hr%10;
  uint8_t  numero = dec << 4 | un;
  uint8_t resF = 96 | numero;;
  return resF;
}

uint8_t RTC::leerSeg(){ // reading the register of seconds
    I2CMasterSlaveAddrSet(I2C0_BASE, this->rtcDir , false); // escritura
    I2CMasterDataPut(I2C0_BASE, this->reg[0]); //escribo
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_SINGLE_SEND); //envio
    while(!(I2CMasterBusy(I2C0_BASE)));  //espero
    while(I2CMasterBusy(I2C0_BASE));  //espero

    I2CMasterSlaveAddrSet(I2C0_BASE, this->rtcDir , true); ///lectura
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_SINGLE_RECEIVE); //cargo
    while(!(I2CMasterBusy(I2C0_BASE)));
    while(I2CMasterBusy(I2C0_BASE));
    this->segundos = RTC::bcdToint(I2CMasterDataGet(I2C0_BASE));
    return I2CMasterDataGet(I2C0_BASE);
}
void RTC::escSeg(uint8_t x){ // writing seconds register
    I2CMasterSlaveAddrSet(I2C0_BASE, this->rtcDir , false); // escritura
    I2CMasterDataPut(I2C0_BASE, this->reg[0]); // direccionamos al registro 0
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_START);
    while(!(I2CMasterBusy(I2C0_BASE)));
    while(I2CMasterBusy(I2C0_BASE));

    I2CMasterDataPut(I2C0_BASE, x); // escribimos en el registro
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_CONT);
    while(!(I2CMasterBusy(I2C0_BASE)));
    while(I2CMasterBusy(I2C0_BASE));

    //I2CMasterDataPut(I2C0_BASE, x);
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_FINISH);
    while(!(I2CMasterBusy(I2C0_BASE)));
    while(I2CMasterBusy(I2C0_BASE));

}
uint8_t RTC::leerMin(){
    I2CMasterSlaveAddrSet(I2C0_BASE, this->rtcDir , false); // escritura
    I2CMasterDataPut(I2C0_BASE, reg[1]); //escribo
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_SINGLE_SEND); //envio
    while(!(I2CMasterBusy(I2C0_BASE)));  //espero
    while(I2CMasterBusy(I2C0_BASE));  //espero

    I2CMasterSlaveAddrSet(I2C0_BASE, this->rtcDir , true); ///lectura
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_SINGLE_RECEIVE); //cargo
    while(!(I2CMasterBusy(I2C0_BASE)));
    while(I2CMasterBusy(I2C0_BASE));
    this->minuto = RTC::bcdToint(I2CMasterDataGet(I2C0_BASE));
    return I2CMasterDataGet(I2C0_BASE);
}
void RTC::escMin(uint8_t x){
    I2CMasterSlaveAddrSet(I2C0_BASE, this->rtcDir , false); // escritura
    I2CMasterDataPut(I2C0_BASE, this->reg[1]); // direccionamos al registro 0
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_START);
    while(!(I2CMasterBusy(I2C0_BASE)));
    while(I2CMasterBusy(I2C0_BASE));

    I2CMasterDataPut(I2C0_BASE, x); // escribimos en el registro
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_CONT);
    while(!(I2CMasterBusy(I2C0_BASE)));
    while(I2CMasterBusy(I2C0_BASE));

    //I2CMasterDataPut(I2C0_BASE, x);
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_FINISH);
    while(!(I2CMasterBusy(I2C0_BASE)));
    while(I2CMasterBusy(I2C0_BASE));
}
uint8_t RTC::leerHr(){
    I2CMasterSlaveAddrSet(I2C0_BASE, this->rtcDir , false); // escritura
    I2CMasterDataPut(I2C0_BASE, reg[2]); //escribo
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_SINGLE_SEND); //envio
    while(!(I2CMasterBusy(I2C0_BASE)));  //espero
    while(I2CMasterBusy(I2C0_BASE));  //espero

    I2CMasterSlaveAddrSet(I2C0_BASE, this->rtcDir , true); ///lectura
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_SINGLE_RECEIVE); //cargo
    while(!(I2CMasterBusy(I2C0_BASE)));
    while(I2CMasterBusy(I2C0_BASE));

    this->hora = RTC::bcdTointHr(I2CMasterDataGet(I2C0_BASE));
    this->am_pm = I2CMasterDataGet(I2C0_BASE) & 32;
    if( this->am_pm > 0){
        if(this->hora != 12){
            this->hora  =  this->hora + 12;
        }
    }
    else{
        if(this->hora == 12){
            this->hora = 0;
        }
    }

    return I2CMasterDataGet(I2C0_BASE);
}
void RTC::escHr(uint8_t x){
    I2CMasterSlaveAddrSet(I2C0_BASE, this->rtcDir , false); // escritura
    I2CMasterDataPut(I2C0_BASE, this->reg[2]); // direccionamos al registro 0
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_START);
    while(!(I2CMasterBusy(I2C0_BASE)));
    while(I2CMasterBusy(I2C0_BASE));

    I2CMasterDataPut(I2C0_BASE, x); // escribimos en el registro
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_CONT);
    while(!(I2CMasterBusy(I2C0_BASE)));
    while(I2CMasterBusy(I2C0_BASE));

    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_FINISH);
    while(!(I2CMasterBusy(I2C0_BASE)));
    while(I2CMasterBusy(I2C0_BASE));
}
uint8_t RTC::leerDia(){
    I2CMasterSlaveAddrSet(I2C0_BASE, this->rtcDir , false); // escritura
    I2CMasterDataPut(I2C0_BASE, reg[3]); //escribo
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_SINGLE_SEND); //envio
    while(!(I2CMasterBusy(I2C0_BASE)));  //espero
    while(I2CMasterBusy(I2C0_BASE));  //espero

    I2CMasterSlaveAddrSet(I2C0_BASE, this->rtcDir , true); ///lectura
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_SINGLE_RECEIVE); //cargo
    while(!(I2CMasterBusy(I2C0_BASE)));
    while(I2CMasterBusy(I2C0_BASE));

    return I2CMasterDataGet(I2C0_BASE);;
}
void RTC::escDia(uint8_t x){
    I2CMasterSlaveAddrSet(I2C0_BASE, this->rtcDir , false); // escritura
    I2CMasterDataPut(I2C0_BASE, this->reg[3]); // direccionamos al registro 0
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_START);
    while(!(I2CMasterBusy(I2C0_BASE)));
    while(I2CMasterBusy(I2C0_BASE));

    I2CMasterDataPut(I2C0_BASE, x); // escribimos en el registro
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_CONT);
    while(!(I2CMasterBusy(I2C0_BASE)));
    while(I2CMasterBusy(I2C0_BASE));

    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_FINISH);
    while(!(I2CMasterBusy(I2C0_BASE)));
    while(I2CMasterBusy(I2C0_BASE));
}
uint8_t RTC::leerFecha(){
    I2CMasterSlaveAddrSet(I2C0_BASE, this->rtcDir , false); // escritura
    I2CMasterDataPut(I2C0_BASE, reg[4]); //escribo
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_SINGLE_SEND); //envio
    while(!(I2CMasterBusy(I2C0_BASE)));  //espero
    while(I2CMasterBusy(I2C0_BASE));  //espero

    I2CMasterSlaveAddrSet(I2C0_BASE, this->rtcDir , true); ///lectura
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_SINGLE_RECEIVE); //cargo
    while(!(I2CMasterBusy(I2C0_BASE)));
    while(I2CMasterBusy(I2C0_BASE));
    this->fecha = RTC::bcdToint(I2CMasterDataGet(I2C0_BASE));
    return I2CMasterDataGet(I2C0_BASE);;
}
void RTC::escFecha(uint8_t x){
    I2CMasterSlaveAddrSet(I2C0_BASE, this->rtcDir , false); // escritura
    I2CMasterDataPut(I2C0_BASE, this->reg[4]); // direccionamos al registro 0
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_START);
    while(!(I2CMasterBusy(I2C0_BASE)));
    while(I2CMasterBusy(I2C0_BASE));

    I2CMasterDataPut(I2C0_BASE, x); // escribimos en el registro
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_CONT);
    while(!(I2CMasterBusy(I2C0_BASE)));
    while(I2CMasterBusy(I2C0_BASE));

    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_FINISH);
    while(!(I2CMasterBusy(I2C0_BASE)));
    while(I2CMasterBusy(I2C0_BASE));
}
uint8_t RTC::leerMes(){
    I2CMasterSlaveAddrSet(I2C0_BASE, this->rtcDir , false); // escritura
    I2CMasterDataPut(I2C0_BASE, reg[5]); //escribo
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_SINGLE_SEND); //envio
    while(!(I2CMasterBusy(I2C0_BASE)));  //espero
    while(I2CMasterBusy(I2C0_BASE));  //espero

    I2CMasterSlaveAddrSet(I2C0_BASE, this->rtcDir , true); ///lectura
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_SINGLE_RECEIVE); //cargo
    while(!(I2CMasterBusy(I2C0_BASE)));
    while(I2CMasterBusy(I2C0_BASE));
    this->mes = RTC::bcdToint(I2CMasterDataGet(I2C0_BASE));
    return I2CMasterDataGet(I2C0_BASE);;
}
void RTC::escMes(uint8_t x){
    I2CMasterSlaveAddrSet(I2C0_BASE, this->rtcDir , false); // escritura
    I2CMasterDataPut(I2C0_BASE, this->reg[5]); // direccionamos al registro 0
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_START);
    while(!(I2CMasterBusy(I2C0_BASE)));
    while(I2CMasterBusy(I2C0_BASE));

    I2CMasterDataPut(I2C0_BASE, x); // escribimos en el registro
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_CONT);
    while(!(I2CMasterBusy(I2C0_BASE)));
    while(I2CMasterBusy(I2C0_BASE));

    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_FINISH);
    while(!(I2CMasterBusy(I2C0_BASE)));
    while(I2CMasterBusy(I2C0_BASE));
}
uint8_t RTC::leerAnio(){
    I2CMasterSlaveAddrSet(I2C0_BASE, this->rtcDir , false); // escritura
    I2CMasterDataPut(I2C0_BASE, reg[6]); //escribo
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_SINGLE_SEND); //envio
    while(!(I2CMasterBusy(I2C0_BASE)));  //espero
    while(I2CMasterBusy(I2C0_BASE));  //espero

    I2CMasterSlaveAddrSet(I2C0_BASE, this->rtcDir , true); ///lectura
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_SINGLE_RECEIVE); //cargo
    while(!(I2CMasterBusy(I2C0_BASE)));
    while(I2CMasterBusy(I2C0_BASE));
    this->anio = RTC::bcdToint(I2CMasterDataGet(I2C0_BASE));
    return I2CMasterDataGet(I2C0_BASE);;
}
void RTC::escAnio(uint8_t x){
    I2CMasterSlaveAddrSet(I2C0_BASE, this->rtcDir , false); // escritura
    I2CMasterDataPut(I2C0_BASE, this->reg[6]); // direccionamos al registro 0
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_START);
    while(!(I2CMasterBusy(I2C0_BASE)));
    while(I2CMasterBusy(I2C0_BASE));

    I2CMasterDataPut(I2C0_BASE, x); // escribimos en el registro
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_CONT);
    while(!(I2CMasterBusy(I2C0_BASE)));
    while(I2CMasterBusy(I2C0_BASE));

    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_FINISH);
    while(!(I2CMasterBusy(I2C0_BASE)));
    while(I2CMasterBusy(I2C0_BASE));
}
void RTC::lectura(){
    RTC::leerSeg();
    RTC::leerMin();
    RTC::leerHr();
    RTC::leerFecha();
    RTC::leerMes();
    RTC::leerAnio();
}
void RTC::escrituraPm(int seg, int min, int hr, int fec, int mes, int an){
    RTC::escSeg(RTC::intTobcd(seg));
    RTC::escMin(RTC::intTobcd(min));
    RTC::escHr(RTC::intTobcdHrPm(hr));
    RTC::escFecha(RTC::intTobcd(fec));
    RTC::escMes(RTC::intTobcd(mes));
    RTC::escAnio(RTC::intTobcd(an));
}
void RTC::escrituraAm(int seg, int min, int hr, int fec, int mes, int an){
    RTC::escSeg(RTC::intTobcd(seg));
    RTC::escMin(RTC::intTobcd(min));
    RTC::escHr(RTC::intTobcdHrAm(hr));
    RTC::escFecha(RTC::intTobcd(fec));
    RTC::escMes(RTC::intTobcd(mes));
    RTC::escAnio(RTC::intTobcd(an));
}
