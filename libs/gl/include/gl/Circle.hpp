//  Copyright (c) 2024-2026 Daniel Moreno. All rights reserved.
//

#pragma once

#include <gl/Shape.hpp>
#include <gl/Program.hpp>

#include <memory>
#include <array>

namespace gl
{
  class Circle : public Shape
  {
    std::unique_ptr<gl::Program> program_;
    GLuint VBO, VAO;
    bool initialized_;
    size_t numSegments_;
    std::array<float, 4> color_;
    std::array<float, 3> center_;
    float radius_;
  public:
    Circle(size_t numSegments = 36);

    ~Circle();

    void initGL() override;

    void deinitGL() override;

    void drawGL() const override;

    void setColor(float r, float g, float b, float a = 1.0f);

    void setCenter(float x, float y, float z = 0.0f);

    void setRadius(float radius);

  private:
    std::vector<float> generateVertices_() const;
  };
} // namespace gl
