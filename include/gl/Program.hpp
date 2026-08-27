//  Copyright (c) 2024-2026 Daniel Moreno. All rights reserved.
//

#pragma once

#include <gl/Shader.hpp>

#include <array>
#include <vector>
#include <string>

namespace gl
{
  //! \brief An OpenGL shader program class
  //!
  //! This class encapsulates an OpenGL shader program, allowing the user to
  //! create a program from vertex and fragment shader source code, attach
  //! shaders, link the program, and set uniform variables.
  class Program
  {
  public:
    //! \brief Default constructor.
    Program() = default;

    //! \brief Construct with shader objects.
    //! \param[in] vertexShader  The vertex shader object.
    //! \param[in] fragmentShader  The fragment shader object.
    //! \param[in] attributes  A vector of pairs specifying attribute locations
    //!                        before linking the program. Each pair consists of
    //!                        an attribute index and its corresponding name.
    //! \throw std::invalid_argument if the vertex or fragment shader is not
    //!        compiled.
    //! \throw std::runtime_error if the program fails to link.
    //!
    //! Creates a shader program by attaching the provided vertex and fragment
    //! shader objects, and linking the program. The `attributes` parameter
    //! allows  specifying attribute locations before linking the program.
    Program(const Shader& vertexShader, const Shader& fragmentShader,
            const std::vector<std::pair<GLuint, std::string>>& attributes = {});

    //! \brief Construct with shader source code.
    //! \param[in] vertexShader  The source code of the vertex shader.
    //! \param[in] fragmentShader  The source code of the fragment shader.
    //! \param[in] attributes  A vector of pairs specifying attribute locations
    //!                        before linking the program. Each pair consists of
    //!                        an attribute index and its corresponding name.
    //! \throw std::runtime_error if the shaders fail to compile or the program
    //!        fails to link.
    //!
    //! Creates a shader program by compiling and linking the provided vertex
    //! and fragment shader source code. The `attributes` parameter allows
    //! specifying attribute locations before linking the program.
    Program(const std::string& vertexShader, const std::string& fragmentShader,
            const std::vector<std::pair<GLuint, std::string>>& attributes = {});

    //! \brief Copy constructor.
    Program(const Program&) = delete;

    //! \brief Move constructor.
    Program(Program&&) noexcept;

    //! \brief Destructor.
    ~Program();

    //! \brief Copy assignment operator.
    Program& operator=(const Program&) = delete;

    //! \brief Move assignment operator.
    Program& operator=(Program&&) noexcept;

    //! \brief Checks if the shader program is initialized.
    //! \return true if the shader program is initialized, false otherwise.
    bool isInitialized() const { return program_ > 0; }

    //! \brief Initializes the shader program.
    //! \note If the program is already initialized, this method does nothing.
    void initialize();

    //! \brief Initializes the shader program with the given vertex and fragment
    //!        shader objects and optional attribute locations.
    //! \param[in] vertexShader  The vertex shader object.
    //! \param[in] fragmentShader  The fragment shader object.
    //! \param[in] attributes  A vector of pairs specifying attribute locations
    //!                        before linking the program. Each pair consists of
    //!                        an attribute index and its corresponding name.
    //! \throw std::invalid_argument if the vertex or fragment shader is not
    //!        compiled.
    //! \throw std::runtime_error if the program fails to link.
    //!
    //! This method initializes the shader program by attaching the provided
    //! vertex and fragment shader objects, and linking the program. The
    //! `attributes` parameter allows specifying attribute locations before
    //! linking the program. If the program is already initialized, it will be
    //! re-initialized with the new shaders and attribute locations.
    void initialize(
      const Shader& vertexShader,
      const Shader& fragmentShader,
      const std::vector<std::pair<GLuint, std::string>>& attributes = {});

    //! \brief Initializes the shader program with the given vertex and fragment
    //!        shader source code and optional attribute locations.
    //! \param[in] vertexShader  The source code of the vertex shader.
    //! \param[in] fragmentShader  The source code of the fragment shader.
    //! \param[in] attributes  A vector of pairs specifying attribute locations
    //!                        before linking the program. Each pair consists of
    //!                        an attribute index and its corresponding name.
    //! \throw std::runtime_error if the shaders fail to compile or the program
    //!        fails to link.
    //!
    //! This method initializes the shader program by compiling and linking the
    //! provided vertex and fragment shader source code. The `attributes`
    //! parameter allows specifying attribute locations before linking the
    //! program. If the program is already initialized, it will be
    //! re-initialized with the new shaders and attribute locations.
    void initialize(
      const std::string& vertexShader,
      const std::string& fragmentShader,
      const std::vector<std::pair<GLuint, std::string>>& attributes = {});

    //! \brief Uninitializes the shader program.
    //! \note If the program is not initialized, this method does nothing.
    void uninitialize();

    //! \brief Attaches a shader to the program.
    //! \param[in] shader  The shader object to attach.
    //! \throw std::invalid_argument if the shader is not compiled.
    //!
    //! This method attaches the provided shader object to the program. The
    //! shader must be compiled before being attached. If the program is not
    //! initialized, it will be initialized before attaching the shader.
    void attachShader(const Shader& shader);

    //! \brief Checks if the shader program is linked.
    //! \return true if the shader program is linked, false otherwise.
    bool isLinked() const;

    //! \brief Links the shader program.
    //! \throw std::runtime_error if the program is not initialized or if the
    //!        linking fails.
    //!
    //! This method links the shader program after all shaders have been
    //! attached.
    void link();

    //! \brief Uses the shader program for rendering.
    void use() const;

    //! \brief Stops using the shader program for rendering.
    void unuse() const;

    //! \brief Binds attribute locations before linking the program.
    //! \param[in] attributes  A vector of pairs specifying attribute locations
    //!                        before linking the program. Each pair consists of
    //!                        an attribute index and its corresponding name.
    void bindAttributeLocations(
      const std::vector<std::pair<GLuint, std::string>>& attributes);

    // Set uniform variables
    void setUniform1i(const char* name, int32_t value) const;
    void setUniform2i(const char* name, const std::array<int32_t,2>& value) const;
    void setUniform3i(const char* name, const std::array<int32_t,3>& value) const;
    void setUniform4i(const char* name, const std::array<int32_t,4>& value) const;

    void setUniform1f(const char* name, float value) const;
    void setUniform2f(const char* name, const std::array<float,2>& value) const;
    void setUniform3f(const char* name, const std::array<float,3>& value) const;
    void setUniform4f(const char* name, const std::array<float,4>& value) const;

    //! \brief Gets the OpenGL program handle.
    //! \return The OpenGL program handle.
    GLuint getId() const { return program_; }

  private:
    GLuint program_ = 0;
  };
} // namespace gl
