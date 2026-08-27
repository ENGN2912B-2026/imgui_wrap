//  Copyright (c) 2025-2026 Daniel Moreno. All rights reserved.
//

#include <gl/Program.hpp>

#include <stdexcept>
#include <cassert>

namespace gl
{
  Program::Program(
    const Shader& vertexShader,
    const Shader& fragmentShader,
    const std::vector<std::pair<unsigned, std::string>>& attributes) : Program{}
  {
    initialize(vertexShader, fragmentShader, attributes);
  }

  Program::Program(
    const std::string& vertexShader,
    const std::string& fragmentShader,
    const std::vector<std::pair<unsigned, std::string>>& attributes) : Program{}
  {
    initialize(vertexShader, fragmentShader, attributes);
  }

  Program::Program(Program&& other) noexcept
  {
    operator=(std::move(other));
  }

  Program::~Program()
  {
    uninitialize();
  }

  Program& Program::operator=(Program&& other) noexcept
  {
    if (this != &other)
    {
      program_ = other.program_;

      // Reset the other program to a default state
      Program empty;
      other.program_ = empty.program_;
    }
    return *this;
  }

  void Program::initialize()
  {
    if (!isInitialized())
    {
      program_ = glCreateProgram();
    }
  }

  void Program::initialize(
    const Shader& vertexShader,
    const Shader& fragmentShader,
    const std::vector<std::pair<unsigned, std::string>>& attributes)
  {
    if (!isInitialized())
    {
      initialize();
    }
    assert(isInitialized() && "Program object must be valid");

    // Attach the shaders and link the program
    attachShader(vertexShader);
    attachShader(fragmentShader);
    bindAttributeLocations(attributes);
    link();
  }

  void Program::initialize(
    const std::string& vertexShader,
    const std::string& fragmentShader,
    const std::vector<std::pair<unsigned, std::string>>& attributes)
  {
    Shader vertexShaderObj{ GL_VERTEX_SHADER, vertexShader };
    Shader fragmentShaderObj{ GL_FRAGMENT_SHADER, fragmentShader };
    initialize(vertexShaderObj, fragmentShaderObj, attributes);
  }

  void Program::uninitialize()
  {
    if (isInitialized())
    {
      glDeleteProgram(program_);
      program_ = 0;
    }
  }

  void Program::attachShader(const Shader& shader)
  {
    if (!shader.isCompiled())
    {
      throw std::invalid_argument{ "ERROR::PROGRAM:: "
        "Cannot attach uncompiled shader!"};
    }
    if (!isInitialized())
    {
      initialize();
    }
    glAttachShader(program_, shader.getId());
  }

  bool Program::isLinked() const
  {
    if (isInitialized())
    {
      GLint status;
      glGetProgramiv(program_, GL_LINK_STATUS, &status);
      return status == GL_TRUE;
    }
    return false;
  }

  void Program::link()
  {
    if (!isInitialized())
    {
      throw std::runtime_error{ "ERROR::PROGRAM:: "
        "Cannot link uninitialized program!"};
    }

    // Link the program
    glLinkProgram(program_);

    // Check the link status
    int status_, length_;
    glGetProgramiv(program_, GL_LINK_STATUS, &status_);
    if (status_ == GL_FALSE)
    {
      glGetProgramiv(program_, GL_INFO_LOG_LENGTH, &length_);
      std::vector<char> infoLog_(length_);
      glGetProgramInfoLog(program_, length_, nullptr, infoLog_.data());
      throw std::runtime_error(infoLog_.data());
    }
  }

  void Program::use() const
  {
    glUseProgram(program_);
  }

  void Program::unuse() const
  {
    glUseProgram(0);
  }

  void Program::bindAttributeLocations(
    const std::vector<std::pair<unsigned, std::string>>& attributes)
  {
    for (const auto& [index, name] : attributes)
    {
      glBindAttribLocation(program_, index, name.c_str());
    }
  }

  void Program::setUniform1i(const char* name, int32_t value) const
  {
    const int location{ glGetUniformLocation(program_, name) };
    glUniform1i(location, value);
  }

  void Program::setUniform2i(
    const char* name, const std::array<int32_t, 2>& value) const
  {
    const int location{ glGetUniformLocation(program_, name) };
    glUniform2i(location, value[0], value[1]);
  }

  void Program::setUniform3i(
    const char* name, const std::array<int32_t, 3>& value) const
  {
    const int location{ glGetUniformLocation(program_, name) };
    glUniform3i(location, value[0], value[1], value[2]);
  }

  void Program::setUniform4i(
    const char* name, const std::array<int32_t, 4>& value) const
  {
    const int location{ glGetUniformLocation(program_, name) };
    glUniform4i(location, value[0], value[1], value[2], value[3]);
  }

  void Program::setUniform1f(const char* name, float value) const
  {
    const int location{ glGetUniformLocation(program_, name) };
    glUniform1f(location, value);
  }

  void Program::setUniform2f(
    const char* name, const std::array<float, 2>& value) const
  {
    const int location{ glGetUniformLocation(program_, name) };
    glUniform2f(location, value[0], value[1]);
  }

  void Program::setUniform3f(
    const char* name, const std::array<float, 3>& value) const
  {
    const int location{ glGetUniformLocation(program_, name) };
    glUniform3f(location, value[0], value[1], value[2]);
  }

  void Program::setUniform4f(
    const char* name, const std::array<float, 4>& value) const
  {
    const int location{ glGetUniformLocation(program_, name) };
    glUniform4f(location, value[0], value[1], value[2], value[3]);
  }

} // namespace gl
