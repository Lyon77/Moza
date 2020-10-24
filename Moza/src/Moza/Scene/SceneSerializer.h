#pragma once

#include "Scene.h"

namespace Moza
{
	class SceneSerializer
	{
	public:
		SceneSerializer(const Ref<Scene>& scene);
		
		void Serialize(const std::string& filepath);  // Text
		void SerializeRuntime(const std::string& filepath);  // Binary

		bool Deserialize(const std::string& filepath);
		bool DeserializeRuntime(const std::string& filepath);
	private:
		Ref<Scene> m_Scene;
	};
}