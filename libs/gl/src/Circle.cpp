//  Copyright (c) 2024 Daniel Moreno. All rights reserved.
//

#include <gl/Circle.hpp>

#include <stdexcept>
#include <cmath>

constexpr static double PI{ 3.14159265358979323846 };

namespace gl
{
  Circle::Circle(size_t numSegments) :
    program_{ nullptr },
    VBO{ 0 },
    VAO{ 0 },
    initialized_{ false },
    numSegments_{ numSegments },
    color_{ 1.0f, 0.0f, 0.0f, 1.0f },
    center_{ 0.0f, 0.0f, 0.0f },
    radius_{ 1.0f }
  {

  }

  Circle::~Circle()
  {
    deinitGL();
  }

  void Circle::initGL()
  {
    if (initialized_)
    { // Already initialized
      return;
    }

    if (!program_)
    {
      const size_t glslVersion{ gl::Shader::getShadingLanguageVersion() };
      std::string vertexShader, fragmentShader;
      if (glslVersion == 140)
      {
        vertexShader =
          "#version 140\n"
          "attribute vec2 aPos;\n"
          "uniform vec3 uCenter;\n"
          "uniform float uRadius;\n"
          "void main()\n"
          "{\n"
          "  gl_Position = vec4(vec3(aPos * uRadius, 0.0) + uCenter, 1.0);\n"
          "}\n"
          ;
        fragmentShader =
          "#version 140\n"
          "uniform vec4 uColor;\n"
          "void main()\n"
          "{\n"
          "  gl_FragColor = uColor;\n"
          "}\n"
          ;
      }
      else if (glslVersion > 140)
      {
        vertexShader =
          "#version 150\n"
          "in vec2 aPos;\n"
          "uniform vec3 uCenter;\n"
          "uniform float uRadius;\n"
          "void main()\n"
          "{\n"
          "  gl_Position = vec4(vec3(aPos * uRadius, 0.0) + uCenter, 1.0);\n"
          "}\n"
          ;
        fragmentShader =
          "#version 150\n"
          "uniform vec4 uColor;\n"
          "out vec4 FragColor;\n"
          "void main()\n"
          "{\n"
          "  FragColor = uColor;\n"
          "}\n"
          ;
      }
      else
      {
        throw std::runtime_error("Unsupported GLSL version");
      }
      program_ = std::make_unique<gl::Program>(vertexShader, fragmentShader);
    }

    // Configure the vertex buffer object (VBO)
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    const std::vector<float> vertices{ generateVertices_() };
    glBufferData(GL_ARRAY_BUFFER,
      sizeof(float)*vertices.size(), vertices.data(), GL_STATIC_DRAW);

    // Configure the vertex array object (VAO)
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Position attribute
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
    glEnableVertexAttribArray(0);

    // Clean up
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Set the initialized flag
    initialized_ = true;
  }

  void Circle::deinitGL()
  {
    if (initialized_)
    {
      glDeleteVertexArrays(1, &VAO);
      glDeleteBuffers(1, &VBO);
    }
  }

  void Circle::drawGL() const
  {
    if (!initialized_)
    {
      throw std::runtime_error("Circle not initialized");
    }

    // Bind the VAO
    glBindVertexArray(VAO);

    // Bind the VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Use the shader program
    program_->use();

    // Set the uniforms
    program_->setUniform3f("uCenter", center_);
    program_->setUniform1f("uRadius", radius_);
    program_->setUniform4f("uColor", color_);

    // Draw the vertices
    glDrawArrays(GL_TRIANGLE_FAN, 0, numSegments_ + 1);

    // Unbind the VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Unbind the VAO
    glBindVertexArray(0);

    // Unuse the shader program
    program_->unuse();
  }

  void Circle::setColor(float r, float g, float b, float a)
  {
    color_[0] = r;
    color_[1] = g;
    color_[2] = b;
    color_[3] = a;
  }

  void Circle::setRadius(float radius)
  {
    radius_ = radius;
  }

  void Circle::setCenter(float x, float y, float z)
  {
    center_[0] = x;
    center_[1] = y;
    center_[2] = z;
  }

  std::vector<float> Circle::generateVertices_() const
  {
    const double angleStep{ 2 * PI / (numSegments_ - 1) };
    std::vector<float> vertices;
    vertices.reserve(2 + numSegments_ * 2);
    vertices.emplace_back(0.0f);
    vertices.emplace_back(0.0f);
    for (size_t i = 0; i < numSegments_; ++i)
    {
      const double angle{ i * angleStep };
      const float x{ static_cast<float>(std::cos(angle)) };
      const float y{ static_cast<float>(std::sin(angle)) };
      vertices.emplace_back(x);
      vertices.emplace_back(y);
    }
    return vertices;
  }

} // namespace gl
