#include "Sandbox2D.h"

#include "Platform/OpenGL/OpenGLShader.h"

#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f, true)
{
}

void Sandbox2D::OnAttach()
{
	// VertexArray
	m_SquareVertexArray = Moza::VertexArray::Create();

	//The Square
	float squareVertices[6 * 4] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.5f,  0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f
	};

	Moza::Ref<Moza::VertexBuffer> m_SquareVertexBuffer;
	m_SquareVertexBuffer.reset(Moza::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));

	Moza::BufferLayout squareLayout{
		{ Moza::ShaderDataType::Float3, "a_Position" }
	};

	m_SquareVertexBuffer->SetLayout(squareLayout);

	m_SquareVertexArray->AddVertexBuffer(m_SquareVertexBuffer);

	unsigned int squareIndices[6] = { 0, 1, 2, 2, 3, 0 };

	Moza::Ref<Moza::IndexBuffer> m_SquareIndexBuffer;
	m_SquareIndexBuffer.reset(Moza::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
	m_SquareVertexArray->SetIndexBuffer(m_SquareIndexBuffer);

	m_FlatColorShader = Moza::Shader::Create("assets/shaders/FlatColor.glsl");
}

void Sandbox2D::OnDetach()
{
}

void Sandbox2D::OnUpdate(Moza::Timestep ts)
{
	// update Camera
	m_CameraController.OnUpdate(ts);

	// update Render
	Moza::RendererCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	Moza::RendererCommand::Clear();

	Moza::Renderer::BeginScene(m_CameraController.GetCamera());

	std::dynamic_pointer_cast<Moza::OpenGLShader>(m_FlatColorShader)->Bind();
	std::dynamic_pointer_cast<Moza::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat4("u_Color", m_SquareColor);

	Moza::Renderer::Submit(m_FlatColorShader, m_SquareVertexArray, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

	Moza::Renderer::EndScene();
}

void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
	ImGui::End();
}

void Sandbox2D::OnEvent(Moza::Event& e)
{
	m_CameraController.OnEvent(e);
}
