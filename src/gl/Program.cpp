//  Copyright (c) 2025 Daniel Moreno. All rights reserved.
//

#include <gl/gl.h>
#include <gl/Program.hpp>

#include <vector>
#include <stdexcept>

namespace gl
{
  Program::Program()
  {
    program_ = glCreateProgram();
  }

  Program::~Program()
  {
    glDeleteProgram(program_);
  }

  Program::Program(
    const std::string& vertexShader,
    const std::string& fragmentShader,
    const std::vector<std::pair<unsigned, std::string>>& attributes)
  {
    program_ = glCreateProgram();
    attachShader(Shader(GL_VERTEX_SHADER));
    shaders_.back().source(vertexShader.c_str());
    shaders_.back().compile();
    attachShader(Shader(GL_FRAGMENT_SHADER));
    shaders_.back().source(fragmentShader.c_str());
    shaders_.back().compile();
    bindAttributeLocations(attributes);
    link();
  }

  void Program::attachShader(Shader shader)
  {
    shaders_.emplace_back(std::move(shader));
    glAttachShader(program_, shaders_.back().get());
  }

  void Program::link()
  {
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
