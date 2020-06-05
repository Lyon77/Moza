#pragma once

#include "Moza/Core/Core.h"
#include "Moza/Renderer/Shader.h"
#include "Moza/Renderer/Texture.h"

#include <unordered_set>

namespace Moza
{
	class Material
	{
		friend class MaterialInstance;
	public:
		Material(const Ref <Shader> & shader);
		virtual ~Material() = default;

		void Bind() const;

		template <typename T>
		void Set(const std::string& name, const T& value)
		{
			auto decl = FindUniformDeclaration(name);
			MZ_CORE_ASSERT(decl, "Could not find uniform with name '{0}'", name);
			//MZ_CORE_ASSERT(decl, "Could not find uniform with name 'x'");
			auto& buffer = GetUniformBufferTarget(decl);
			buffer.Write((unsigned char*)&value, decl->GetSize(), decl->GetOffset());

			for (auto mi : m_MaterialInstances)
				mi->OnMaterialValueUpdated(decl);
		}

		void Set(const std::string& name, const Ref<Texture>& texture)
		{
			auto decl = FindResourceDeclaration(name);
			uint32_t slot = decl->GetRegister();
			if (m_Textures.size() <= slot)
				m_Textures.resize((size_t)slot + 1);
			m_Textures[slot] = texture;
		}

		void Set(const std::string& name, const Ref<Texture2D>& texture)
		{
			Set(name, (const Ref<Texture>&)texture);
		}

		void Set(const std::string& name, const Ref<TextureCube>& texture)
		{
			Set(name, (const Ref<Texture>&)texture);
		}

	private:
		void BindTextures() const;

		void AllocateStorage();
		void OnShaderReloaded();

		ShaderUniformDeclaration* FindUniformDeclaration(const std::string& name);
		ShaderResourceDeclaration* FindResourceDeclaration(const std::string& name);
		Buffer& GetUniformBufferTarget(ShaderUniformDeclaration* uniformDeclaration);

		Ref<Shader> m_Shader;
		std::unordered_set<MaterialInstance*> m_MaterialInstances;

		Buffer m_VSUniformStorageBuffer;
		Buffer m_PSUniformStorageBuffer;
		std::vector<Ref<Texture>> m_Textures;

		int32_t m_RenderFlags = 0;
	};

	class MaterialInstance
	{
		friend class Material;
	public:
		MaterialInstance(const Ref<Material>& material);
		virtual ~MaterialInstance() = default;

		void Bind() const;

		template <typename T>
		void Set(const std::string& name, const T& value)
		{
			auto decl = m_Material->FindUniformDeclaration(name);
			// HZ_CORE_ASSERT(decl, "Could not find uniform with name '{0}'", name);
			MZ_CORE_ASSERT(decl, "Could not find uniform with name 'x'");
			auto& buffer = GetUniformBufferTarget(decl);
			buffer.Write((unsigned char*)&value, decl->GetSize(), decl->GetOffset());

			m_OverriddenValues.insert(name);
		}

		void Set(const std::string& name, const Ref<Texture>& texture)
		{
			auto decl = m_Material->FindResourceDeclaration(name);
			uint32_t slot = decl->GetRegister();
			if (m_Textures.size() <= slot)
				m_Textures.resize((size_t)slot + 1);
			m_Textures[slot] = texture;
		}

		void Set(const std::string& name, const Ref<Texture2D>& texture)
		{
			Set(name, (const Ref<Texture>&)texture);
		}

		void Set(const std::string& name, const Ref<TextureCube>& texture)
		{
			Set(name, (const Ref<Texture>&)texture);
		}
	private:
		void AllocateStorage();
		void OnShaderReloaded();
		Buffer& GetUniformBufferTarget(ShaderUniformDeclaration* uniformDeclaration);
		void OnMaterialValueUpdated(ShaderUniformDeclaration* decl);

		Ref<Material> m_Material;

		Buffer m_VSUniformStorageBuffer;
		Buffer m_PSUniformStorageBuffer;
		std::vector<Ref<Texture>> m_Textures;

		// TODO: This is temporary; come up with a proper system to track overrides
		std::unordered_set<std::string> m_OverriddenValues;

	};
}