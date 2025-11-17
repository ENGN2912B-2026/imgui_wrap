//  Copyright (c) 2024-2025 Daniel Moreno. All rights reserved.
//

#pragma once

#include <gl/Shader.hpp>

#include <array>
#include <vector>
#include <string>

namespace gl
{
  // OpenGL shader program class
  // ---------------------------
  // This class encapsulates an OpenGL shader program, allowing the user to
  // create a program from vertex and fragment shader source code, attach
  // shaders, link the program, and set uniform variables.
  class Program
  {
    unsigned program_;
    std::vector<Shader> shaders_;
  public:
    // Default constructor
    // -------------------
    // Creates an empty shader program. The program handle can be obtained using
    // the `get()` method. Users can attach shaders using the `attachShader()`
    // method and link the program using the `link()` method.
    Program();

    // Destructor
    // ----------
    // Deletes the shader program.
    ~Program();

    // Construct with shaders
    // ----------------------
    // Creates a shader program by compiling and linking the provided vertex
    // and fragment shader source code. The `attributes` parameter allows
    // specifying attribute locations before linking the program.
    Program(
      const std::string& vertexShader,
      const std::string& fragmentShader,
      const std::vector<std::pair<unsigned, std::string>>& attributes = {});

    void attachShader(Shader shader);
    void link();
    void use() const;
    void unuse() const;

    // Binds attribute locations before linking the program
    void bindAttributeLocations(
      const std::vector<std::pair<unsigned, std::string>>& attributes);

    // Set uniform variables
    void setUniform1i(const char* name, int32_t value) const;
    void setUniform2i(const char* name, const std::array<int32_t,2>& value) const;
    void setUniform3i(const char* name, const std::array<int32_t,3>& value) const;
    void setUniform4i(const char* name, const std::array<int32_t,4>& value) const;

    void setUniform1f(const char* name, float value) const;
    void setUniform2f(const char* name, const std::array<float,2>& value) const;
    void setUniform3f(const char* name, const std::array<float,3>& value) const;
    void setUniform4f(const char* name, const std::array<float,4>& value) const;

    // Returns the OpenGL program handle
    unsigned get() const { return program_; }
  };
} // namespace gl
