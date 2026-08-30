//  Copyright (c) 2024-2026 Daniel Moreno. All rights reserved.
//

#pragma once

#include <gl/Handler.hpp>

#include <string>

namespace gl
{
  //! \brief A simple shader class that can be used to manage OpenGL shaders.
  class Shader : public Handler
  {
  public:
    //! \brief Deleted copy constructor and copy assignment operator, and
    //!        default move constructor and move assignment operator.
    GL_NO_COPY_DEFAULT_MOVE(Shader)

    //! \brief Default constructor.
    Shader() = default;

    //! \brief Constructs a shader of the specified type.
    //! \param[in] type The type of shader to create. This is a valid OpenGL
    //!             shader type, such as GL_VERTEX_SHADER or GL_FRAGMENT_SHADER.
    Shader(GLuint type);

    //! \brief Constructs a shader of the specified type with the given source
    //!        code.
    //! \param[in] type The type of shader to create. This is a valid OpenGL
    //!                 shader type, such as GL_VERTEX_SHADER or
    //!                 GL_FRAGMENT_SHADER.
    //! \param[in] source The source code of the shader.
    //! \throw std::invalid_argument if the source is null.
    //! \throw std::runtime_error if the shader fails to compile.
    Shader(GLuint type, const std::string& source);

    //! \brief Destructor.
    ~Shader() { uninitialize(); }

    //! \brief Initializes a shader of the specified type.
    //! \param[in] type The type of shader to create. This is a valid OpenGL
    //!             shader type, such as GL_VERTEX_SHADER or GL_FRAGMENT_SHADER.
    //! \note If the shader is already initialized, this method does nothing.
    void initialize(GLuint type);

    //! \brief Initializes a shader of the specified type with the given source code.
    //! \param[in] type The type of shader to create. This is a valid OpenGL
    //!                 shader type, such as GL_VERTEX_SHADER or
    //!                 GL_FRAGMENT_SHADER.
    //! \param[in] source The source code of the shader.
    //! \throw std::invalid_argument if the source is null.
    //! \throw std::runtime_error if the shader fails to compile.
    //!
    //! This method initializes the shader and sets its source code. If the
    //! shader is already initialized, it will be re-initialized with the new
    //! type and source code.
    void initialize(GLuint type, const std::string& source);

    //! \brief Uninitializes the shader.
    //! \note If the shader is not initialized, this method does nothing.
    void uninitialize();

    //! \brief Checks if the shader has source code set.
    //! \return true if the shader has source code set, false otherwise.
    bool hasSource() const;

    //! \brief Sets the source code of the shader.
    //! \param[in] source The source code of the shader.
    //! \throw std::invalid_argument if the source is null.
    void setSource(const std::string& source);

    //! \brief Gets the source code of the shader.
    //! \return The source code of the shader as a string.
    // \throw std::runtime_error if the shader is not initialized.
    std::string getSource() const;

    //! \brief Checks if the shader is compiled.
    //! \return true if the shader is compiled, false otherwise.
    bool isCompiled() const;

    //! \brief Compiles the shader.
    //! \throw std::runtime_error if the shader is not initialized, has no
    //!        source code, or fails to compile.
    void compile();

    //! \brief Gets the shading language version supported by the OpenGL context.
    //! \return The shading language version as an integer, e.g., 140 for
    //!         GLSL 1.40, 150 for GLSL 1.50, etc.
    //! \throw std::runtime_error if the shading language version cannot be
    //!        determined.
    static size_t getShadingLanguageVersion();
  };

} // namespace gl
