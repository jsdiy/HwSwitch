//	ハードウェアスイッチ
//	『昼夜逆転』工作室	https://github.com/jsdiy
//	2025/01 - 2025/09	@jsdiy

#include <Arduino.h>
#include "HwSwitch.hpp"

//初期化
void	HwSwitch::Initialize(gpio_num_t swPin, ulong longHoldThresholdMSec)
{
	this->swPin = swPin;
	pinMode(swPin, INPUT_PULLUP);
	longHoldThresholdTime = longHoldThresholdMSec;
	prevPinState = SwOff;
	prevMills = 0;
}

//ボタンの押下状態を判定する
ESwState	HwSwitch::State(void)
{
	int8_t nowPinState;
	auto nowMills = millis();
	if (DebounceTime < nowMills - prevMills)
	{
		nowPinState = digitalRead(swPin);
		prevMills = nowMills;
	}
	else
	{
		nowPinState = prevPinState;
	}

	ESwState swState;
	if (prevPinState == SwOff && nowPinState == SwOn)
	{
		swState = ESwState::On;
		holdStartTime = nowMills;
	}
	else if (prevPinState == SwOn && nowPinState == SwOn)
	{
		swState = (nowMills - holdStartTime < longHoldThresholdTime)
			? ESwState::ShortHold : ESwState::LongHold;
	}
	else if (prevPinState == SwOn && nowPinState == SwOff)
	{
		swState = ESwState::Release;
		holdStartTime = 0;
	}
	else	//(prevPinState == SwOff && nowPinState == SwOff)
	{
		swState = ESwState::Off;
	}

	prevPinState = nowPinState;
	return swState;
}
