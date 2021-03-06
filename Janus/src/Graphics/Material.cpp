#include "jnpch.h"

#include "Graphics/Material.h"
#include "Graphics/Light.h"

namespace Janus
{

	Ref<Material> Material::Create(const Ref<Shader> &shader, const std::string& name)
	{
		return Ref<Material>::Create(shader, name);
	}

	Material::Material(const Ref<Shader> &shader, const std::string& name) : m_Shader(shader), m_Name(name)
	{
		AllocateStorage();
		m_MaterialFlags |= (uint32_t)MaterialFlag::DepthTest;
		m_MaterialFlags |= (uint32_t)MaterialFlag::Blend;
	}

	void Material::AllocateStorage()
	{
		if (m_Shader->HasVSMaterialUniformBuffer())
		{
			const auto &vsBuffer = m_Shader->GetVSMaterialUniformBuffer();
			m_VSUniformStorageBuffer.Allocate(vsBuffer.GetSize());
			m_VSUniformStorageBuffer.ZeroInitialize();
		}

		if (m_Shader->HasPSMaterialUniformBuffer())
		{
			const auto &psBuffer = m_Shader->GetPSMaterialUniformBuffer();
			m_PSUniformStorageBuffer.Allocate(psBuffer.GetSize());
			m_PSUniformStorageBuffer.ZeroInitialize();
		}
	}

	ShaderUniformDeclaration *Material::FindUniformDeclaration(const std::string &name)
	{
		if (m_VSUniformStorageBuffer)
		{
			auto &declarations = m_Shader->GetVSMaterialUniformBuffer().GetUniformDeclarations();
			for (ShaderUniformDeclaration *uniform : declarations)
			{
				if (uniform->GetName() == name)
					return uniform;
			}
		}

		if (m_PSUniformStorageBuffer)
		{
			auto &declarations = m_Shader->GetPSMaterialUniformBuffer().GetUniformDeclarations();
			for (ShaderUniformDeclaration *uniform : declarations)
			{
				if (uniform->GetName() == name)
					return uniform;
			}
		}
		return nullptr;
	}

	ShaderResourceDeclaration *Material::FindResourceDeclaration(const std::string &name)
	{
		auto &resources = m_Shader->GetResources();
		for (ShaderResourceDeclaration *resource : resources)
		{
			if (resource->GetName() == name)
				return resource;
		}
		return nullptr;
	}

	Buffer &Material::GetUniformBufferTarget(ShaderUniformDeclaration *uniformDeclaration)
	{
		switch (uniformDeclaration->GetDomain())
		{
		case ShaderDomain::Vertex:
			return m_VSUniformStorageBuffer;
		case ShaderDomain::Pixel:
			return m_PSUniformStorageBuffer;
		}

		JN_ASSERT(false, "MATERIAL_ERROR: Invalid uniform declaration domain! Material does not support this shader type.");
		return m_VSUniformStorageBuffer;
	}

	void Material::Bind()
	{
		m_Shader->Bind();

		if (m_VSUniformStorageBuffer)
			m_Shader->SetVSMaterialUniformBuffer(m_VSUniformStorageBuffer);

		if (m_PSUniformStorageBuffer)
			m_Shader->SetPSMaterialUniformBuffer(m_PSUniformStorageBuffer);

		
		BindTextures();
	}

	void Material::BindTextures()
	{
		for (size_t i = 0; i < m_Textures.size(); i++)
		{
			auto &texture = m_Textures[i];
			if (texture)
				texture->Bind(i);
		}
	}

	void Material::SetFlag(MaterialFlag flag, bool value)
	{
		if (value)
		{
			m_MaterialFlags |= (uint32_t)flag;
		}
		else
		{
			m_MaterialFlags &= ~(uint32_t)flag;
		}
	}

	Ref<MaterialInstance> MaterialInstance::Create(const Ref<Material> &material)
	{
		return Ref<MaterialInstance>::Create(material);
	}

	MaterialInstance::MaterialInstance(const Ref<Material> &material, const std::string &name)
		: m_Material(material), m_Name(name)
	{
		m_Material->m_MaterialInstances.insert(this);
		AllocateStorage();
	}

	MaterialInstance::~MaterialInstance()
	{
		m_Material->m_MaterialInstances.erase(this);
	}

	void MaterialInstance::AllocateStorage()
	{
		if (m_Material->m_Shader->HasVSMaterialUniformBuffer())
		{
			const auto &vsBuffer = m_Material->m_Shader->GetVSMaterialUniformBuffer();
			m_VSUniformStorageBuffer.Allocate(vsBuffer.GetSize());
			memcpy(m_VSUniformStorageBuffer.Data, m_Material->m_VSUniformStorageBuffer.Data, vsBuffer.GetSize());
		}

		if (m_Material->m_Shader->HasPSMaterialUniformBuffer())
		{
			const auto &psBuffer = m_Material->m_Shader->GetPSMaterialUniformBuffer();
			m_PSUniformStorageBuffer.Allocate(psBuffer.GetSize());
			memcpy(m_PSUniformStorageBuffer.Data, m_Material->m_PSUniformStorageBuffer.Data, psBuffer.GetSize());
		}
	}

	void MaterialInstance::OnMaterialValueUpdated(ShaderUniformDeclaration *decl)
	{
		if (m_OverriddenValues.find(decl->GetName()) == m_OverriddenValues.end())
		{
			auto &buffer = GetUniformBufferTarget(decl);
			auto &materialBuffer = m_Material->GetUniformBufferTarget(decl);
			buffer.Write(materialBuffer.Data + decl->GetOffset(), decl->GetSize(), decl->GetOffset());
		}
	}

	Buffer &MaterialInstance::GetUniformBufferTarget(ShaderUniformDeclaration *uniformDeclaration)
	{
		switch (uniformDeclaration->GetDomain())
		{
		case ShaderDomain::Vertex:
			return m_VSUniformStorageBuffer;
		case ShaderDomain::Pixel:
			return m_PSUniformStorageBuffer;
		}

		JN_ASSERT(false, "APPLICATION_ERROR: Invalid uniform declaration domain! Material does not support this shader type");
		return m_VSUniformStorageBuffer;
	}

	void MaterialInstance::SetFlag(MaterialFlag flag, bool value)
	{
		if (value)
		{
			m_Material->m_MaterialFlags |= (uint32_t)flag;
		}
		else
		{
			m_Material->m_MaterialFlags &= ~(uint32_t)flag;
		}
	}

	void MaterialInstance::Bind()
	{
		m_Material->m_Shader->Bind();

		if (m_VSUniformStorageBuffer)
			m_Material->m_Shader->SetVSMaterialUniformBuffer(m_VSUniformStorageBuffer);

		if (m_PSUniformStorageBuffer)
			m_Material->m_Shader->SetPSMaterialUniformBuffer(m_PSUniformStorageBuffer);

		m_Material->BindTextures();
		for (size_t i = 0; i < m_Textures.size(); i++)
		{
			auto &texture = m_Textures[i];
			if (texture)
				texture->Bind(i);
		}
	}
}