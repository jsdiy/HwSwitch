//
#include <Arduino.h>
#include "HwSwitch.hpp"

//初期化
void	HwSwitch::Initialize(gpio_num_t swPin, uint16_t longHoldThreshold)
{
	this->swPin = swPin;
	pinMode(swPin, INPUT_PULLUP);
	prevPinState = SwOff;

	this->longHoldThreshold = longHoldThreshold;
	holdCount = 0;
}

//ボタンの押下状態を判定する
//・一定の間隔で呼び出し、直前の押下状態と比較することで現在の状態を判定する。
//・例えば1秒間隔で呼び出せばチャタリング対策は不要。
ESwState	HwSwitch::State(void)
{
	uint8_t nowPinState = digitalRead(swPin);
	ESwState swState;

	if (prevPinState == SwOff && nowPinState == SwOn)
	{
		swState = ESwState::On;
	}
	else if (prevPinState == SwOn && nowPinState == SwOn)
	{
		if (holdCount < longHoldThreshold)
		{
			swState = ESwState::ShortHold;
			holdCount++;
		}
		else
		{
			swState = ESwState::LongHold;
		}
	}
	else if (prevPinState == SwOn && nowPinState == SwOff)
	{
		swState = ESwState::Release;
		holdCount = 0;
	}
	else	//(prevPinState == SwOff && nowPinState == SwOff)
	{
		swState = ESwState::Off;
	}

	prevPinState = nowPinState;
	return swState;
}
