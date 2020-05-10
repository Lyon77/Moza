#include "Sandbox3D.h"

#include <imgui/imgui.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/quaternion.hpp>

static void ImGuiShowHelpMarker(const char* desc)
{
	ImGui::TextDisabled("(?)");
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(desc);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}

Sandbox3D::Sandbox3D()
	: m_ClearColor{ 0.1f, 0.1f, 0.1f, 1.0f }, m_Scene(Scene::Spheres),
	m_Camera(glm::perspectiveFov(glm::radians(45.0f), 1280.0f, 720.0f, 0.1f, 10000.0f))
{
}

void Sandbox3D::OnAttach()
{
	MZ_PROFILE_FUNCTION();

	m_SimplePBRShader = Moza::Shader::Create("assets/shaders/simplepbr.glsl");
	m_QuadShader = Moza::Shader::Create("assets/shaders/quad.glsl");
	m_HDRShader = Moza::Shader::Create("assets/shaders/hdr.glsl");
	//m_Mesh = Moza::CreateRef<Moza::Mesh>("assets/meshes/cerberus.fbx");
	//m_SphereMesh = Moza::CreateRef<Moza::Mesh>("assets/models/Sphere.fbx");


}

void Sandbox3D::OnDetach()
{
	MZ_PROFILE_FUNCTION();
}

void Sandbox3D::OnUpdate(Moza::Timestep ts)
{
	MZ_PROFILE_FUNCTION();
}

void Sandbox3D::OnImGuiRender()
{
	MZ_PROFILE_FUNCTION();
}

void Sandbox3D::OnEvent(Moza::Event& e)
{
	MZ_PROFILE_FUNCTION();
}
