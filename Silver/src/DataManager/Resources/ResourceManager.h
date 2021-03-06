#pragma once
#include "Utils/Singleton.h"
#include "Libraries/ModelLibrary.h"
#include "Libraries/TextureLibrary.h"
#include "Libraries/ShaderLibrary.h"

namespace Silver {

	// TODO: Set all lib func return const T&
	class ResourceManager : public Singleton<ResourceManager>
	{
	public:
		ModelLibrary m_ModelLibrary;
		TextureLibrary m_TextureLibrary;
		ShaderLibrary m_ShaderLibrary;

	};

}