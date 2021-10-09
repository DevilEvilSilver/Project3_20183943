#pragma once

namespace Silver {

	class Timer
	{
	public:
		Timer()
			: m_NewFrameTime(0.0f), m_LastFrameTime(0.0f)
		{
		}

		void UpdateTimestep();
		float GetTimestep() const { return m_NewFrameTime - m_LastFrameTime; }

	private:
		float m_NewFrameTime;
		float m_LastFrameTime;
	};

}