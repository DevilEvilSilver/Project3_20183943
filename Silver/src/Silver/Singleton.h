#pragma once
#include <memory>

namespace Silver {

	template <class T>
	class SILVER_API Singleton
	{
	protected:
		Singleton() {}
		~Singleton() {}
		/*implicit methods exist for the copy constructor and operator= and we want to forbid calling them.*/
		Singleton(const Singleton &) = delete;
		Singleton& operator =(const Singleton &) = delete;

	public:
		static void CreateInstance()
		{
			if (s_Instance == nullptr)
				s_Instance = new T();
		}

		static T& GetInstance()
		{
			return s_Instance;
		}

		static void ResetInstance() {
			SAFE_DELETE(s_Instance);
		}

	protected:
		static T* s_Instance;
	};

	template <class T> T* Singleton<T>::s_Instance = nullptr;

}