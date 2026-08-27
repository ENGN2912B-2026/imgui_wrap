//  Copyright (c) 2026 Daniel Moreno. All rights reserved.
//

#include <gui2/Application.hpp>
#include <gui2/Panel.hpp>
#include <gui2/Layout.hpp>
#include <gui2/Separator.hpp>
#include <gui2/Button.hpp>
#include <gui2/Image.hpp>

#include <gl/gl.h>
#include <gl/FrameBuffer.hpp>
#include <gl/Program.hpp>

#include <timer/Timer.hpp>

#include <print>

// This example shows how to use OpenGL shaders in a GUI application.
//

using namespace gui2;

class WidgetGL
{
public:
  Rect display(const Runtime& rt, const Rect& rect)
  {
    const Vec2i availableSize{ rect.getAvailableSize() };
    if (!program_.isInitialized())
    {
      initialize_(availableSize);
      startAnimation();
    }
    else if (frameBuffer_.getSize() != availableSize)
    {
      frameBuffer_.setSize(availableSize);
    }
    configureViewport_(availableSize);

    // Draw the OpenGL content into the frame buffer
    drawGL_();

    // Display the frame buffer's texture as an image in the GUI
    return rt.display(Image{ frameBuffer_.getTexture() }, rect);
  }

  bool isAnimationRunning() const
  {
    return timer_.isRunning();
  }

  void startAnimation()
  {
    if (!timer_.isRunning())
    {
      timer_.start(100,
        [this]
        { // timer callback
          angle_ += 7.2f;
          if (angle_ >= 360.0f)
          {
            angle_ -= 360.0f;
          }
        }
      );
    }
  }

  void stopAnimation()
  {
    if (timer_.isRunning())
    {
      timer_.stop();
    }
  }

  ~WidgetGL()
  {
    stopAnimation();
    if (VAO)
    {
      glDeleteVertexArrays(1, &VAO);
    }
    if (VBO)
    {
      glDeleteBuffers(1, &VBO);
    }
  }

private:
  gl::FrameBuffer frameBuffer_;
  gl::Program program_;
  GLuint VBO, VAO;
  timer::Timer timer_;
  float angle_ = 0.0f;

  void initialize_(const Vec2i& frameBufferSize)
  {
    // Initialize the frame buffer
    frameBuffer_.initialize(frameBufferSize, GL_LINEAR);

    // Initialize the shader program
    const size_t glslVersion{ gl::Shader::getShadingLanguageVersion() };
    //std::cout << "GLSL Version: " << glslVersion << std::endl; //TODO show in UI
    std::string vertexShader, fragmentShader;
    if (glslVersion == 140)
    {
      vertexShader =
        "#version 140\n"
        "attribute vec3 aPos;\n"
        "attribute vec3 aColor;\n"
        "\n"
        "varying vec3 vColor;\n"
        "\n"
        "void main()\n"
        "{\n"
        "  gl_Position = vec4(aPos, 1.0);\n"
        "  vColor = aColor;\n"
        "}\n"
        ;
      fragmentShader =
        "#version 140\n"
        "varying vec3 vColor;\n"
        "\n"
        "void main()\n"
        "{\n"
        "  gl_FragColor = vec4(vColor, 1.0);\n"
        "}\n"
        ;
    }
    else if (glslVersion > 140)
    {
      vertexShader =
        "#version 150\n"
        "in vec3 aPos;\n"
        "in vec3 aColor;\n"
        "\n"
        "out vec3 vColor;\n"
        "\n"
        "void main()\n"
        "{\n"
        "  gl_Position = vec4(aPos, 1.0);\n"
        "  vColor = aColor;\n"
        "}\n"
        ;
      fragmentShader =
        "#version 150\n"
        "in vec3 vColor;\n"
        "out vec4 FragColor;\n"
        "\n"
        "void main()\n"
        "{\n"
        "  FragColor = vec4(vColor, 1.0);\n"
        "}\n"
        ;
    }
    else
    {
      throw std::runtime_error{"Unsupported GLSL version"};
    }
    program_.initialize(vertexShader, fragmentShader);

    // Initialize the vertex buffer and vertex array objects
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    // Define the vertex data
    const std::vector<float> vertices{ getTriangleVertices_() };

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER,
      sizeof(float)*vertices.size(), vertices.data(), GL_DYNAMIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
  }

  std::vector<float> getTriangleVertices_() const
  {
    const float factor{ std::cos(angle_ * 3.14159f / 180.0f) };
    return {
        // Positions         // Colors
        -0.6f * factor,  0.75f, 0.0f,  1.0f, 0.0f, 0.0f, // Red
         0.6f * factor,  0.75f, 0.0f,  0.0f, 1.0f, 0.0f, // Green
         0.0f * factor, -0.75f, 0.0f,  0.0f, 0.0f, 1.0f  // Blue
         // We create the triangle with the Y-axis flipped since the frame
         // buffer's texture is flipped vertically when displayed in the GUI.
    };
  }

  void drawGL_()
  {
    frameBuffer_.bind();

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    drawTriangle_();

    frameBuffer_.unbind();
}

  void configureViewport_(const Vec2i& viewportSize)
  {
    glViewport(0, 0, viewportSize.x, viewportSize.y);
  }

  void drawTriangle_()
  {
    // update the vertex data
    const std::vector<float> vertices{ getTriangleVertices_() };

    // Bind the VAO
    glBindVertexArray(VAO);

    // Bind the VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Update the vertex data
    glBufferSubData(
      GL_ARRAY_BUFFER, 0, sizeof(float)*vertices.size(), vertices.data());

    // Unbind the VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Use the shader program
    assert(program_.isLinked() && "Shader program must be linked before use");
    program_.use();

    // Draw the triangle
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);

    // Unuse the shader program
    program_.unuse();
  }
};

static_assert(Displayable<WidgetGL>, "WidgetGL should be a Displayable");


int main(int argc, char** argv)
{
  using namespace gui2;

  // Title of the window
  std::string title{"GUI: Hello Shaders v2!"};

  // Create the application
  Application app{ title };

  // Get the window
  Window& window = app.getWindow();

  // Create the OpenGL widget
  WidgetGL widgetGL;

  // Set the content of the window to include the OpenGL widget and other
  // GUI elements
  window.setContent(
    HBox{
      VBox{
        // Main panel
        Stretch{3, Panel{ VStack{
          "Main Panel",
          Separator{},
          std::ref(widgetGL),
        }}},
        // Bottom panel
        Stretch{1, Panel{ VStack{
          "Bottom Panel",
          Separator{},
          [&]{ return widgetGL.isAnimationRunning()
            ? "Animation is running" : "Animation is stopped"; },
        }}},
      },
      // Side panel
      Fixed{250, Panel{ VStack{
        "Side Panel",
        Separator{},
        [&]{ return widgetGL.isAnimationRunning()
          ? Button{ "Stop Animation", [&]{ widgetGL.stopAnimation(); } }
          : Button{ "Start Animation", [&]{ widgetGL.startAnimation(); } };
        },
      }}},
    }
  );

  // Run the application
  app.run();

  // Success
  std::println("Finished successfully!\n");

  return 0;
}
