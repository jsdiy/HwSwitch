//	GPIOを8ビットのポートとして操作する
//	・ESP32のGPIOのうち任意の8個をまとめて1つのポートとして制御し、8bit値を入出力できるようにします。
//	『昼夜逆転』工作室	http://jsdiy.starfree.jp/
//	2024/07	@jsdiy

/*	参考にしたヘッダーファイル：
	esp32-hal-gpio.h	pinMode()等および、それらの引数等
	esp_bit_defs.h	BIT()等
	gpio_types.h	gpio_num_t等
	gpio.h	gpio_output_set()等	C:\Users\{USERNAME}\.platformio\packages\framework-arduinoespressif32\tools\sdk\esp32\include\esp_rom\include\esp32\rom\gpio.h
	gpio.h	gpio_config()等　※GpioPortでは未使用	C:\Users\{USERNAME}\.platformio\packages\framework-arduinoespressif32\tools\sdk\esp32\include\driver\include\driver\gpio.h
*/

#include <Arduino.h>
#include "GpioPort.hpp"

//初期化
//・8bit分のGPIOを割り当てる。bit7がMSB、bit0がLSB。
//・上位4bitのみ／下位4bitのみ使うなども可。使用しないビットには GPIO_NUM_NC を渡す。
void	GpioPort::Initialize(gpio_num_t bit7, gpio_num_t bit6, gpio_num_t bit5, gpio_num_t bit4,
			gpio_num_t bit3, gpio_num_t bit2, gpio_num_t bit1, gpio_num_t bit0)
{
	pins[7] = bit7;	pins[6] = bit6;	pins[5] = bit5;	pins[4] = bit4;
	pins[3] = bit3;	pins[2] = bit2;	pins[1] = bit1;	pins[0] = bit0;

	for (uint8_t i = 0; i < PinMax; i++)
	{
		if (GPIO_NUM_0 <= pins[i] && pins[i] <= GPIO_NUM_31)
		{
			oneBitmask32[i] = BIT(pins[i]);
			oneBitmaskHigh[i] = 0;
		}
		else if (GPIO_NUM_32 <= pins[i] && pins[i] <= GPIO_NUM_39)
		{
			oneBitmask32[i] = 0;
			oneBitmaskHigh[i] = BIT(pins[i] - 32);
		}
		else
		{
			//pins[i]がGPIO_NUM_NCの場合（または無効値の場合）
			oneBitmask32[i] = 0;
			oneBitmaskHigh[i] = 0;
		}
	}
}

//ピンモードを設定する
//引数：	ArduinoフレームワークのpinMode()と同じ
void	GpioPort::portMode(uint8_t mode)
{
	for (uint8_t i = 0; i < PinMax; i++) { pinMode(pins[i], mode); }
}

//ポートに値を書く
void	GpioPort::digitalWrite(uint8_t val)
{
	uint32_t setBitmask32 = 0, clearBitmask32 = 0;
#if	USE_GPIO32_OR_HIGHER
	uint32_t setBitmaskHigh = 0, clearBitmaskHigh = 0;
#endif

	for (uint8_t i = 0; i < PinMax; i++)
	{
		bool isBitOn = (val & BitVal[i]);
#if	USE_GPIO32_OR_HIGHER
		if (GPIO_NUM_32 <= pins[i])
		{
			(isBitOn ? setBitmaskHigh : clearBitmaskHigh) |= oneBitmaskHigh[i];
		}
		else
#endif
		if (GPIO_NUM_0 <= pins[i])
		{
			(isBitOn ? setBitmask32 : clearBitmask32) |= oneBitmask32[i];
		}
	}

	gpio_output_set(setBitmask32, clearBitmask32, 0, 0);
#if	USE_GPIO32_OR_HIGHER
	gpio_output_set_high(setBitmaskHigh, clearBitmaskHigh, 0, 0);
#endif
}

//ポートの値を読む
uint8_t	GpioPort::digitalRead(void)
{
	uint8_t val = 0x00;
	uint32_t bitValue32 = gpio_input_get();
#if	USE_GPIO32_OR_HIGHER
	uint32_t bitValueHigh = gpio_input_get_high();
/*
・実験により、gpio_input_get()の後にgpio_input_get_high()が実行されるまでの時間は208ns.
・gpio_input_get()とgpio_input_get_high()の間（208nsの間）に入力信号が変化していることもあり得るので、
	厳密にはgpio_input_get_high()の後に再度gpio_input_get()を実行し、
	1回目のgpio_input_get()と値が異なっていたら2回目のgpio_input_get()の値を採用する、というようにする。
・なお、この関数で追従できる入力信号の変化の速さは、1/208ns=4.8MHz以下ということになる。
*/
#endif

	for (uint8_t i = 0; i < PinMax; i++)
	{
#if	USE_GPIO32_OR_HIGHER
		if (GPIO_NUM_32 <= pins[i])
		{
			if (bitValueHigh & oneBitmaskHigh[i]) { val |= BitVal[i]; }
		}
		else
#endif
		if (GPIO_NUM_0 <= pins[i])
		{
			if (bitValue32 & oneBitmask32[i]) { val |= BitVal[i]; }
		}
	}

	return val;
}
