#pragma once
#include <vector>
#include <string>

namespace Silver {

	enum class DataType
	{
		None = 0, Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Bool
	};

	static unsigned int DataTypeSize(DataType type)
	{
		switch (type)
		{
		case DataType::Float:	return 4;
		case DataType::Float2:	return 4 * 2;
		case DataType::Float3:	return 4 * 3;
		case DataType::Float4:	return 4 * 4;
		case DataType::Mat3:	return 4 * 9;
		case DataType::Mat4:	return 4 * 16;
		case DataType::Int:		return 4;
		case DataType::Int2:	return 4 * 2;
		case DataType::Int3:	return 4 * 3;
		case DataType::Int4:	return 4 * 4;
		case DataType::Bool:	return 1;
		}

		SV_CORE_ERROR("Unknown DataType: Can't calculate data size!");
		return 0;
	}

	struct VertexAttribute
	{
		std::string name;
		DataType type;
		unsigned int size;
		__int64 offset;	// will be cast to void* later
		bool normalized = false;

		VertexAttribute() {}

		VertexAttribute(DataType typ, const std::string& strName)
			: name(strName), type(typ), size(DataTypeSize(typ)), offset(0)
		{

		}

		unsigned int GetComponentCount() const
		{
			switch (type)
			{
			case DataType::Float:	return 1;
			case DataType::Float2:	return 2;
			case DataType::Float3:	return 3;
			case DataType::Float4:	return 4;
			case DataType::Mat3:	return 9;
			case DataType::Mat4:	return 16;
			case DataType::Int:		return 1;
			case DataType::Int2:	return 2;
			case DataType::Int3:	return 3;
			case DataType::Int4:	return 4;
			case DataType::Bool:	return 1;
			}

			SV_CORE_ERROR("Unknown DataType: Can't get component count!");
			return 0;
		}
	};

	class VertexLayout 
	{
	public:
		VertexLayout() {};

		VertexLayout(const std::initializer_list<VertexAttribute>& list)
			: m_AttributeList(list)
		{
			InitAttribute();
		}

		unsigned int GetStride() const { return m_Stride; }
		const std::vector<VertexAttribute>& GetLayout() const { return m_AttributeList; }

		std::vector<VertexAttribute>::iterator begin() { return m_AttributeList.begin(); }
		std::vector<VertexAttribute>::iterator end() { return m_AttributeList.end(); }
		std::vector<VertexAttribute>::const_iterator begin() const { return m_AttributeList.begin(); }
		std::vector<VertexAttribute>::const_iterator end() const { return m_AttributeList.end(); }

	private:
		void InitAttribute()
		{
			unsigned int offset = 0;
			m_Stride = 0;
			for (auto& attrib : m_AttributeList)
			{
				attrib.offset = offset;
				offset += attrib.size;
				m_Stride += attrib.size;
			}
		}

	private:
		std::vector<VertexAttribute> m_AttributeList;
		unsigned int m_Stride = 0;
	};

	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void SetLayout(const VertexLayout& layout) = 0;
		virtual const VertexLayout& GetLayout() const = 0;

		static VertexBuffer* Create(float* vertices, unsigned int size);
	};

	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual unsigned int GetCount() const = 0;

		static IndexBuffer* Create(unsigned int * indices, unsigned int count);
	};

}
