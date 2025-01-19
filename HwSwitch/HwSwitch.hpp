//	ハードウェアスイッチ
//	『昼夜逆転』工作室	http://jsdiy.starfree.jp
//	2025/01	@jsdiy
//	2025/01	AVR版(2019/01)を移植、ReleaseとOffの意味を入れ替え
/*
	| gpio|--<SW>--+		SW: button switch, slide switch, etc.
	|     |        |		turn on:LOW, off:HIGH
	|ESP32|       GND
*/

#pragma	once

#include <Arduino.h>

//スイッチの押下状態
enum	class	ESwState	: uint8_t
{
	On,			//オンになった瞬間
	ShortHold,	//オンであり続けている状態（指定時間未満）
	LongHold,	//オンであり続けている状態（指定時間以上経過）
	Release,	//オフになった瞬間
	Off			//オフであり続けている状態
};

//ハードウェアスイッチ
class	HwSwitch
{
private:
	static	const	uint8_t	SwOn = LOW,	SwOff = HIGH;
	gpio_num_t	swPin = GPIO_NUM_NC;
	uint8_t		prevPinState;	//値はSwOn/SwOffを取る
	uint16_t	longHoldThreshold;	//Status()が何回呼ばれたら「長押し」とするか
	uint16_t	holdCount;

public:
	HwSwitch(void) {}	//この場合は後でInitialize()を呼ぶこと
	HwSwitch(gpio_num_t swPin, uint16_t longHoldThreshold = 0) { Initialize(swPin, longHoldThreshold); }
	void	Initialize(gpio_num_t swPin, uint16_t longHoldThreshold = 0);
	ESwState	State(void);
};
