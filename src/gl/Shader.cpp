//  Copyright (c) 2024-2026 Daniel Moreno. All rights reserved.
//

#include <gl/Shader.hpp>

#include <vector>
#include <stdexcept>
#include <cstdio>

namespace gl
{
  Shader::Shader(GLuint type) : Shader{}
  {
    initialize(type);
  }

  Shader::Shader(GLuint type, const std::string& source) : Shader{}
  {
    initialize(type, source);
  }

  Shader::Shader(Shader&& other) noexcept
  {
    operator=(std::move(other));
  }

  Shader::~Shader()
  {
    uninitialize();
  }

  Shader& Shader::operator=(Shader&& other) noexcept
  {
    if (this != &other)
    {
      shader_ = other.shader_;

      // Reset the other shader to a default state
      Shader empty;
      other.shader_ = empty.shader_;
    }
    return *this;
  }

  void Shader::initialize(GLuint type)
  {
    if (!isInitialized())
    {
      shader_ = glCreateShader(type);
    }
  }

  void Shader::initialize(GLuint type, const std::string& source)
  {
    // Ensure the shader is uninitialized before initializing it with a new type
    // and source code
    uninitialize();
    // Initialize the shader with the specified type
    initialize(type);
    // Set the source code
    setSource(source);
    // Compile the shader
    compile();
  }

  void Shader::uninitialize()
  {
    if (isInitialized())
    {
      glDeleteShader(shader_);
      shader_ = 0;
    }
  }

  bool Shader::hasSource() const
  {
    if (isInitialized())
    {
      GLint length;
      glGetShaderiv(shader_, GL_SHADER_SOURCE_LENGTH, &length);
      return length > 0;
    }
    return false;
  }

  void Shader::setSource(const std::string& source)
  {
    if (!isInitialized())
    {
      throw std::runtime_error{ "ERROR::SHADER:: "
        "Cannot set source of uninitialized shader!"};
    }
    const char* sourceCStr = source.c_str();
    glShaderSource(shader_, 1, &sourceCStr, nullptr);
  }

  std::string Shader::getSource() const
  {
    if (!isInitialized())
    {
      throw std::runtime_error{ "ERROR::SHADER:: "
        "Cannot get source of uninitialized shader!"};
    }

    GLint length;
    glGetShaderiv(shader_, GL_SHADER_SOURCE_LENGTH, &length);
    if (length <= 0)
    {
      return "";
    }

    std::vector<char> source(length);
    glGetShaderSource(shader_, length, nullptr, source.data());
    return std::string(source.data(), length - 1); // Exclude null terminator
  }

  bool Shader::isCompiled() const
  {
    if (isInitialized())
    {
      int status;
      glGetShaderiv(shader_, GL_COMPILE_STATUS, &status);
      return status == GL_TRUE;
    }
    return false;
  }

  void Shader::compile()
  {
    if (!isInitialized())
    {
      throw std::runtime_error{ "ERROR::SHADER:: "
        "Cannot compile uninitialized shader!"};
    }
    if (!hasSource())
    {
      throw std::runtime_error{ "ERROR::SHADER:: "
        "Cannot compile shader without source code!"};
    }

    // Compile the shader
    glCompileShader(shader_);

    // Check compilation status
    int status_, length_;
    glGetShaderiv(shader_, GL_COMPILE_STATUS, &status_);
    if (status_ == GL_FALSE)
    {
      glGetShaderiv(shader_, GL_INFO_LOG_LENGTH, &length_);
      std::vector<char> infoLog_(length_);
      glGetShaderInfoLog(shader_, length_, nullptr, infoLog_.data());
      throw std::runtime_error(infoLog_.data());
    }
  }

  size_t Shader::getShadingLanguageVersion()
  {
    const char* versionStr{
      reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION)) };
    if (!versionStr)
    {
      throw std::runtime_error{ "Failed to get GLSL version" };
    }
    // Assume format is like "1.40 - Build 31.0.101.5186" or "4.50 NVIDIA"
    double version{ 0.0 };
    if (std::sscanf(versionStr, "%lf", &version) != 1)
    {
      throw std::runtime_error{ "Failed to parse GLSL version" };
    }
    return static_cast<size_t>(version * 100);
  }

} // namespace gl
