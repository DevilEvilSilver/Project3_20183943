#pragma once

#include <string>
#include <vector>
#include <memory>
#include <tinyxml2.h>

namespace Silver {

	//data for skin loader
	class VertexSkinData
	{
	public:
		VertexSkinData() = default;
		~VertexSkinData();

		void AddJointEffect(unsigned int jointID, float weight);
		void LimitJointNumber(unsigned int max);

		const std::vector<int>& GetJointIDList() { return m_JointIDs; }
		const std::vector<float>& GetWeightList() { return m_Weight; }

	private:
		float SaveTopWeights(std::vector<float>& topWeightsArray, unsigned int max);
		void RefillWeightList(std::vector<float>topWeights, float total);
		void RemoveExcessJointIds(unsigned int max);
		void FillEmptyWeights(unsigned int max);

	private:
		std::vector<int> m_JointIDs;
		std::vector<float> m_Weight;
	};

	// CAN CURRENTLY APPLY TO ONLY MODEL WITH 1 MESH !!! 
	class SkinLoader
	{
	public:
		SkinLoader(tinyxml2::XMLElement* node, unsigned int maxWeightCount);
		~SkinLoader();

		const std::vector<std::string>& GetJointOrder() { return m_JointOrder; }
		const std::vector<std::shared_ptr<VertexSkinData>>& GetSkinData() { return m_verticeSkinDatas; }

	private:
		void LoadJointList(tinyxml2::XMLElement* skinningdata);
		void LoadWeights(tinyxml2::XMLElement* skinningdata, std::vector<float>& weights);
		void LoadEffectiveJointsCounts(tinyxml2::XMLElement* skinningdata, std::vector<unsigned int>& effectorJointCounts);
		void LoadSkinData(tinyxml2::XMLElement* skinningdata, std::vector<unsigned int>& counts, std::vector<float>& weights);

	private:
		std::vector<std::string> m_JointOrder;
		std::vector<std::shared_ptr<VertexSkinData>> m_verticeSkinDatas;
		unsigned int m_MaxWeightCount;
	};

}