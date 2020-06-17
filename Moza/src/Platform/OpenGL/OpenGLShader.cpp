#include "mzpch.h"
#include "Platform/OpenGL/OpenGLShader.h"

#include <fstream>
#include <glad/glad.h>

#include <glm/gtc/type_ptr.hpp>

namespace Moza
{
	static GLenum ShaderTypeFromString(const std::string& type)
	{
		if (type == "vertex") return GL_VERTEX_SHADER;
		if (type == "fragment" || type == "pixel") return GL_FRAGMENT_SHADER;

		MZ_CORE_ASSERT(false, "Unknown shader type!");
		return 0;
	}

	static std::string GetBlock(const char* str, const char** outPosition)
	{
		const char* end = strstr(str, "}");
		if (!end)
			return str;

		if (outPosition)
			*outPosition = end;
		uint32_t length = end - str + 1;
		return std::string(str, length);
	}

	static std::string GetStatement(const char* str, const char** outPosition)
	{
		const char* end = strstr(str, ";");
		if (!end)
			return str;

		if (outPosition)
			*outPosition = end;
		uint32_t length = end - str + 1;
		return std::string(str, length);
	}

	static bool IsTypeStringResource(const std::string& type)
	{
		if (type == "sampler2D")		return true;
		if (type == "samplerCube")		return true;
		if (type == "sampler2DShadow")	return true;
		return false;
	}

	static bool StartsWith(const std::string& string, const std::string& start)
	{
		return string.find(start) == 0;
	}

	// Parsing helper functions
	static const char* FindToken(const char* str, const std::string& token)
	{
		const char* t = str;
		while (t = strstr(t, token.c_str()))
		{
			bool left = str == t || isspace(t[-1]);
			bool right = !t[token.size()] || isspace(t[token.size()]);
			if (left && right)
				return t;

			t += token.size();
		}
		return nullptr;
	}

	static const char* FindToken(const std::string& string, const std::string& token)
	{
		return FindToken(string.c_str(), token);
	}

	static std::vector<std::string> SplitString(const std::string& string, const std::string& delimiters)
	{
		size_t start = 0;
		size_t end = string.find_first_of(delimiters);

		std::vector<std::string> result;

		while (end <= std::string::npos)
		{
			std::string token = string.substr(start, end - start);
			if (!token.empty())
				result.push_back(token);

			if (end == std::string::npos)
				break;

			start = end + 1;
			end = string.find_first_of(delimiters, start);
		}

		return result;
	}

	// Parsing Helper functions
	static std::vector<std::string> SplitString(const std::string& string, const char delimiter)
	{
		return SplitString(string, std::string(1, delimiter));
	}

	static std::vector<std::string> Tokenize(const std::string& string)
	{
		return SplitString(string, " \t\n");
	}

	static std::vector<std::string> GetLines(const std::string& string)
	{
		return SplitString(string, "\n");
	}

	//////////////////////////////////////////////////////////////////////////////////////
	// Shader
	//////////////////////////////////////////////////////////////////////////////////////

	OpenGLShader::OpenGLShader(const std::string& filePath)
		: m_AssetPath(filePath)
	{
		MZ_PROFILE_FUNCTION();

		// Extract name from filePath
		auto lastSlash = filePath.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		auto lastDot = filePath.rfind('.');
		auto count = lastDot == std::string::npos ? filePath.size() - lastSlash : lastDot - lastSlash;
		m_Name = filePath.substr(lastSlash, count);

		Reload();
	}

	Ref<OpenGLShader> OpenGLShader::CreateFromString(const std::string& source)
	{
		Ref<OpenGLShader> shader = CreateRef<OpenGLShader>();
		shader->Load(source);
		return shader;
	}

	OpenGLShader::~OpenGLShader()
	{
		MZ_PROFILE_FUNCTION();

		glDeleteProgram(m_RendererID);
	}

	void OpenGLShader::Reload()
	{
		std::string source = ReadFile(m_AssetPath);
		Load(source);
	}

	void OpenGLShader::Load(const std::string & source)
	{
		MZ_PROFILE_FUNCTION();

		m_ShaderSource = PreProcess(source);
		Parse();

		if (m_RendererID)
			glDeleteShader(m_RendererID);

		Compile();
		//ResolveUniforms();
		//ValidateUniforms();

		if (m_Loaded)
		{
			for (auto& callback : m_ShaderReloadedCallbacks)
				callback();
		}

		m_Loaded = true;
	}

	void OpenGLShader::AddShaderReloadedCallback(const ShaderReloadedCallback& callback)
	{
		m_ShaderReloadedCallbacks.push_back(callback);
	}

	void OpenGLShader::Bind() const
	{
		MZ_PROFILE_FUNCTION();

		glUseProgram(m_RendererID);
	}

	void OpenGLShader::UnBind() const
	{
		MZ_PROFILE_FUNCTION();

		glUseProgram(0);
	}

	void OpenGLShader::SetVSMaterialUniformBuffer(Buffer buffer)
	{
		glUseProgram(m_RendererID);
		ResolveAndSetUniforms(m_VSMaterialUniformBuffer, buffer);
	}

	void OpenGLShader::SetPSMaterialUniformBuffer(Buffer buffer)
	{
		glUseProgram(m_RendererID);
		ResolveAndSetUniforms(m_PSMaterialUniformBuffer, buffer);
	}

	void OpenGLShader::SetInt(const std::string& name, int value)
	{
		MZ_PROFILE_FUNCTION();

		UploadUniformInt(name, value);
	}

	void OpenGLShader::SetIntArray(const std::string& name, int* values, uint32_t count)
	{
		MZ_PROFILE_FUNCTION();

		UploadUniformIntArray(name, values, count);
	}

	void OpenGLShader::SetFloat(const std::string& name, const float value)
	{
		MZ_PROFILE_FUNCTION();

		UploadUniformFloat(name, value);
	}

	void OpenGLShader::SetFloat2(const std::string& name, const glm::vec2& value)
	{
		MZ_PROFILE_FUNCTION();

		UploadUniformFloat2(name, value);
	}

	void OpenGLShader::SetFloat3(const std::string& name, const glm::vec3& value)
	{
		MZ_PROFILE_FUNCTION();

		UploadUniformFloat3(name, value);
	}

	void OpenGLShader::SetFloat4(const std::string& name, const glm::vec4& value)
	{
		MZ_PROFILE_FUNCTION();

		UploadUniformFloat4(name, value);
	}

	void OpenGLShader::SetMat4(const std::string& name, const glm::mat4& value)
	{
		MZ_PROFILE_FUNCTION();

		UploadUniformMat4(name, value);
	}

	std::string OpenGLShader::ReadFile(const std::string& filePath)
	{
		MZ_PROFILE_FUNCTION();

		std::string result;
		std::ifstream in(filePath, std::ios::in | std::ios::binary);
		if (in)
		{
			//go to the end to find the length of the string
			in.seekg(0, std::ios::end);

			size_t size = in.tellg();

			if (size != -1)
			{
				//resize the string
				result.resize(in.tellg());

				//move the stream back to the front and send the data to result
				in.seekg(0, std::ios::beg);
				in.read(&result[0], result.size());
				in.close();
			} 
			else
			{
				MZ_CORE_ERROR("Could not read from file '{0}'", filePath);
			}
			
		}
		else
		{
			MZ_CORE_ERROR("Could not open file '{0}'", filePath);
		}

		return result;
	}

	std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string& source)
	{
		MZ_PROFILE_FUNCTION();

		std::unordered_map<GLenum, std::string> shaderSources;

		const char* typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);
		size_t pos = source.find(typeToken, 0);
		while (pos != std::string::npos)
		{
			//calculate shader type frin pos location
			size_t eol = source.find_first_of("\r\n", pos);
			MZ_CORE_ASSERT(eol != std::string::npos, "Syntax Error!");
			size_t begin = pos + typeTokenLength + 1;
			std::string type = source.substr(begin, eol - begin);
			MZ_CORE_ASSERT(ShaderTypeFromString(type), "Invalid shader type specifier!");

			//get the body of the shader
			size_t nextLinePos = source.find_first_not_of("\r\n", eol);
			MZ_CORE_ASSERT(nextLinePos != std::string::npos, "Syntax error");
			pos = source.find(typeToken, nextLinePos);
			shaderSources[ShaderTypeFromString(type)] = (pos == std::string::npos) ? source.substr(nextLinePos) : source.substr(nextLinePos, pos - nextLinePos);
		}

		return shaderSources;
	}

	void OpenGLShader::Parse()
	{
		const char* token;
		const char* vstr;
		const char* fstr;

		m_Resources.clear();
		m_Structs.clear();
		m_VSMaterialUniformBuffer.reset();
		m_PSMaterialUniformBuffer.reset();

		auto& vertexSource = m_ShaderSource[GL_VERTEX_SHADER];
		auto& fragmentSource = m_ShaderSource[GL_FRAGMENT_SHADER];

		// Vertex Shader
		vstr = vertexSource.c_str();
		while (token = FindToken(vstr, "struct"))
			ParseUniformStruct(GetBlock(token, &vstr), ShaderDomain::Vertex);

		vstr = vertexSource.c_str();
		while (token = FindToken(vstr, "uniform"))
			ParseUniform(GetStatement(token, &vstr), ShaderDomain::Vertex);

		// Fragment Shader
		fstr = fragmentSource.c_str();
		while (token = FindToken(fstr, "struct"))
			ParseUniformStruct(GetBlock(token, &fstr), ShaderDomain::Pixel);

		fstr = fragmentSource.c_str();
		while (token = FindToken(fstr, "uniform"))
			ParseUniform(GetStatement(token, &fstr), ShaderDomain::Pixel);
	}

	void OpenGLShader::ParseUniform(const std::string& statement, ShaderDomain domain)
	{
		std::vector<std::string> tokens = Tokenize(statement);
		uint32_t index = 0;

		index++; // "uniform"
		std::string typeString = tokens[index++];
		std::string name = tokens[index++];
		// Strip ; from name if present
		if (const char* s = strstr(name.c_str(), ";"))
			name = std::string(name.c_str(), s - name.c_str());

		std::string n(name);
		int32_t count = 1;
		const char* namestr = n.c_str();
		if (const char* s = strstr(namestr, "["))
		{
			name = std::string(namestr, s - namestr);

			const char* end = strstr(namestr, "]");
			std::string c(s + 1, end - s);
			count = atoi(c.c_str());
		}

		if (IsTypeStringResource(typeString))
		{
			ShaderResourceDeclaration* declaration = new OpenGLShaderResourceDeclaration(OpenGLShaderResourceDeclaration::StringToType(typeString), name, count);
			m_Resources.push_back(declaration);
		}
		else
		{
			OpenGLShaderUniformDeclaration::Type t = OpenGLShaderUniformDeclaration::StringToType(typeString);
			OpenGLShaderUniformDeclaration* declaration = nullptr;

			if (t == OpenGLShaderUniformDeclaration::Type::NONE)
			{
				// Find struct
				ShaderStruct* s = FindStruct(typeString);
				MZ_CORE_ASSERT(s, "");
				declaration = new OpenGLShaderUniformDeclaration(domain, s, name, count);
			}
			else
			{
				declaration = new OpenGLShaderUniformDeclaration(domain, t, name, count);
			}

			if (StartsWith(name, "r_"))
			{
				if (domain == ShaderDomain::Vertex)
					((OpenGLShaderUniformBufferDeclaration*)m_VSRendererUniformBuffers.front())->PushUniform(declaration);
				else if (domain == ShaderDomain::Pixel)
					((OpenGLShaderUniformBufferDeclaration*)m_PSRendererUniformBuffers.front())->PushUniform(declaration);
			}
			else
			{
				if (domain == ShaderDomain::Vertex)
				{
					if (!m_VSMaterialUniformBuffer)
						m_VSMaterialUniformBuffer.reset(new OpenGLShaderUniformBufferDeclaration("", domain));

					m_VSMaterialUniformBuffer->PushUniform(declaration);
				}
				else if (domain == ShaderDomain::Pixel)
				{
					if (!m_PSMaterialUniformBuffer)
						m_PSMaterialUniformBuffer.reset(new OpenGLShaderUniformBufferDeclaration("", domain));

					m_PSMaterialUniformBuffer->PushUniform(declaration);
				}
			}
		}
	}

	void OpenGLShader::ParseUniformStruct(const std::string& block, ShaderDomain domain)
	{
		std::vector<std::string> tokens = Tokenize(block);

		uint32_t index = 0;
		index++; // struct
		std::string name = tokens[index++];
		ShaderStruct* uniformStruct = new ShaderStruct(name);
		index++; // {
		while (index < tokens.size())
		{
			if (tokens[index] == "}")
				break;

			std::string type = tokens[index++];
			std::string name = tokens[index++];

			// Strip ; from name if present
			if (const char* s = strstr(name.c_str(), ";"))
				name = std::string(name.c_str(), s - name.c_str());

			uint32_t count = 1;
			const char* namestr = name.c_str();
			if (const char* s = strstr(namestr, "["))
			{
				name = std::string(namestr, s - namestr);

				const char* end = strstr(namestr, "]");
				std::string c(s + 1, end - s);
				count = atoi(c.c_str());
			}
			ShaderUniformDeclaration* field = new OpenGLShaderUniformDeclaration(domain, OpenGLShaderUniformDeclaration::StringToType(type), name, count);
			uniformStruct->AddField(field);
		}
		m_Structs.push_back(uniformStruct);
	}

	ShaderStruct* OpenGLShader::FindStruct(const std::string& name)
	{
		for (ShaderStruct* s : m_Structs)
		{
			if (s->GetName() == name)
				return s;
		}
		return nullptr;
	}

	void OpenGLShader::Compile()
	{
		MZ_PROFILE_FUNCTION();

		// Get a program object.
		GLuint program = glCreateProgram();

		MZ_CORE_ASSERT(m_ShaderSource.size() <= 2, "We only support 2 shaders for now");
		std::array<GLenum, 2> glShaderIDs;

		int glShaderIDIndex = 0;
		for (auto& kv : m_ShaderSource)
		{
			GLenum type = kv.first;
			const std::string& source = kv.second;

			// Create an empty shader handle
			GLuint shader = glCreateShader(type);

			// Send the shader source code to GL
			// Note that std::string's .c_str is NULL character terminated.
			const GLchar* sourceCStr = source.c_str();
			glShaderSource(shader, 1, &sourceCStr, 0);

			// Compile the vertex shader
			glCompileShader(shader);

			GLint isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

				// The maxLength includes the NULL character
				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

				// We don't need the shader anymore.
				glDeleteShader(shader);

				MZ_CORE_ERROR("{0}", infoLog.data());
				MZ_CORE_ASSERT(false, "Shader compilation failure!");
				break;
			}

			// Attach our shaders to our program
			glAttachShader(program, shader);
			glShaderIDs[glShaderIDIndex++] = shader;
		}

		//Set m_RendererID now so that we know the shaders work
		m_RendererID = program;

		// Link our program
		glLinkProgram(program);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(program);
			// Don't leak shaders either.
			for (auto id : glShaderIDs)
				glDeleteShader(id);

			MZ_CORE_ERROR("{0}", infoLog.data());
			MZ_CORE_ASSERT(false, "Shader link failure!");
			return;
		}

		// Always detach shaders after a successful link.
		for (auto id : glShaderIDs)
		{
			glDetachShader(program, id);
			glDeleteShader(id);
		}

		m_RendererID = program;
	}

	void OpenGLShader::ResolveUniforms()
	{
		glUseProgram(m_RendererID);

		for (size_t i = 0; i < m_VSRendererUniformBuffers.size(); i++)
		{
			OpenGLShaderUniformBufferDeclaration* decl = (OpenGLShaderUniformBufferDeclaration*)m_VSRendererUniformBuffers[i];
			const ShaderUniformList& uniforms = decl->GetUniformDeclarations();
			for (size_t j = 0; j < uniforms.size(); j++)
			{
				OpenGLShaderUniformDeclaration* uniform = (OpenGLShaderUniformDeclaration*)uniforms[j];
				if (uniform->GetType() == OpenGLShaderUniformDeclaration::Type::STRUCT)
				{
					const ShaderStruct& s = uniform->GetShaderUniformStruct();
					const auto& fields = s.GetFields();
					for (size_t k = 0; k < fields.size(); k++)
					{
						OpenGLShaderUniformDeclaration* field = (OpenGLShaderUniformDeclaration*)fields[k];
						field->m_Location = GetUniformLocation(uniform->m_Name + "." + field->m_Name);
					}
				}
				else
				{
					uniform->m_Location = GetUniformLocation(uniform->m_Name);
				}
			}
		}

		for (size_t i = 0; i < m_PSRendererUniformBuffers.size(); i++)
		{
			OpenGLShaderUniformBufferDeclaration* decl = (OpenGLShaderUniformBufferDeclaration*)m_PSRendererUniformBuffers[i];
			const ShaderUniformList& uniforms = decl->GetUniformDeclarations();
			for (size_t j = 0; j < uniforms.size(); j++)
			{
				OpenGLShaderUniformDeclaration* uniform = (OpenGLShaderUniformDeclaration*)uniforms[j];
				if (uniform->GetType() == OpenGLShaderUniformDeclaration::Type::STRUCT)
				{
					const ShaderStruct& s = uniform->GetShaderUniformStruct();
					const auto& fields = s.GetFields();
					for (size_t k = 0; k < fields.size(); k++)
					{
						OpenGLShaderUniformDeclaration* field = (OpenGLShaderUniformDeclaration*)fields[k];
						field->m_Location = GetUniformLocation(uniform->m_Name + "." + field->m_Name);
					}
				}
				else
				{
					uniform->m_Location = GetUniformLocation(uniform->m_Name);
				}
			}
		}

		{
			const auto& decl = m_VSMaterialUniformBuffer;
			if (decl)
			{
				const ShaderUniformList& uniforms = decl->GetUniformDeclarations();
				for (size_t j = 0; j < uniforms.size(); j++)
				{
					OpenGLShaderUniformDeclaration* uniform = (OpenGLShaderUniformDeclaration*)uniforms[j];
					if (uniform->GetType() == OpenGLShaderUniformDeclaration::Type::STRUCT)
					{
						const ShaderStruct& s = uniform->GetShaderUniformStruct();
						const auto& fields = s.GetFields();
						for (size_t k = 0; k < fields.size(); k++)
						{
							OpenGLShaderUniformDeclaration* field = (OpenGLShaderUniformDeclaration*)fields[k];
							field->m_Location = GetUniformLocation(uniform->m_Name + "." + field->m_Name);
						}
					}
					else
					{
						uniform->m_Location = GetUniformLocation(uniform->m_Name);
					}
				}
			}
		}

		{
			const auto& decl = m_PSMaterialUniformBuffer;
			if (decl)
			{
				const ShaderUniformList& uniforms = decl->GetUniformDeclarations();
				for (size_t j = 0; j < uniforms.size(); j++)
				{
					OpenGLShaderUniformDeclaration* uniform = (OpenGLShaderUniformDeclaration*)uniforms[j];
					if (uniform->GetType() == OpenGLShaderUniformDeclaration::Type::STRUCT)
					{
						const ShaderStruct& s = uniform->GetShaderUniformStruct();
						const auto& fields = s.GetFields();
						for (size_t k = 0; k < fields.size(); k++)
						{
							OpenGLShaderUniformDeclaration* field = (OpenGLShaderUniformDeclaration*)fields[k];
							field->m_Location = GetUniformLocation(uniform->m_Name + "." + field->m_Name);
						}
					}
					else
					{
						uniform->m_Location = GetUniformLocation(uniform->m_Name);
					}
				}
			}
		}

		uint32_t sampler = 0;
		for (size_t i = 0; i < m_Resources.size(); i++)
		{
			OpenGLShaderResourceDeclaration* resource = (OpenGLShaderResourceDeclaration*)m_Resources[i];
			int32_t location = GetUniformLocation(resource->m_Name);

			if (resource->GetCount() == 1)
			{
				resource->m_Register = sampler;
				if (location != -1)
					UploadUniformInt(resource->m_Name, sampler);

				sampler++;
			}
			else if (resource->GetCount() > 1)
			{
				resource->m_Register = 0;
				uint32_t count = resource->GetCount();
				int* samplers = new int[count];
				for (uint32_t s = 0; s < count; s++)
					samplers[s] = s;
				UploadUniformIntArray(resource->GetName(), samplers, count);
				delete[] samplers;
			}
		}
	}

	void OpenGLShader::ValidateUniforms()
	{
	}

	int32_t OpenGLShader::GetUniformLocation(const std::string& name) const
	{
		int32_t result = glGetUniformLocation(m_RendererID, name.c_str());
		if (result == -1)
			MZ_CORE_WARN("Could not find uniform '{0}' in shader", name);

		return result;
	}

	void OpenGLShader::ResolveAndSetUniforms(const Scope<OpenGLShaderUniformBufferDeclaration>& decl, Buffer buffer)
	{
		const ShaderUniformList& uniforms = decl->GetUniformDeclarations();
		for (size_t i = 0; i < uniforms.size(); i++)
		{
			OpenGLShaderUniformDeclaration* uniform = (OpenGLShaderUniformDeclaration*)uniforms[i];
			if (uniform->IsArray())
				ResolveAndSetUniformArray(uniform, buffer);
			else
				ResolveAndSetUniform(uniform, buffer);
		}
	}

	void OpenGLShader::ResolveAndSetUniform(OpenGLShaderUniformDeclaration* uniform, Buffer buffer)
	{
		if (uniform->GetLocation() == -1)
			return;

		MZ_CORE_ASSERT(uniform->GetLocation() != -1, "Uniform has invalid location!");

		uint32_t offset = uniform->GetOffset();
		switch (uniform->GetType())
		{
		case OpenGLShaderUniformDeclaration::Type::FLOAT32:
			UploadUniformFloat(uniform->GetName(), *(float*)&buffer.Data[offset]);
			break;
		case OpenGLShaderUniformDeclaration::Type::INT32:
			UploadUniformInt(uniform->GetName(), *(int32_t*)&buffer.Data[offset]);
			break;
		case OpenGLShaderUniformDeclaration::Type::VEC2:
			UploadUniformFloat2(uniform->GetName(), *(glm::vec2*) & buffer.Data[offset]);
			break;
		case OpenGLShaderUniformDeclaration::Type::VEC3:
			UploadUniformFloat3(uniform->GetName(), *(glm::vec3*) & buffer.Data[offset]);
			break;
		case OpenGLShaderUniformDeclaration::Type::VEC4:
			UploadUniformFloat4(uniform->GetName(), *(glm::vec4*) & buffer.Data[offset]);
			break;
		case OpenGLShaderUniformDeclaration::Type::MAT3:
			UploadUniformMat3(uniform->GetName(), *(glm::mat3*) & buffer.Data[offset]);
			break;
		case OpenGLShaderUniformDeclaration::Type::MAT4:
			UploadUniformMat4(uniform->GetName(), *(glm::mat4*) & buffer.Data[offset]);
			break;
		case OpenGLShaderUniformDeclaration::Type::STRUCT:
			UploadUniformStruct(uniform, buffer.Data, offset);
			break;
		default:
			MZ_CORE_ASSERT(false, "Unknown uniform type!");
		}
	}

	void OpenGLShader::ResolveAndSetUniformArray(OpenGLShaderUniformDeclaration* uniform, Buffer buffer)
	{
		uint32_t offset = uniform->GetOffset();
		switch (uniform->GetType())
		{
		case OpenGLShaderUniformDeclaration::Type::FLOAT32:
			UploadUniformFloat(uniform->GetName(), *(float*)&buffer.Data[offset]);
			break;
		case OpenGLShaderUniformDeclaration::Type::INT32:
			UploadUniformInt(uniform->GetName(), *(int32_t*)&buffer.Data[offset]);
			break;
		case OpenGLShaderUniformDeclaration::Type::VEC2:
			UploadUniformFloat2(uniform->GetName(), *(glm::vec2*) & buffer.Data[offset]);
			break;
		case OpenGLShaderUniformDeclaration::Type::VEC3:
			UploadUniformFloat3(uniform->GetName(), *(glm::vec3*) & buffer.Data[offset]);
			break;
		case OpenGLShaderUniformDeclaration::Type::VEC4:
			UploadUniformFloat4(uniform->GetName(), *(glm::vec4*) & buffer.Data[offset]);
			break;
		case OpenGLShaderUniformDeclaration::Type::MAT3:
			UploadUniformMat3(uniform->GetName(), *(glm::mat3*) & buffer.Data[offset]);
			break;
		case OpenGLShaderUniformDeclaration::Type::MAT4:
			UploadUniformMat4Array(uniform->GetName(), *(glm::mat4*) & buffer.Data[offset], uniform->GetCount());
			break;
		case OpenGLShaderUniformDeclaration::Type::STRUCT:
			UploadUniformStruct(uniform, buffer.Data, offset);
			break;
		default:
			MZ_CORE_ASSERT(false, "Unknown uniform type!");
		}
	}

	void OpenGLShader::ResolveAndSetUniformField(const OpenGLShaderUniformDeclaration& field, unsigned char* data, int32_t offset)
	{
		switch (field.GetType())
		{
		case OpenGLShaderUniformDeclaration::Type::FLOAT32:
			UploadUniformFloat(field.GetName(), *(float*)&data[offset]);
			break;
		case OpenGLShaderUniformDeclaration::Type::INT32:
			UploadUniformInt(field.GetName(), *(int32_t*)&data[offset]);
			break;
		case OpenGLShaderUniformDeclaration::Type::VEC2:
			UploadUniformFloat2(field.GetName(), *(glm::vec2*) & data[offset]);
			break;
		case OpenGLShaderUniformDeclaration::Type::VEC3:
			UploadUniformFloat3(field.GetName(), *(glm::vec3*) & data[offset]);
			break;
		case OpenGLShaderUniformDeclaration::Type::VEC4:
			UploadUniformFloat4(field.GetName(), *(glm::vec4*) & data[offset]);
			break;
		case OpenGLShaderUniformDeclaration::Type::MAT3:
			UploadUniformMat3(field.GetName(), *(glm::mat3*) & data[offset]);
			break;
		case OpenGLShaderUniformDeclaration::Type::MAT4:
			UploadUniformMat4(field.GetName(), *(glm::mat4*) & data[offset]);
			break;
		default:
			MZ_CORE_ASSERT(false, "Unknown uniform type!");
		}
	}

	void OpenGLShader::UploadUniformInt(const std::string& name, int value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1i(location, value);
	}

	void OpenGLShader::UploadUniformIntArray(const std::string& name, int* values, uint32_t count)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1iv(location, count, values);
	}

	void OpenGLShader::UploadUniformMat3(const std::string& name, const glm::mat3 & matrix)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void OpenGLShader::UploadUniformMat4(const std::string& name, const glm::mat4 & matrix)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void OpenGLShader::UploadUniformMat4Array(const std::string& name, const glm::mat4& matrix, uint32_t count)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix4fv(location, count, GL_FALSE, glm::value_ptr(matrix));
	}

	void OpenGLShader::UploadUniformFloat(const std::string& name, float value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1f(location, value);
	}

	void OpenGLShader::UploadUniformFloat2(const std::string& name, const glm::vec2 & values)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform2f(location, values.x, values.y);
	}

	void OpenGLShader::UploadUniformFloat3(const std::string& name, const glm::vec3 & values)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform3f(location, values.x, values.y, values.z);
	}

	void OpenGLShader::UploadUniformFloat4(const std::string& name, const glm::vec4 & values)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform4f(location, values.x, values.y, values.z, values.w);
	}

	void OpenGLShader::UploadUniformStruct(OpenGLShaderUniformDeclaration* uniform, unsigned char* buffer, uint32_t offset)
	{
		const ShaderStruct& s = uniform->GetShaderUniformStruct();
		const auto& fields = s.GetFields();
		for (size_t k = 0; k < fields.size(); k++)
		{
			OpenGLShaderUniformDeclaration* field = (OpenGLShaderUniformDeclaration*)fields[k];
			ResolveAndSetUniformField(*field, buffer, offset);
			offset += field->m_Size;
		}
	}
}