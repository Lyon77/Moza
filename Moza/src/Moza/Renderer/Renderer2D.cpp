#include "mzpch.h"
#include "Renderer2D.h"

#include "VertexArray.h"
#include "Shader.h"
#include "RendererCommand.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Moza
{

	struct Renderer2DStorage
	{
		Ref<VertexArray> QuadVertexArray;
		Ref<Shader> FlatColorShader;
	};

	static Renderer2DStorage* s_Data;

	void Renderer2D::Init()
	{
		s_Data = new Renderer2DStorage();

		// VertexArray
		s_Data->QuadVertexArray = VertexArray::Create();

		//The Square
		float squareVertices[6 * 4] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.5f,  0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f
		};

		Ref<VertexBuffer> m_SquareVertexBuffer;
		m_SquareVertexBuffer.reset(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));

		BufferLayout squareLayout{
			{ ShaderDataType::Float3, "a_Position" }
		};

		m_SquareVertexBuffer->SetLayout(squareLayout);

		s_Data->QuadVertexArray->AddVertexBuffer(m_SquareVertexBuffer);

		unsigned int squareIndices[6] = { 0, 1, 2, 2, 3, 0 };

		Ref<IndexBuffer> m_SquareIndexBuffer;
		m_SquareIndexBuffer.reset(IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		s_Data->QuadVertexArray->SetIndexBuffer(m_SquareIndexBuffer);

		s_Data->FlatColorShader = Shader::Create("assets/shaders/FlatColor.glsl");
	}

	void Renderer2D::Shutdown()
	{
		delete s_Data;
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		s_Data->FlatColorShader->Bind();
		s_Data->FlatColorShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
	}

	void Renderer2D::EndScene()
	{
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		s_Data->FlatColorShader->Bind();
		s_Data->FlatColorShader->SetFloat4("u_Color", color);

		// Create transform
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_Data->FlatColorShader->SetMat4("u_Transform", transform);

		s_Data->QuadVertexArray->Bind();
		RendererCommand::DrawIndexed(s_Data->QuadVertexArray);
	}
}