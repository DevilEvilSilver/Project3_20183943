#include "pch.h"
#include "Shader.h"

#include <fstream>
#include <filesystem>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace Silver {

	static GLenum ShaderTypeFromString(const std::string& type)
	{
		if (type == "vertex")
			return GL_VERTEX_SHADER;
		if (type == "fragment")
			return GL_FRAGMENT_SHADER;

		SV_CORE_ERROR("Unknown shader type: {0}", type);
		return 0;
	}

	Shader::Shader(const std::string & filepath)
		: m_Directory(filepath)
	{
		std::string source = ReadFile(filepath);
		auto shaderSources = PreProcess(source);
		Compile(shaderSources);

		// Extract name from filepath
		std::filesystem::path path = filepath;
		m_Name = path.stem().string();
	}

	Shader::Shader(const std::string & name, const std::string & vertexSrc, const std::string & fragmentSrc)
		: m_Name(name), m_Directory("")
	{
		std::unordered_map<GLenum, std::string> shaderSources;
		shaderSources[GL_VERTEX_SHADER] = vertexSrc;
		shaderSources[GL_FRAGMENT_SHADER] = fragmentSrc;
		Compile(shaderSources);
	}

	Shader::~Shader()
	{
		glDeleteProgram(m_RendererID);
	}

	std::string Shader::ReadFile(const std::string& filepath)
	{
		std::string result;
		std::ifstream in(filepath, std::ios::in | std::ios::binary);
		if (in)
		{
			in.seekg(0, std::ios::end);
			result.resize(in.tellg());
			in.seekg(0, std::ios::beg);
			in.read(&result[0], result.size());
			in.close();

		}
		else
		{
			SV_CORE_ERROR("Could not oprn file {0}", filepath);
		}

		return result;
	}

	std::unordered_map<GLenum, std::string> Shader::PreProcess(const std::string& source)
	{
		std::unordered_map<GLenum, std::string> shaderSources;

		const char* typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);
		size_t pos = source.find(typeToken, 0);
		while (pos != std::string::npos)
		{
			size_t eol = source.find_first_of("\r\n", pos);
			if (eol == std::string::npos)
				SV_CORE_ERROR("Syntax error: {0}", source);
			size_t begin = pos + typeTokenLength + 1;
			std::string type = source.substr(begin, eol - begin);
			if (type != "vertex" && type != "fragment")
				SV_CORE_ERROR("Invalid shader type: {0}", source);

			size_t nextLinePos = source.find_first_not_of("\r\n", eol);
			pos = source.find(typeToken, nextLinePos);
			shaderSources[ShaderTypeFromString(type)] = source.substr(nextLinePos,
				pos - (nextLinePos == std::string::npos ? source.size() - 1 : nextLinePos));
		}

		return shaderSources;
	}

	void Shader::Compile(std::unordered_map<GLenum, std::string> shaderSources)
	{
		GLuint program = glCreateProgram();
		if (shaderSources.size() > 2)
			SV_CORE_ERROR("Engine only support upto 2 shaders");
		std::array<GLenum, 2> glShaderIDs;
		int glShaderIDIndex = 0;
		for (auto&& [key,value] : shaderSources)
		{
			GLenum type = key;
			GLuint shader = glCreateShader(type);

			// Note that std::string's .c_str is NULL character terminated.
			const GLchar *source = value.c_str();
			glShaderSource(shader, 1, &source, 0);

			// Compile the shader
			glCompileShader(shader);

			GLint isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

				// The maxLength includes the NULL character
				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

				// We don't need the shader anymore.
				glDeleteShader(shader);

				SV_CORE_ERROR("Shader compilation failed: {0}", infoLog.data());

				// In this simple program, we'll just leave
				return;
			}

			glAttachShader(program, shader);
			glShaderIDs[glShaderIDIndex++] = shader;
		}

		m_RendererID = program;

		// Link our program
		glLinkProgram(program);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int *)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(program);
			// Don't leak shaders either.
			for (auto id : glShaderIDs)
				glDeleteShader(id);

			SV_CORE_ERROR("Shader link failed: {0}", infoLog.data());

			// In this simple program, we'll just leave
			return;
		}

		// Always detach shaders after a successful link.
		for (auto id : glShaderIDs)
			glDetachShader(program, id);
	}

	void Shader::Bind()
	{
		glUseProgram(m_RendererID);
	}

	void Shader::Unbind()
	{
		glUseProgram(0);
	}

	void Shader::SubmitUniformInt(const std::string & name, int value)
	{
		GLint location = (glGetUniformLocation(m_RendererID, name.c_str()));
		glUniform1i(location, value);
	}

	void Shader::SubmitUniformFloat(const std::string & name, float value)
	{
		GLint location = (glGetUniformLocation(m_RendererID, name.c_str()));
		glUniform1f(location, value);
	}

	void Shader::SubmitUniformFloat2(const std::string & name, const glm::vec2 & value)
	{
		GLint location = (glGetUniformLocation(m_RendererID, name.c_str()));
		glUniform2f(location, value.x , value.y);
	}

	void Shader::SubmitUniformFloat3(const std::string & name, const glm::vec3 & value)
	{
		GLint location = (glGetUniformLocation(m_RendererID, name.c_str()));
		glUniform3f(location, value.x, value.y, value.z);
	}

	void Shader::SubmitUniformFloat4(const std::string & name, const glm::vec4 & value)
	{
		GLint location = (glGetUniformLocation(m_RendererID, name.c_str()));
		glUniform4f(location, value.x, value.y, value.z, value.w);
	}

	void Shader::SubmitUniformMat3(const std::string & name, const glm::mat3 & matrix)
	{
		GLint location = (glGetUniformLocation(m_RendererID, name.c_str()));
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void Shader::SubmitUniformMat4(const std::string& name, const glm::mat4 & matrix)
	{
		GLint location = (glGetUniformLocation(m_RendererID, name.c_str()));
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void Shader::SubmitUniformMat4Array(const std::string& name, const std::vector<glm::mat4>& matrices)
	{
		//GLint location = (glGetUniformLocation(m_RendererID, name.c_str()));
		//glUniformMatrix4fv(location, matrices.size(), GL_FALSE, glm::value_ptr(matrices[1]));
		for (unsigned int i = 0; i < matrices.size(); ++i)
		{
			SubmitUniformMat4(name + "[" + std::to_string(i) + "]", matrices[i]);
		}
	}

}