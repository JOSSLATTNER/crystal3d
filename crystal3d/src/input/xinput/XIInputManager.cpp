#include "XIInputManager.h"

namespace Input
{
	namespace XInput
	{
		XIInputManager::XIInputManager()
		{
		}

		XIInputManager::~XIInputManager()
		{

		}

		void XIInputManager::Initialize()
		{
			CrLogSuccess("InputManager initialized [OK]");
		}

		bool XIInputManager::IsKeyPressed(KeyboardInput::EKeyCode a_KeyCode)
		{
			return GetAsyncKeyState(a_KeyCode) & 0x8000;
		}

		double_t XIInputManager::GetAxis(uint32_t a_ControllerID, ControllerInput::EAxis a_Axis)
		{
			if (m_InputStates.find(a_ControllerID) != m_InputStates.end())
			{
				XINPUT_STATE state = m_InputStates[a_ControllerID];

				switch (a_Axis)
				{
				case Input::ControllerInput::LEFT_X:
					return this->NormalizeAxis(state.Gamepad.sThumbLX);
				case Input::ControllerInput::LEFT_Y:
					return this->NormalizeAxis(state.Gamepad.sThumbLY);
				case Input::ControllerInput::RIGHT_X:
					return this->NormalizeAxis(state.Gamepad.sThumbRX);
				case Input::ControllerInput::RIGHT_Y:
					return this->NormalizeAxis(state.Gamepad.sThumbRY);
				}
			}
			return 0.0f;
		}

		bool XIInputManager::IsButtonPressed(uint32_t a_ControllerID, ControllerInput::EButtons a_Button)
		{
			if (m_InputStates.find(a_ControllerID) != m_InputStates.end())
			{
				XINPUT_STATE state = m_InputStates[a_ControllerID];
				WORD btn = m_Mappings.at(a_Button);

				return Util::check_bit(state.Gamepad.wButtons, btn);
			}
			return false;
		}

		void XIInputManager::Poll()
		{
			m_InputStates.clear();

			for (DWORD i = 0; i < XUSER_MAX_COUNT; i++)
			{
				XINPUT_STATE state{};

				if (XInputGetState(i, &state) == ERROR_SUCCESS)
				{
					m_InputStates[i] = state;
				}
			}
		}

		std::vector<uint32_t> XIInputManager::ListDevices()
		{
			std::vector<uint32_t> devices;
			for (auto& pair : m_InputStates)
				devices.emplace_back(pair.first);
			return devices;
		}

		double_t XIInputManager::NormalizeAxis(int32_t a_Value)
		{
			return 2 * ((double_t)a_Value - AXIS_MIN) / (AXIS_MAX - AXIS_MIN) - 1;
		}
	}
}