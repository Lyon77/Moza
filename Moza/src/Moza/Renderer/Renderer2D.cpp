#include "mzpch.h"
#include "Moza/Renderer/Renderer2D.h"

#include "Moza/Renderer/VertexArray.h"
#include "Moza/Renderer/Shader.h"
#include "Moza/Renderer/RendererCommand.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Moza
{

	struct Renderer2DStorage
	{
		Ref<VertexArray> QuadVertexArray;
		Ref<Shader> TextureAndColorShader;
		Ref<Shader> ScreenShader;
		Ref<Texture2D> WhiteTexture;
	};

	static Renderer2DStorage* s_Data;

	void Renderer2D::Init()
	{
		MZ_PROFILE_FUNCTION();

		s_Data = new Renderer2DStorage();

		// VertexArray
		s_Data->QuadVertexArray = VertexArray::Create();

		//The Square
		float squareVertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};

		Ref<VertexBuffer> m_SquareVertexBuffer = VertexBuffer::Create(squareVertices, sizeof(squareVertices));

		BufferLayout squareLayout{
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float2, "a_TextCood" }
		};

		m_SquareVertexBuffer->SetLayout(squareLayout);

		s_Data->QuadVertexArray->AddVertexBuffer(m_SquareVertexBuffer);

		unsigned int squareIndices[6] = { 0, 1, 2, 2, 3, 0 };

		Ref<IndexBuffer> m_SquareIndexBuffer = IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
		s_Data->QuadVertexArray->SetIndexBuffer(m_SquareIndexBuffer);

		// Create White Texture
		s_Data->WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_Data->WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		s_Data->TextureAndColorShader = Shader::Create("assets/shaders/TextureAndColor.glsl");
		s_Data->ScreenShader = Shader::Create("assets/shaders/TextureAndColor.glsl");
		s_Data->TextureAndColorShader->Bind();
		s_Data->TextureAndColorShader->SetInt("u_Texture", 0);
		s_Data->ScreenShader->Bind();
		s_Data->ScreenShader->SetInt("u_Texture", 0);
	}

	void Renderer2D::Shutdown()
	{
		MZ_PROFILE_FUNCTION();

		delete s_Data;
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		MZ_PROFILE_FUNCTION();

		s_Data->TextureAndColorShader->Bind();
		s_Data->TextureAndColorShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());

		s_Data->ScreenShader->Bind();
		s_Data->ScreenShader->SetMat4("u_ViewProjection", camera.GetProjectionMatrix());
	}

	void Renderer2D::EndScene()
	{
		MZ_PROFILE_FUNCTION();

	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		MZ_PROFILE_FUNCTION();
		s_Data->TextureAndColorShader->Bind();
		s_Data->TextureAndColorShader->SetFloat4("u_Color", color);

		// Bind White Texture
		s_Data->WhiteTexture->Bind();

		s_Data->TextureAndColorShader->SetFloat("u_TextureScale", 1.0f);

		// Create transform
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_Data->TextureAndColorShader->SetMat4("u_Transform", transform);

		s_Data->QuadVertexArray->Bind();
		RendererCommand::DrawIndexed(s_Data->QuadVertexArray);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& color, const float textureScale)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, texture, color, textureScale);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& color, const float textureScale)
	{
		MZ_PROFILE_FUNCTION();
		s_Data->TextureAndColorShader->Bind();
		texture->Bind();
		s_Data->TextureAndColorShader->SetFloat("u_TextureScale", textureScale);

		s_Data->TextureAndColorShader->SetFloat4("u_Color", color);

		// Create transform
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_Data->TextureAndColorShader->SetMat4("u_Transform", transform);

		s_Data->QuadVertexArray->Bind();
		RendererCommand::DrawIndexed(s_Data->QuadVertexArray);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, color);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		MZ_PROFILE_FUNCTION();
		s_Data->TextureAndColorShader->Bind();
		s_Data->TextureAndColorShader->SetFloat4("u_Color", color);

		// Bind White Texture
		s_Data->WhiteTexture->Bind();

		s_Data->TextureAndColorShader->SetFloat("u_TextureScale", 1.0f);

		// Create transform
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) 
			* glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_Data->TextureAndColorShader->SetMat4("u_Transform", transform);

		s_Data->QuadVertexArray->Bind();
		RendererCommand::DrawIndexed(s_Data->QuadVertexArray);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, const glm::vec4& color, const float textureScale)
	{
		DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, texture, color, textureScale);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, const glm::vec4& color, const float textureScale)
	{
		MZ_PROFILE_FUNCTION();
		s_Data->TextureAndColorShader->Bind();
		texture->Bind();
		s_Data->TextureAndColorShader->SetFloat("u_TextureScale", textureScale);

		s_Data->TextureAndColorShader->SetFloat4("u_Color", color);

		// Create transform
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) 
			* glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_Data->TextureAndColorShader->SetMat4("u_Transform", transform);

		s_Data->QuadVertexArray->Bind();
		RendererCommand::DrawIndexed(s_Data->QuadVertexArray);
	}

	void Renderer2D::DrawScreenQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& color, float textureScale)
	{
		MZ_PROFILE_FUNCTION();
		s_Data->ScreenShader->Bind();
		s_Data->ScreenShader->SetFloat4("u_Color", color);

		// Bind White Texture
		texture->Bind();

		s_Data->ScreenShader->SetFloat("u_TextureScale", textureScale);

		// Create transform
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), { position.x, position.y, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_Data->ScreenShader->SetMat4("u_Transform", transform);

		s_Data->QuadVertexArray->Bind();
		RendererCommand::DrawIndexed(s_Data->QuadVertexArray);
	}
}