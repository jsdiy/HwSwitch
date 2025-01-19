#include <Arduino.h>
#include "HwSwitch.hpp"

HwSwitch	buttonSw;

void	setup(void)
{
	Serial.begin(115200);

	//GPIO-9にスイッチが接続されているとする
	//3秒押し続けたら「長押し」とする（HwSwitch::State()を1秒間隔で呼び出すとして）
	buttonSw.Initialize(GPIO_NUM_9, 3);
}

void	loop(void)
{
	ESwState swState = buttonSw.State();
	switch (swState)
	{
	case ESwState::On:	Serial.println("On");	break;
	case ESwState::ShortHold:	Serial.println("ShortHold");	break;
	case ESwState::LongHold:	Serial.println("LongHold");	break;
	case ESwState::Release:	Serial.println("Release");	break;
	case ESwState::Off:	Serial.println("Off");	break;
	default:	break;
	}
	delay(1000);
}
