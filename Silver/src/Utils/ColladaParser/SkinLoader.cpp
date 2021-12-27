#include "pch.h"
#include "SkinLoader.h"

namespace Silver {

	SkinLoader::SkinLoader(tinyxml2::XMLElement* node, unsigned int maxWeightCount)
		:m_MaxWeightCount(maxWeightCount)
	{
		tinyxml2::XMLElement* skinningdata = node
			->FirstChildElement("controller")
			->FirstChildElement("skin");

		LoadJointList(skinningdata);
		std::vector<float> weights;
		LoadWeights(skinningdata, weights);
		std::vector<unsigned int> effectorJointCounts;
		LoadEffectiveJointsCounts(skinningdata, effectorJointCounts);
		LoadSkinData(skinningdata, effectorJointCounts, weights);
	}

	SkinLoader::~SkinLoader()
	{
		m_JointOrder.clear();
		m_verticeSkinDatas.clear(); // vector of obj (shared_ptr) !!!
	}

	void SkinLoader::LoadJointList(tinyxml2::XMLElement* skinningdata)
	{
		tinyxml2::XMLElement* input = skinningdata
			->FirstChildElement("vertex_weights")
			->FirstChildElement("input");

		// Get source ID
		std::string jointDataId;
		while (input != NULL) {
			if (std::string(input->Attribute("semantic")) == "JOINT") {
				std::string jointDataSource = input->Attribute("source");
				jointDataId = jointDataSource.substr(1);
				break;
			}
			input = input->NextSiblingElement("input");
		}
		// Iterate through source elements 
		tinyxml2::XMLElement* source = skinningdata->FirstChildElement("source");
		while (source != NULL) {
			// Get joints data
			if (std::string(source->Attribute("id")) == jointDataId) {
				std::string jointsData = source->FirstChildElement("Name_array")->GetText();
				unsigned int numJoints;
				source->FirstChildElement("Name_array")->QueryUnsignedAttribute("count", &numJoints);
				for (unsigned int i = 0; i < numJoints; i++)
				{
					m_JointOrder.push_back(jointsData.substr(0, jointsData.find(" ")));
					jointsData.erase(0, jointsData.find(" ") + 1);
				}
			}
			source = source->NextSiblingElement("source");
		}
	}

	void SkinLoader::LoadWeights(tinyxml2::XMLElement* skinningdata, std::vector<float>& weights)
	{
		tinyxml2::XMLElement* input = skinningdata
			->FirstChildElement("vertex_weights")
			->FirstChildElement("input");

		// Get source ID
		std::string weightDataId;
		while (input != NULL) {
			if (std::string(input->Attribute("semantic")) == "WEIGHT") {
				std::string weightDataSource = input->Attribute("source");
				weightDataId = weightDataSource.substr(1);
				break;
			}
			input = input->NextSiblingElement("input");
		}
		// Iterate through source elements 
		tinyxml2::XMLElement* source = skinningdata->FirstChildElement("source");
		while (source != NULL) {
			// Get joints data
			if (std::string(source->Attribute("id")) == weightDataId) {
				std::string weightsData = source->FirstChildElement("float_array")->GetText();
				unsigned int numWeights;
				source->FirstChildElement("float_array")->QueryUnsignedAttribute("count", &numWeights);
				for (unsigned int i = 0; i < numWeights; i++)
				{
					weights.push_back(std::stof(weightsData.substr(0, weightsData.find(" "))));
					weightsData.erase(0, weightsData.find(" ") + 1);
				}
			}
			source = source->NextSiblingElement("source");
		}
	}

	void SkinLoader::LoadEffectiveJointsCounts(tinyxml2::XMLElement* skinningdata, std::vector<unsigned int>& effectorJointCounts)
	{
		std::string rawData = skinningdata
			->FirstChildElement("vertex_weights")
			->FirstChildElement("vcount")->GetText();
		size_t pos;
		while ((pos = rawData.find(" ")) != std::string::npos)
		{
			effectorJointCounts.push_back(std::stoi(rawData.substr(0, pos)));
			rawData.erase(0, pos + 1);
		}
		//effectorJointCounts.push_back(std::stoi(rawData.substr(0, pos)));
	}

	void SkinLoader::LoadSkinData(tinyxml2::XMLElement* skinningdata, std::vector<unsigned int>& counts, std::vector<float>& weights)
	{
		std::string rawData = skinningdata
			->FirstChildElement("vertex_weights")
			->FirstChildElement("v")->GetText();
		for (unsigned int count : counts) {
			std::shared_ptr<VertexSkinData> skinData = std::make_shared<VertexSkinData>();
			for (unsigned int i = 0; i < count; i++) {
				unsigned int jointId = std::stoi(rawData.substr(0, rawData.find(" ")));
				rawData.erase(0, rawData.find(" ") + 1);
				unsigned int weightId = std::stoi(rawData.substr(0, rawData.find(" ")));
				rawData.erase(0, rawData.find(" ") + 1);
				skinData->AddJointEffect(jointId, weights[weightId]);
			}
			skinData->LimitJointNumber(m_MaxWeightCount);
			m_verticeSkinDatas.push_back(skinData);
		}
	}

	VertexSkinData::~VertexSkinData()
	{
		m_JointIDs.clear();
		m_Weight.clear();
	}

	void VertexSkinData::AddJointEffect(unsigned int jointID, float weight)
	{
		// add in order
		for (unsigned int i = 0; i < m_Weight.size(); i++) {
			if (weight > m_Weight[i]) {
				m_JointIDs.insert(m_JointIDs.begin() + i, jointID);
				m_Weight.insert(m_Weight.begin() + i, weight);
				return;
			}
		}
		m_JointIDs.push_back(jointID);
		m_Weight.push_back(weight);
	}

	void VertexSkinData::LimitJointNumber(unsigned int max)
	{
		std::sort(m_Weight.begin(), m_Weight.end(), 
			[](float a, float b)
		{
			return a > b;
		});
		if (m_JointIDs.size() > max) {
			std::vector<float> topWeights;
			topWeights.reserve(max);
			float total = SaveTopWeights(topWeights, max);
			RefillWeightList(topWeights, total);
			RemoveExcessJointIds(max);
		}
		else if (m_JointIDs.size() < max) {
			FillEmptyWeights(max);
		}
	}

	float VertexSkinData::SaveTopWeights(std::vector<float>& topWeightsArray, unsigned int max)
	{
		float total = 0;
		for (unsigned int i = 0; i < max; i++) {
			topWeightsArray.push_back(m_Weight[i]);
			total += topWeightsArray[i];
		}
		return total;
	}

	void VertexSkinData::RefillWeightList(std::vector<float> topWeights, float total)
	{
		m_Weight.clear();
		for (int i = 0; i < topWeights.size(); i++) {
			m_Weight.push_back(std::min<float>(topWeights[i] / total, 1));
		}
	}

	void VertexSkinData::RemoveExcessJointIds(unsigned int max)
	{
		while (m_JointIDs.size() > max) {
			m_JointIDs.pop_back();
		}
	}

	void VertexSkinData::FillEmptyWeights(unsigned int max)
	{
		while (m_JointIDs.size() < max) {
			m_JointIDs.push_back(0);
			m_Weight.push_back(0.0f);
		}
	}

}