#pragma once
#include "Mappings.h"

namespace Input
{
	class IInputManager
	{
	public:
		virtual ~IInputManager() {};
		virtual void Initialize() = 0;
		virtual void Poll() = 0;

		/*Keyboard*/
		virtual bool IsKeyPressed(KeyboardInput::EKeyCode a_KeyCode) = 0;

		/*Controller*/
		virtual bool IsButtonPressed(uint32_t a_ControllerID, ControllerInput::EButtons a_Button) = 0;
		virtual double_t GetAxis(uint32_t a_ControllerID, ControllerInput::EAxis a_Axis) = 0;
		virtual std::vector<uint32_t> ListDevices() = 0;

	};
}