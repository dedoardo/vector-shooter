/**
==========================================================================
						ControllerXBox
==========================================================================
**/
#include "ControllerXBox.h"


/**=============================
ControllerXBox::ControllerXBox
=============================**/
ControllerXBox::~ControllerXBox()
{
	StopAllVibrations();
}

/**=============================
ControllerXBox::ControllerXBox
=============================**/
ControllerXBox::ControllerXBox(int playerNumber)
{
	ControllerNum_ = playerNumber - 1;
	MAX_THUMB_VALUE = 32768;
	MIN_THUMB_VALUE = -32768;
	MAX_VIBRATION_VALUE = 65535;
}

/**=============================
ControllerXBox::IsConnected
=============================**/
bool ControllerXBox::IsConnected()
{
	ZeroMemory(&ControllerState_,sizeof(XINPUT_STATE));

	DWORD result = XInputGetState(ControllerNum_,&ControllerState_);

	if (result == ERROR_SUCCESS)
		return true;
	return false;
}

/**=============================
ControllerXBox::Vibrate
=============================**/
void ControllerXBox::Vibrate(unsigned int leftPower,unsigned int rightPower)
{
	XINPUT_VIBRATION vibration;

	ZeroMemory(&vibration,sizeof(XINPUT_VIBRATION));

	vibration.wLeftMotorSpeed = leftPower;
	vibration.wRightMotorSpeed = rightPower;

	XInputSetState(ControllerNum_,&vibration);
}

/**=============================
ControllerXBox::VibrateRight
=============================**/
void ControllerXBox::VibrateRight(unsigned int rightPower)
{
	XINPUT_VIBRATION vibration;

	ZeroMemory(&vibration,sizeof(XINPUT_VIBRATION));

	vibration.wRightMotorSpeed = rightPower;

	XInputSetState(ControllerNum_,&vibration);

}

/**=============================
ControllerXBox::VibrateRight
=============================**/
void ControllerXBox::VibrateLeft(unsigned int leftPower)
{
	XINPUT_VIBRATION vibration;

	ZeroMemory(&vibration,sizeof(XINPUT_VIBRATION));

	vibration.wLeftMotorSpeed = leftPower;

	XInputSetState(ControllerNum_,&vibration);

}

/**=============================
ControllerXBox::StopAllVibrations
=============================**/
void ControllerXBox::StopAllVibrations()
{
	XINPUT_VIBRATION vibration;

	ZeroMemory(&vibration,sizeof(XINPUT_VIBRATION));

	vibration.wLeftMotorSpeed = 0;
	vibration.wRightMotorSpeed = 0;

	XInputSetState(ControllerNum_,&vibration);

}

/**=============================
ControllerXBox::GetState
=============================**/
XINPUT_STATE ControllerXBox::GetState()
{
	ZeroMemory(&ControllerState_,sizeof(XINPUT_STATE));

	XInputGetState(ControllerNum_,&ControllerState_);

	return ControllerState_;


}