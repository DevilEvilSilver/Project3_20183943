#pragma once

#include "../Model/Model.h"

#include <glm/glm.hpp>
#include <memory>
#include <vector>

namespace Silver {

	class Animator
	{
	public:
		Animator();
		~Animator() = default;

		void BindAnimation(const std::shared_ptr<Animation>& animtion);
		void UnbindAnimation();
		void OnUpdate(float deltaTime);
		void ApplyPoseToJoints(std::shared_ptr<Joint>& joint, glm::mat4& parentTransform);
		bool HasAnimation() { return (m_CurrAnimation != nullptr && m_CurrAnimation->HasAnimations()); }

		const std::shared_ptr<Animation>& GetCurrAnimation() { return m_CurrAnimation; }

	private:
		glm::mat4 GetCurrLocalTransform(const std::shared_ptr<Joint>& joint);
		void IncreaseCurrentTime(float deltaTime);
		void CalculateCurrentAnimationPose(float deltaTime);
		glm::mat4 Interpolate(std::shared_ptr<KeyFrame> prev, std::shared_ptr<KeyFrame> next, float progression);

	private:
		std::shared_ptr<Animation> m_CurrAnimation = nullptr;
		std::unordered_map<std::string, glm::mat4> m_CurrPose;
		float m_CurrTime = 0.0f;
		
	};

}