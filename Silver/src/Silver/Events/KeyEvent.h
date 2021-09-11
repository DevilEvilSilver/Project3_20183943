#pragma once
#include "Event.h"

namespace Silver {

	class KeyEvent : public Event
	{
	public:
		int GetKeyCode() const { return m_KeyCode; }

	protected:
		KeyEvent(int keycode)
			: m_KeyCode(keycode) {}

		int m_KeyCode;
	};

	class KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(int keycode, int repeatCount)
			: KeyEvent(keycode) {}

		int GetRepeatCount() const { return m_RepeatCount; }
		
		EVENT_CLASS_TYPE(KeyPressed)

	private:
		int m_RepeatCount;
	};

	class KeyTypedEvent : public KeyEvent
	{
	public:
		KeyTypedEvent(int keycode)
			: KeyEvent(keycode) {}

		EVENT_CLASS_TYPE(KeyTyped)
	};

	class KeyReleaseEvent : public KeyEvent
	{
	public:
		KeyReleaseEvent(int keycode)
			: KeyEvent(keycode) {}

		EVENT_CLASS_TYPE(KeyReleased)
	};

}