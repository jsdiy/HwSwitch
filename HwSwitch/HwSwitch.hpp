//	ハードウェアスイッチ
//	『昼夜逆転』工作室	http://jsdiy.starfree.jp
//	2025/01	@jsdiy
//	2025/01	AVR版(2019/01)を移植、ReleaseとOffの意味を入れ替え
//	2025/09	ESP32版としてmills()利用へ改造
/*
	| gpio|--<SW>--+	SW: button switch, slide switch, etc.
	|     |        |	turn on:LOW, off:HIGH
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
	static	const	int8_t	SwOn = LOW,	SwOff = HIGH;
	static	const	ulong	DebounceTime = 50;
	gpio_num_t	swPin;
	int8_t	prevPinState;	//値はSwOn/SwOffを取る
	ulong	prevMills;
	ulong	longHoldThresholdTime, holdStartTime;

public:
	HwSwitch(void) {}	//この場合は後でInitialize()を呼ぶ必要がある
	HwSwitch(gpio_num_t swPin, ulong longHoldThresholdMSec = 0) { Initialize(swPin, longHoldThresholdMSec); }
	void	Initialize(gpio_num_t swPin, ulong longHoldThresholdMSec = 0);
	ESwState	State(void);
};
