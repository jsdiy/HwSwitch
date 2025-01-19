//	GPIOを8ビットのポートとして操作する
//	・ESP32のGPIOのうち任意の8個をまとめて1つのポートとして制御し、8bit値を入出力できるようにします。
//	『昼夜逆転』工作室	http://jsdiy.starfree.jp/
//	2024/07	@jsdiy

#pragma	once

#include <Arduino.h>

//ポートのピンアサインにGPIO_NUM_32以上のピンを使用する(1), 使用しない(0)
//・使用する場合、上位32bit側のレジスタも読み書きする必要があるので、
//	厳密には8bit同時入力/同時出力にはならない。
#define	USE_GPIO32_OR_HIGHER	1

class	GpioPort
{
private:
	const	static	uint8_t	PinMax = 8;
	const	uint8_t	BitVal[PinMax] =
	{
	//bit:76543210
		0b00000001,
		0b00000010,
		0b00000100,
		0b00001000,
		0b00010000,
		0b00100000,
		0b01000000,
		0b10000000
	};

	gpio_num_t	pins[PinMax];
	uint32_t	oneBitmask32[PinMax];	//GPIO_NUM:0-31
	uint32_t	oneBitmaskHigh[PinMax];	//GPIO_NUM:32-39

public:
	GpioPort(void) {}
	void	Initialize(gpio_num_t bit7, gpio_num_t bit6, gpio_num_t bit5, gpio_num_t bit4,
				gpio_num_t bit3, gpio_num_t bit2, gpio_num_t bit1, gpio_num_t bit0);
	void	portMode(uint8_t mode);
	void	digitalWrite(uint8_t val);
	uint8_t	digitalRead(void);
};
