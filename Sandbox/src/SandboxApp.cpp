#include <Moza.h>

#include "Platform/OpenGL/OpenGLShader.h"

#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class ExampleLayer : public Moza::Layer
{
public:
	ExampleLayer()
		: Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_CameraPosition(0.0f), m_RenderPosition(0.0f)
	{
		// VertexArray
		m_VertexArray.reset(Moza::VertexArray::Create());
		m_SquareVertexArray.reset(Moza::VertexArray::Create());

		//The Triangle
		float verticies[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.8f, 0.7f, 0.2f, 1.0f
		};

		// Vertex Buffer
		Moza::Ref<Moza::VertexBuffer> m_VertexBuffer;
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
		Moza::Ref<Moza::IndexBuffer> m_IndexBuffer;
		m_IndexBuffer.reset(Moza::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(m_IndexBuffer);

		//The Square
		float squareVertices[6 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};

		Moza::Ref<Moza::VertexBuffer> m_SquareVertexBuffer;
		m_SquareVertexBuffer.reset(Moza::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));

		Moza::BufferLayout squareLayout{
			{ Moza::ShaderDataType::Float3, "a_Position" },
			{ Moza::ShaderDataType::Float2, "a_TexCoord" }
		};
		
		m_SquareVertexBuffer->SetLayout(squareLayout);

		m_SquareVertexArray->AddVertexBuffer(m_SquareVertexBuffer);
		
		unsigned int squareIndices[6] = { 0, 1, 2, 2, 3, 0 };

		Moza::Ref<Moza::IndexBuffer> m_SquareIndexBuffer;
		m_SquareIndexBuffer.reset(Moza::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
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

		m_Shader.reset(Moza::Shader::Create(vertexSrc, fragmentSrc));

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

		m_FlatColorShader.reset(Moza::Shader::Create(flatColorVertexSrc, flatColorFragmentSrc));

		std::string textureShaderVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec2 a_TexCoord;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec2 v_TexCoord;

			void main()
			{
				v_TexCoord = a_TexCoord;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);	
			}
		)";

		std::string textureShaderFragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec2 v_TexCoord;
			
			uniform sampler2D u_Texture;

			void main()
			{
				color = texture(u_Texture, v_TexCoord);
			}
		)";

		m_TextureShader.reset(Moza::Shader::Create(textureShaderVertexSrc, textureShaderFragmentSrc));

		m_Texture = Moza::Texture2D::Create("assets/textures/Checkerboard.png");
		m_ChernoLogoTexture = Moza::Texture2D::Create("assets/textures/ChernoLogo.png");

		std::dynamic_pointer_cast<Moza::OpenGLShader>(m_TextureShader)->Bind();
		std::dynamic_pointer_cast<Moza::OpenGLShader>(m_TextureShader)->UploadUniformInt("u_Texture", 0);
	}

	void OnUpdate(Moza::Timestep ts) override
	{
		//move camera
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

		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);

		Moza::Renderer::BeginScene(m_Camera);

		//Render array of squares
		std::dynamic_pointer_cast<Moza::OpenGLShader>(m_FlatColorShader)->Bind();
		std::dynamic_pointer_cast<Moza::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat3("u_Color", m_SquareColor);

		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
		for (int y = 0; y < 8; y++) 
		{
			for (int x = 0; x < 8; x++)
			{
				glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos + m_RenderPosition) * scale;
				Moza::Renderer::Submit(m_FlatColorShader, m_SquareVertexArray, transform);
			}
		}

		m_Texture->Bind();
		Moza::Renderer::Submit(m_TextureShader, m_SquareVertexArray, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

		m_ChernoLogoTexture->Bind();
		Moza::Renderer::Submit(m_TextureShader, m_SquareVertexArray, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

		//Render Triangle
		//Moza::Renderer::Submit(m_Shader, m_VertexArray); 

		Moza::Renderer::EndScene();
	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
		ImGui::End();
	}

	void OnEvent(Moza::Event& event) override
	{
	}

private:
	Moza::Ref<Moza::VertexArray> m_VertexArray;
	Moza::Ref<Moza::Shader> m_Shader;
	Moza::Ref<Moza::VertexArray> m_SquareVertexArray;
	Moza::Ref<Moza::Shader> m_FlatColorShader, m_TextureShader;

	Moza::Ref<Moza::Texture2D> m_Texture, m_ChernoLogoTexture;

	Moza::OrthographicCamera m_Camera;
	glm::vec3 m_CameraPosition;
	float m_CameraMoveSpeed = 3.0f;
	
	float m_CameraRotation = 0.0f;
	float m_CameraRotationSpeed = 60.0f;

	glm::vec3 m_RenderPosition;
	float m_RenderMoveSpeed = 3.0f;

	glm::vec3 m_SquareColor = { 0.2f, 0.3f, 0.8f };
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