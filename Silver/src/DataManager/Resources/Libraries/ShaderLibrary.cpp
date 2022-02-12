#include "pch.h"
#include "ShaderLibrary.h"

namespace Silver {

	ShaderLibrary::ShaderLibrary()
	{
		std::string vertexSrc2 = R"(
				#version 330 core

				layout(location = 0) in vec3 a_Position;

				uniform	mat4 u_ViewProjection;
				uniform	mat4 u_World;

				out vec3 v_Position;
				out vec4 v_Color;

				void main()
				{
					gl_Position = u_ViewProjection * u_World * vec4(a_Position, 1.0);
					v_Position = a_Position;
				}

			)";

		std::string fragmentSrc2 = R"(
				#version 330 core

				layout(location = 0) out vec4 color;
			
				uniform vec3 u_Color;

				in vec3 v_Position;
				in vec4 v_Color;

				void main()
				{
					color = vec4(u_Color, 1.0);
				}

			)";

		Add(std::make_shared<Silver::Shader>(DEFAULT_SHADER, vertexSrc2, fragmentSrc2));
	}

	void ShaderLibrary::Add(const std::shared_ptr<Shader>& shader)
	{
		auto& name = shader->GetName();
		Add(name, shader);
	}

	void ShaderLibrary::Add(const std::string& name, const std::shared_ptr<Shader>& shader)
	{
		if (IsExist(name))
			SV_CORE_WARN("Shader {0} already exist in ShaderLibrary !!!", name);
		m_Shaders[name] = shader;
	}

	std::shared_ptr<Shader> ShaderLibrary::Load(const std::string& filepath)
	{
		auto shader = std::make_shared<Shader>(filepath);
		Add(shader);
		return shader;
	}

	std::shared_ptr<Shader> ShaderLibrary::Load(const std::string& name, const std::string& filepath)
	{
		auto shader = std::make_shared<Shader>(filepath);
		Add(name, shader);
		return shader;
	}

	std::shared_ptr<Shader> ShaderLibrary::Get(const std::string& name)
	{
		if (!IsExist(name))
			SV_CORE_ERROR("Shader {0} does not exist in ShaderLibrary !!!", name);
		return m_Shaders[name];
	}

	bool ShaderLibrary::IsExist(const std::string& name) const
	{
		return m_Shaders.find(name) != m_Shaders.end();
	}

}