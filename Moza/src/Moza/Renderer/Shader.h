#pragma once

#include <string>

namespace Moza
{
	class MOZA_API Shader
	{
	public:
		virtual ~Shader() = default;

		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;

		virtual const std::string& GetName() const = 0;

		static Ref<Shader> Create(const std::string& filePath);
		static Ref<Shader> Create(const std::string& name, std::string & vertexSrc, const std::string & fragmentSrc);
	};

	class MOZA_API ShaderLibrary
	{
	public:
		void Add(const std::string& name, const Ref<Shader>& shader);
		void Add(const Ref<Shader>& shader);
		Ref<Shader> Load(const std::string& filePath);
		Ref<Shader> Load(const std::string& name, std::string& filePath);

		Ref<Shader> Get(const std::string& name);

		bool Exists(const std::string name) const;
	private:
		std::unordered_map<std::string, Ref<Shader>> m_Shaders;
	};
}