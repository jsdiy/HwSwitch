#include <Arduino.h>
#include "GpioPort.hpp"

//GpioPortの測定、GPIO個別制御の測定	※どれか一つを1、他は0にする
#define	TEST_GPIOPORT_WRITE	1
#define	TEST_GPIOPORT_READ	0
#define	TEST_ONEBYONE_WRITE	0
#define	TEST_ONEBYONE_READ	0

GpioPort	port;

const	static	uint8_t	BitLen = 8;
uint8_t	bits[BitLen];
const	gpio_num_t	pins[] =
{
	GPIO_NUM_32,	//bit0	//上位32bitレジスタに含まれるピン
	GPIO_NUM_33,	//bit1	//上位32bitレジスタに含まれるピン
//	GPIO_NUM_22,	//bit0
//	GPIO_NUM_21,	//bit1
	GPIO_NUM_25,	//bit2
	GPIO_NUM_26,	//bit3
	GPIO_NUM_27,	//bit4
	GPIO_NUM_14,	//bit5
	GPIO_NUM_17,	//bit6
	GPIO_NUM_16,	//bit7
};

void	setup(void)
{
	Serial.begin(115200);

	port.Initialize(GPIO_NUM_16, GPIO_NUM_17, GPIO_NUM_14, GPIO_NUM_27,
		GPIO_NUM_26, GPIO_NUM_25, /*GPIO_NUM_21, GPIO_NUM_22*/ GPIO_NUM_33, GPIO_NUM_32);

#if	TEST_GPIOPORT_WRITE
	port.portMode(OUTPUT);
#endif
#if	TEST_GPIOPORT_READ
	port.portMode(INPUT_PULLUP);
#endif

#if	TEST_ONEBYONE_WRITE
	for (uint8_t i = 0; i < BitLen; i++) { pinMode(pins[i], OUTPUT); }
#endif
#if	TEST_ONEBYONE_READ
	for (uint8_t i = 0; i < BitLen; i++) { pinMode(pins[i], INPUT_PULLUP); }
#endif
}

void	loop(void)
{
#if	(0)	//GpioPort完成確認
	port.digitalWrite(val);
	delay(200);

	val = port.digitalRead();
	val++;
#endif

#if	TEST_GPIOPORT_WRITE
	//50ms周期(20MHz)で[Hi_Lo_]出力
	port.digitalWrite(0xFF);
	delay(25);
	port.digitalWrite(0x00);
	delay(25);
#endif
#if	TEST_GPIOPORT_READ
	uint8_t val = port.digitalRead();
Serial.printf("%02X ", val);
#endif

#if	TEST_ONEBYONE_WRITE
	//50ms周期(20MHz)で[Hi_Lo_]出力
	//for (uint8_t i = 0; i < BitLen; i++) { digitalWrite(pins[i], HIGH); }
	//↓わずかでも高速性を期待してベタ書き
	digitalWrite(pins[0], HIGH);	digitalWrite(pins[1], HIGH);
	digitalWrite(pins[2], HIGH);	digitalWrite(pins[3], HIGH);
	digitalWrite(pins[4], HIGH);	digitalWrite(pins[5], HIGH);
	digitalWrite(pins[6], HIGH);	digitalWrite(pins[7], HIGH);
	delay(25);
	//for (uint8_t i = 0; i < BitLen; i++) { digitalWrite(pins[i], LOW); }
	digitalWrite(pins[0], LOW);	digitalWrite(pins[1], LOW);
	digitalWrite(pins[2], LOW);	digitalWrite(pins[3], LOW);
	digitalWrite(pins[4], LOW);	digitalWrite(pins[5], LOW);
	digitalWrite(pins[6], LOW);	digitalWrite(pins[7], LOW);
	delay(25);
#endif
#if	TEST_ONEBYONE_READ
	//for (uint8_t i = 0; i < BitLen; i++) { bits[i] = digitalRead(pins[i]); }
	//↓わずかでも高速性を期待してベタ書き
	bits[0] = digitalRead(pins[0]);	bits[1] = digitalRead(pins[1]);
	bits[2] = digitalRead(pins[2]);	bits[3] = digitalRead(pins[3]);
	bits[4] = digitalRead(pins[4]);	bits[5] = digitalRead(pins[5]);
	bits[6] = digitalRead(pins[6]);	bits[7] = digitalRead(pins[7]);
	uint8_t val = 0x00;
	for (uint8_t i = 0; i < BitLen; i++) { val |= (bits[i] << i); }
Serial.printf("%02X ", val);
#endif
}
