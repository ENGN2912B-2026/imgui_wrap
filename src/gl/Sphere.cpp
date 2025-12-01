//  Copyright (c) 2024-2025 Daniel Moreno. All rights reserved.
//

#include <gl/Sphere.hpp>

#include <stdexcept>
#include <cmath>

constexpr static double PI{ 3.14159265358979323846 };

namespace gl
{
  Sphere::Sphere(size_t latitudes, size_t longitudes)
    : program_{ nullptr }
    , VBO{ 0 }
    , VAO{ 0 }
    , initialized_{ false }
    , latitudes_{ latitudes }
    , longitudes_{ longitudes }
    , color_{ 1.0f, 0.0f, 0.0f, 1.0f }
    , center_{ 0.0f, 0.0f, 0.0f }
    , radius_{ 1.0f }
    , lightPosition_{ -1.0f, 1.0f, -1.0f }
  {

  }

  Sphere::~Sphere()
  {
    deinitGL();
  }

  void Sphere::initGL()
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
          "attribute vec3 aPos;\n"
          "\n"
          "varying vec3 vFragPos;\n"
          "varying vec3 vNormal;\n"
          "\n"
          "uniform vec3 uCenter;\n"
          "uniform float uRadius;\n"
          "\n"
          "void main()\n"
          "{\n"
          "  vFragPos = aPos * uRadius;\n"
          "  vNormal = normalize(aPos);\n"
          "  gl_Position = vec4(vFragPos + uCenter, 1.0);\n"
          "}\n"
          ;
        fragmentShader =
          "#version 140\n"
          "varying vec3 vFragPos;\n"
          "varying vec3 vNormal;\n"
          "\n"
          "uniform vec4 uObjectColor;\n"
          "uniform vec4 uAmbientLightColor;\n"
          "uniform struct Light\n"
          "{\n"
          "  vec4 ambientColor;\n"
          "  vec4 diffuseColor;\n"
          "  vec4 specularColor;\n"
          "  vec3 position;\n"
          "} uLight0;\n"
          "\n"
          "void main()\n"
          "{\n"
          "  // Ambient light and Light0 Ambient component\n"
          "  vec4 ambient = uAmbientLightColor + uLight0.ambientColor;\n"
          "\n"
          "  // Light0 Diffuse component\n"
          "  vec3 lightDir = normalize(uLight0.position - vFragPos);\n"
          "  float diff = max(dot(vNormal, lightDir), 0.0);\n"
          "  vec4 diffuse = diff * uLight0.diffuseColor;\n"
          "\n"
          "  // Light0 Specular component\n"
          "  vec3 viewDir = normalize(-vFragPos);\n"
          "  vec3 reflectDir = reflect(-lightDir, vNormal);\n"
          "  float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);\n"
          "  vec4 specular = spec * uLight0.specularColor;\n"
          "\n"
          "  gl_FragColor = (ambient + diffuse + specular) * uObjectColor;\n"
          "}\n"
          ;
      }
      else if (glslVersion > 140)
      {
        vertexShader =
          "#version 150\n"
          "in vec3 aPos;\n"
          "\n"
          "out vec3 vFragPos;\n"
          "out vec3 vNormal;\n"
          "\n"
          "uniform vec3 uCenter;\n"
          "uniform float uRadius;\n"
          "\n"
          "void main()\n"
          "{\n"
          "  vFragPos = aPos * uRadius;\n"
          "  vNormal = normalize(aPos);\n"
          "  gl_Position = vec4(vFragPos + uCenter, 1.0);\n"
          "}\n"
          ;
        fragmentShader =
          "#version 150\n"
          "in vec3 vFragPos;\n"
          "in vec3 vNormal;\n"
          "\n"
          "out vec4 FragColor;\n"
          "\n"
          "uniform vec4 uObjectColor;\n"
          "uniform vec4 uAmbientLightColor;\n"
          "uniform struct Light\n"
          "{\n"
          "  vec4 ambientColor;\n"
          "  vec4 diffuseColor;\n"
          "  vec4 specularColor;\n"
          "  vec3 position;\n"
          "} uLight0;\n"
          "\n"
          "void main()\n"
          "{\n"
          "  // Ambient light and Light0 Ambient component\n"
          "  vec4 ambient = uAmbientLightColor + uLight0.ambientColor;\n"
          "\n"
          "  // Light0 Diffuse component\n"
          "  vec3 lightDir = normalize(uLight0.position - vFragPos);\n"
          "  float diff = max(dot(vNormal, lightDir), 0.0);\n"
          "  vec4 diffuse = diff * uLight0.diffuseColor;\n"
          "\n"
          "  // Light0 Specular component\n"
          "  vec3 viewDir = normalize(-vFragPos);\n"
          "  vec3 reflectDir = reflect(-lightDir, vNormal);\n"
          "  float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);\n"
          "  vec4 specular = spec * uLight0.specularColor;\n"
          "\n"
          "  FragColor = (ambient + diffuse + specular) * uObjectColor;\n"
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
    glEnableVertexAttribArray(0);

    // Clean up
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Set the initialized flag
    initialized_ = true;
  }

  void Sphere::deinitGL()
  {
    if (initialized_)
    {
      glDeleteVertexArrays(1, &VAO);
      glDeleteBuffers(1, &VBO);
    }
  }

  void Sphere::drawGL() const
  {
    if (!initialized_)
    {
      throw std::runtime_error("Sphere not initialized");
    }

    // Bind the VAO
    glBindVertexArray(VAO);

    // Bind the VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Use the shader program
    program_->use();

    // Set the uniforms ---------------------------------------------

    // Object attributes
    program_->setUniform3f("uCenter", center_);
    program_->setUniform1f("uRadius", radius_);
    program_->setUniform4f("uObjectColor", color_);

    // Ambient light
    const std::array<float, 4> ambientLightColor{ 0.2f, 0.2f, 0.2f, 1.0f };
    program_->setUniform4f("uAmbientLightColor", ambientLightColor);

    // Another light
    const std::array<float, 4> light0_ambientColor{ 0.2f, 0.2f, 0.2f, 1.0f };
    const std::array<float, 4> light0_diffuseColor{ 0.6f, 0.6f, 0.6f, 1.0f };
    const std::array<float, 4> light0_specularColor{ 1.0f, 1.0f, 1.0f, 1.0f };
    program_->setUniform4f("uLight0.ambientColor", light0_ambientColor);
    program_->setUniform4f("uLight0.diffuseColor", light0_diffuseColor);
    program_->setUniform4f("uLight0.specularColor", light0_specularColor);
    program_->setUniform3f("uLight0.position", lightPosition_);

    // Draw the vertices
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 3 * latitudes_ * longitudes_);

    // Unbind the VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Unbind the VAO
    glBindVertexArray(0);

    // Unuse the shader program
    program_->unuse();
  }

  void Sphere::setColor(float r, float g, float b, float a)
  {
    color_[0] = r;
    color_[1] = g;
    color_[2] = b;
    color_[3] = a;
  }

  void Sphere::setRadius(float radius)
  {
    radius_ = radius;
  }

  void Sphere::setCenter(float x, float y, float z)
  {
    center_[0] = x;
    center_[1] = y;
    center_[2] = z;
  }

  void Sphere::setLightPosition(float x, float y, float z)
  {
    lightPosition_[0] = x;
    lightPosition_[1] = y;
    lightPosition_[2] = z;
  }

  std::vector<float> Sphere::generateVertices_() const
  {
    std::vector<float> vertices;
    for (size_t i = 0; i <= latitudes_; ++i)
    {
      const double lat0{ PI * (-0.5 + static_cast<double>(i - 1) / latitudes_) };
      const double z0{ std::sin(lat0) };
      const double zr0{ std::cos(lat0) };

      const double lat1{ PI * (-0.5 + static_cast<double>(i) / latitudes_) };
      const double z1{ std::sin(lat1) };
      const double zr1{ std::cos(lat1) };

      for (int j = 0; j <= longitudes_; ++j)
      {
        const double lng{ 2 * PI * static_cast<double>(j - 1) / longitudes_ };
        const double x{ std::cos(lng) };
        const double y{ std::sin(lng) };

        // position and normal are the same in centered unit sphere

        // vertex 1
        vertices.emplace_back(static_cast<float>(zr0 * x));
        vertices.emplace_back(static_cast<float>(zr0 * y));
        vertices.emplace_back(static_cast<float>(z0));

        // vertex 2
        vertices.emplace_back(static_cast<float>(zr1 * x));
        vertices.emplace_back(static_cast<float>(zr1 * y));
        vertices.emplace_back(static_cast<float>(z1));
      }
    }
    return vertices;
  }

} // namespace gl
