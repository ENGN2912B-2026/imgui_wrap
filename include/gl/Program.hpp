//  Copyright (c) 2024-2026 Daniel Moreno. All rights reserved.
//

#pragma once

#include <gl/Shader.hpp>
#include <gl/AutoUnbind.hpp>

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
  class Program : public Handler
  {
  public:
    //! \brief AutoUnbind is a helper class that automatically unbinds the
    //!        program when it goes out of scope. This is useful for ensuring
    //!        that the program is properly unbound, even if an exception is
    //!        thrown.
    using AutoUnbind = gl::AutoUnbind<Program>;

    //! \brief Deleted copy constructor and copy assignment operator, and
    //!        default move constructor and move assignment operator.
    GL_NO_COPY_DEFAULT_MOVE(Program)

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

    //! \brief Destructor.
    ~Program() { uninitialize(); }

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
    void use() const { bind(getId()); }

    //! \brief Stops using the shader program for rendering.
    void unuse() const { bind(0); }

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

    //! \brief Binds the program and returns an AutoUnbind object that will
    //!        automatically unbind the program when it goes out of scope.
    //! \return An AutoUnbind object that will automatically unbind the program
    //!         when it goes out of scope.
    AutoUnbind bindScoped() const;

    //! \brief Binds a program.
    //! \param[in] id The OpenGL ID of the program to bind.
    static void bind(GLuint id) { glUseProgram(id); }

    //! \brief Gets the OpenGL ID of the currently bound program.
    //! \return The OpenGL ID of the currently bound program.
    static GLuint getBoundId();
  };

  //! \brief Static assertion to ensure that Program is AutoUnbindable.
  static_assert(AutoUnbindable<Program>, "Program must be AutoUnbindable");

} // namespace gl
