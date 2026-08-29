//  Copyright (c) 2026 Daniel Moreno. All rights reserved.
//

#include <gui2/Application.hpp>
#include <gui2/Panel.hpp>
#include <gui2/Layout.hpp>
#include <gui2/Separator.hpp>
#include <gui2/Button.hpp>

#include <gui2/WidgetGL.hpp>

#include <gl/Program.hpp>

#include <timer/Timer.hpp>

#include <print>

// This example shows how to use OpenGL shaders in a GUI application.
//

using namespace gui2;

class ViewerGL : public WidgetGL
{
public:
  virtual void initialize(const Vec2i& availableSize) override
  {
    WidgetGL::initialize(availableSize);
    initializeShaders_();
    startAnimation();
  }

  virtual void drawGL() override
  {
    WidgetGL::drawGL();
    drawTriangle_();
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

  const std::string& getGlslVersionString() const
  {
    return glslVersionString_;
  }

  ~ViewerGL()
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
  gl::Program program_;
  GLuint VBO, VAO;
  timer::Timer timer_;
  float angle_ = 0.0f;
  std::string glslVersionString_;

  void initializeShaders_()
  {
    // Initialize the shader program
    const size_t glslVersion{ gl::Shader::getShadingLanguageVersion() };
    glslVersionString_ = std::to_string(glslVersion);
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

int main(int argc, char** argv)
{
  // Title of the window
  std::string title{"GUI: Hello Shaders v2!"};

  // Create the application
  Application app{ title };

  // Get the window
  Window& window = app.getWindow();

  // Create the OpenGL widget
  ViewerGL viewerGL;

  // Set the content of the window to include the OpenGL widget and other
  // GUI elements
  window.setContent(
    HBox{
      VBox{
        // Main panel
        Stretch{3, Panel{ VStack{
          "Main Panel",
          Separator{},
          std::ref(viewerGL),
        }}},
        // Bottom panel
        Stretch{1, Panel{ VStack{
          "Bottom Panel",
          Separator{},
          [&]{ return viewerGL.isAnimationRunning()
            ? "Animation is running" : "Animation is stopped"; },
          [&]{ return "GLSL Version: " + viewerGL.getGlslVersionString(); },
        }}},
      },
      // Side panel
      Fixed{250, Panel{ VStack{
        "Side Panel",
        Separator{},
        [&]{ return viewerGL.isAnimationRunning()
          ? Button{ "Stop Animation", [&]{ viewerGL.stopAnimation(); } }
          : Button{ "Start Animation", [&]{ viewerGL.startAnimation(); } };
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
