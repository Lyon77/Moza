#include "ExampleLayer.h"

#include <imgui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

ExampleLayer::ExampleLayer()
	: Layer("Example"), m_CameraController(1280.0f / 720.0f, true), m_RenderPosition(0.0f)
{
	// VertexArray
	m_VertexArray = Moza::VertexArray::Create();
	m_SquareVertexArray = Moza::VertexArray::Create();

	//The Triangle
	float verticies[3 * 7] = {
		-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
		 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
		 0.0f,  0.5f, 0.0f, 0.8f, 0.7f, 0.2f, 1.0f
	};

	// Vertex Buffer
	Moza::Ref<Moza::VertexBuffer> m_VertexBuffer = Moza::VertexBuffer::Create(verticies, sizeof(verticies));

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
	Moza::Ref<Moza::IndexBuffer> m_IndexBuffer = Moza::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
	m_VertexArray->SetIndexBuffer(m_IndexBuffer);

	//The Square
	float squareVertices[6 * 4] = {
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
		 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
		-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
	};

	Moza::Ref<Moza::VertexBuffer> m_SquareVertexBuffer = Moza::VertexBuffer::Create(squareVertices, sizeof(squareVertices));

	Moza::BufferLayout squareLayout{
		{ Moza::ShaderDataType::Float3, "a_Position" },
		{ Moza::ShaderDataType::Float2, "a_TexCoord" }
	};

	m_SquareVertexBuffer->SetLayout(squareLayout);

	m_SquareVertexArray->AddVertexBuffer(m_SquareVertexBuffer);

	unsigned int squareIndices[6] = { 0, 1, 2, 2, 3, 0 };

	Moza::Ref<Moza::IndexBuffer> m_SquareIndexBuffer = Moza::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
	m_SquareVertexArray->SetIndexBuffer(m_SquareIndexBuffer);

	std::string vertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;			
			out vec4 v_Color;			

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
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

	Moza::Ref<Moza::Shader> shader = Moza::Shader::Create("VertexPosColor", vertexSrc, fragmentSrc);
	m_ShaderLibrary.Add("VertexPosColor", shader);

	std::string flatColorVertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;
 
			out vec3 v_Position;					

			void main()
			{
				v_Position = a_Position;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}
		)";

	std::string flatColorFragmentSrc = R"(
			#version 330 core

			layout(location = 0) out vec4 color;			
			in vec3 v_Position;
	
			uniform vec3 u_Color;

			void main()
			{
				color = vec4(u_Color, 1.0f);
			}
		)";

	Moza::Ref<Moza::Shader> flatColorShader = Moza::Shader::Create("FlatColor", flatColorVertexSrc, flatColorFragmentSrc);

	m_ShaderLibrary.Add("FlatColor", flatColorShader);

	auto textureShader = m_ShaderLibrary.Load("assets/shaders/Texture.glsl");

	m_Texture = Moza::Texture2D::Create("assets/textures/Checkerboard.png");
	m_ChernoLogoTexture = Moza::Texture2D::Create("assets/textures/ChernoLogo.png");

	textureShader->Bind();
	textureShader->SetInt("u_Texture", 0);
}

void ExampleLayer::OnAttach()
{
}

void ExampleLayer::OnDetach()
{
}

void ExampleLayer::OnUpdate(Moza::Timestep ts)
{
	// update Camera
	m_CameraController.OnUpdate(ts);

	// update Render
	//move render
	if (Moza::Input::IsKeyPressed(MZ_KEY_J))
		m_RenderPosition.x -= m_RenderMoveSpeed * ts;
	else if (Moza::Input::IsKeyPressed(MZ_KEY_L))
		m_RenderPosition.x += m_RenderMoveSpeed * ts;

	if (Moza::Input::IsKeyPressed(MZ_KEY_K))
		m_RenderPosition.y -= m_RenderMoveSpeed * ts;
	else if (Moza::Input::IsKeyPressed(MZ_KEY_I))
		m_RenderPosition.y += m_RenderMoveSpeed * ts;


	Moza::RendererCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	Moza::RendererCommand::Clear();

	Moza::Renderer::BeginScene(m_CameraController.GetCamera());

	//Render array of squares
	auto flatShader = m_ShaderLibrary.Get("FlatColor");

	flatShader->Bind();
	flatShader->SetFloat3("u_Color", m_SquareColor);

	glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
	for (int y = 0; y < 8; y++)
	{
		for (int x = 0; x < 8; x++)
		{
			glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
			glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos + m_RenderPosition) * scale;
			Moza::Renderer::Submit(flatShader, m_SquareVertexArray, transform);
		}
	}

	auto textureShader = m_ShaderLibrary.Get("Texture");

	m_Texture->Bind();
	Moza::Renderer::Submit(textureShader, m_SquareVertexArray, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

	m_ChernoLogoTexture->Bind();
	Moza::Renderer::Submit(textureShader, m_SquareVertexArray, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

	//Render Triangle
	auto shader = m_ShaderLibrary.Get("VertexPosColor");
	Moza::Renderer::Submit(shader, m_VertexArray);

	Moza::Renderer::EndScene();
}

void ExampleLayer::OnImGuiRender() 
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
	ImGui::End();
}

void ExampleLayer::OnEvent(Moza::Event& e) 
{
	m_CameraController.OnEvent(e);
}