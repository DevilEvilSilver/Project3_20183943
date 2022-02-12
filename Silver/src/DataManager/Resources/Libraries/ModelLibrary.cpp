#include "pch.h"
#include "ModelLibrary.h"

namespace Silver {
	ModelLibrary::ModelLibrary()
	{
		float vertices[3 * 4] = {
				-1.0f, -1.0f, 0.0f,
				1.0f, -1.0f, 0.0f,
				1.0f, 1.0f, 0.0f,
				-1.0f, 1.0f, 0.0f

		};
		std::shared_ptr<Silver::VertexBuffer> squareVB;
		squareVB = std::make_shared<Silver::VertexBuffer>(vertices, sizeof(vertices));
		squareVB->SetLayout({
			{ Silver::DataType::Float3, "a_Position"},
			});
		float normals[2 * 4] = {
			0.0f, 0.0f,
			1.0f, 0.0f,
			1.0f, 1.0f,
			0.0f, 1.0f

		};
		std::shared_ptr<Silver::VertexBuffer> squareNormal;
		squareNormal = std::make_shared<Silver::VertexBuffer>(normals, sizeof(normals));
		squareNormal->SetLayout({
			{ Silver::DataType::Float2, "a_TexCoord"}
			});
		std::vector<std::shared_ptr<Silver::VertexBuffer>> VBList;
		VBList.push_back(squareVB);
		VBList.push_back(squareNormal);

		unsigned int indices[2 * 3] = {
			0, 1, 2,
			0, 2, 3
		};
		std::shared_ptr<Silver::IndexBuffer> squareIB;
		squareIB = std::make_shared<Silver::IndexBuffer>(indices, std::size(indices));

		auto squareMesh = std::make_shared<Silver::Mesh>(VBList, squareIB);

		std::vector<std::shared_ptr<Silver::Mesh>> meshes;
		meshes.push_back(squareMesh);
		Add(std::make_shared<Silver::StaticModel>(DEFAULT_STATIC_MODEL, meshes));
		Add(std::make_shared<Silver::AnimatedModel>(DEFAULT_ANIMATED_MODEL, meshes));
	}
	void ModelLibrary::Add(const std::shared_ptr<Model>& model)
	{
		auto& name = model->GetName();
		Add(name, model);
	}

	void ModelLibrary::Add(const std::string& name, const std::shared_ptr<Model>& model)
	{
		if (IsExist(name))
			SV_CORE_WARN("Model {0} already exist in ModelLibrary !!!", name);
		m_Models[name] = model;
	}

	std::shared_ptr<StaticModel> ModelLibrary::LoadStatic(const std::string& filepath)
	{
		auto model = std::make_shared<StaticModel>(filepath);
		Add(model);
		return model;
	}

	std::shared_ptr<StaticModel> ModelLibrary::LoadStatic(const std::string& name, const std::string& filepath)
	{
		auto model = std::make_shared<StaticModel>(filepath);
		Add(name, model);
		return model;
	}

	std::shared_ptr<AnimatedModel> ModelLibrary::LoadAnimated(const std::string& filepath)
	{
		auto model = std::make_shared<AnimatedModel>(filepath);
		Add(model);
		return model;
	}

	std::shared_ptr<AnimatedModel> ModelLibrary::LoadAnimated(const std::string& name, const std::string& filepath)
	{
		auto model = std::make_shared<AnimatedModel>(filepath);
		Add(name, model);
		return model;
	}

	std::shared_ptr<Model> ModelLibrary::Get(const std::string& name)
	{
		if (!IsExist(name))
			SV_CORE_ERROR("Model {0} does not exist in ModelLibrary !!!", name);
		return m_Models[name];
	}

	bool ModelLibrary::IsExist(const std::string& name) const
	{
		return m_Models.find(name) != m_Models.end();
	}

}