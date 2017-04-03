#pragma once
#include "core\Core.h"
#include "core\Platform.h"
#include "input\interface\IInputManager.h"

#include <Xinput.h>

namespace Input
{
	namespace XInput
	{
		class XIInputManager
			: public IInputManager
		{
		public:
			static constexpr double_t AXIS_MAX = 32768.;
			static constexpr double_t AXIS_MIN = -32768.;

			XIInputManager();

			// Inherited via IInputManager
			~XIInputManager() override;
			void Initialize() override;
			void Poll() override;
			bool IsKeyPressed(KeyboardInput::EKeyCode a_KeyCode) override;
			double_t GetAxis(uint32_t a_ControllerID, ControllerInput::EAxis a_Axis) override;
			bool IsButtonPressed(uint32_t a_ControllerID, ControllerInput::EButtons a_Button) override;
			std::vector<uint32_t> ListDevices() override;

		private:
			double_t NormalizeAxis(int32_t a_Value);

			std::unordered_map<uint32_t, XINPUT_STATE> m_InputStates;
			const std::unordered_map<ControllerInput::EButtons, WORD> m_Mappings
			{
				{ ControllerInput::A, XINPUT_GAMEPAD_A },
				{ ControllerInput::B, XINPUT_GAMEPAD_B },
				{ ControllerInput::X, XINPUT_GAMEPAD_X },
				{ ControllerInput::Y, XINPUT_GAMEPAD_Y },
				{ ControllerInput::DPAD_LEFT, XINPUT_GAMEPAD_DPAD_LEFT },
				{ ControllerInput::DPAD_RIGHT, XINPUT_GAMEPAD_DPAD_RIGHT },
				{ ControllerInput::DPAD_UP, XINPUT_GAMEPAD_DPAD_UP },
				{ ControllerInput::DPAD_DOWN, XINPUT_GAMEPAD_DPAD_DOWN },
				{ ControllerInput::LEFT_THUMB, XINPUT_GAMEPAD_LEFT_THUMB },
				{ ControllerInput::RIGHT_THUMB, XINPUT_GAMEPAD_RIGHT_THUMB },
				{ ControllerInput::LEFT_SHOULDER, XINPUT_GAMEPAD_LEFT_SHOULDER},
				{ ControllerInput::RIGHT_SHOULDER, XINPUT_GAMEPAD_RIGHT_SHOULDER },
				{ ControllerInput::START, XINPUT_GAMEPAD_START },
				{ ControllerInput::BACK, XINPUT_GAMEPAD_BACK },
			};

		};
	}
}