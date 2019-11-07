#include <Moza.h>

#include "imgui/imgui.h"

class ExampleLayer : public Moza::Layer
{
public:
	ExampleLayer()
		: Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_CameraPosition(0.0f)
	{
		// VertexArray
		m_VertexArray.reset(Moza::VertexArray::Create());

		//The Triangle
		float verticies[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.8f, 0.7f, 0.2f, 1.0f
		};

		// Vertex Buffer
		std::shared_ptr<Moza::VertexBuffer> m_VertexBuffer;
		m_VertexBuffer.reset(Moza::VertexBuffer::Create(verticies, sizeof(verticies)));

		//Create details on how the layout is set
		Moza::BufferLayout layout{
			{ Moza::ShaderDataType::Float3, "a_Position" },
			{ Moza::ShaderDataType::Float4, "a_Color" }
		};

		m_VertexBuffer->SetLayout(layout);

		// assign VertexBuffer to VertexArray
		m_VertexArray->AddVertexBuffer(m_VertexBuffer);

		unsigned int indices[3] = { 0, 1, 2 };

		// Index Buffer
		std::shared_ptr<Moza::IndexBuffer> m_IndexBuffer;
		m_IndexBuffer.reset(Moza::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(m_IndexBuffer);

		std::string vertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_ViewProjection;

			out vec3 v_Position;			
			out vec4 v_Color;			

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
			}
		)";

		std::string fragmentSrc = R"(
			#version 330 core

			layout(location = 0) out vec4 color;			
			in vec3 v_Position;
			in vec4 v_Color;

			void main()
			{
				color = vec4(v_Position * 0.5 + 0.5, 1.0);
				color = v_Color;
			}
		)";

		m_Shader.reset(new Moza::Shader(vertexSrc, fragmentSrc));
	}

	void OnUpdate(Moza::Timestep ts) override
	{
		if (Moza::Input::IsKeyPressed(MZ_KEY_LEFT))
			m_CameraPosition.x -= m_CameraMoveSpeed * ts;
		else if (Moza::Input::IsKeyPressed(MZ_KEY_RIGHT))
			m_CameraPosition.x += m_CameraMoveSpeed * ts;

		if (Moza::Input::IsKeyPressed(MZ_KEY_DOWN))
			m_CameraPosition.y -= m_CameraMoveSpeed * ts;
		else if (Moza::Input::IsKeyPressed(MZ_KEY_UP))
			m_CameraPosition.y += m_CameraMoveSpeed * ts;

		//positive is clockwise rotation for the camera
		if (Moza::Input::IsKeyPressed(MZ_KEY_A))
			m_CameraRotation += m_CameraRotationSpeed * ts;
		else if (Moza::Input::IsKeyPressed(MZ_KEY_D))
			m_CameraRotation -= m_CameraRotationSpeed * ts;
		

		Moza::RendererCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		Moza::RendererCommand::Clear();

		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);

		Moza::Renderer::BeginScene(m_Camera);

		Moza::Renderer::Submit(m_Shader, m_VertexArray);

		Moza::Renderer::EndScene();
	}

	virtual void OnImGuiRender() override
	{
	}

	void OnEvent(Moza::Event& event) override
	{
	}

private:
	std::shared_ptr<Moza::VertexArray> m_VertexArray;
	std::shared_ptr<Moza::Shader> m_Shader;

	Moza::OrthographicCamera m_Camera;
	glm::vec3 m_CameraPosition;
	float m_CameraMoveSpeed = 3.0f;
	
	float m_CameraRotation = 0.0f;
	float m_CameraRotationSpeed = 60.0f;
};

class Sandbox : public Moza::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
	}

	~Sandbox()
	{

	}
};


Moza::Application* Moza::CreateApplication()
{
	return new Sandbox();
}