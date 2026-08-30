//  Copyright (c) 2024-2026 Daniel Moreno. All rights reserved.
//

#pragma once

#include <gl/Shape.hpp>
#include <gl/Program.hpp>

#include <memory>
#include <array>

namespace gl
{
  class Sphere : public Shape
  {
    std::unique_ptr<gl::Program> program_;
    GLuint VBO, VAO;
    bool initialized_;
    size_t latitudes_;
    size_t longitudes_;
    std::array<float, 4> color_;
    std::array<float, 3> center_;
    float radius_;
    std::array<float, 3> lightPosition_;
  public:
    Sphere(size_t latitudes = 60, size_t longitudes = 60);

    ~Sphere();

    void initGL() override;

    void deinitGL() override;

    void drawGL() const override;

    void setColor(float r, float g, float b, float a = 1.0f);

    void setCenter(float x, float y, float z = 0.0f);

    void setRadius(float radius);

    void setLightPosition(float x, float y, float z);

  private:
    std::vector<float> generateVertices_() const;
  };
} // namespace gl
