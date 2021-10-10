#pragma once

#include <string>
#include <glm/glm.hpp>

namespace Silver {

	class Shader
	{
	public:
		Shader(const std::string& vertexSrc, const std::string& fragmentSrc);
		~Shader();

		void Bind();
		void Unbind();

		void SubmitUniformInt(const std::string& name, int value);

		void SubmitUniformFloat(const std::string& name, float value);
		void SubmitUniformFloat2(const std::string& name, const glm::vec2& value);
		void SubmitUniformFloat3(const std::string& name, const glm::vec3& value);
		void SubmitUniformFloat4(const std::string& name, const glm::vec4& value);

		void SubmitUniformMat3(const std::string& name, const glm::mat3& matrix);
		void SubmitUniformMat4(const std::string& name, const glm::mat4& matrix);

	private:
		uint32_t m_RendererID;
	};

}