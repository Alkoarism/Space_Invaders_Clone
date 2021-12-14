#include "OpenGL/shader.h"

// constructor generates the shader on the fly
// ------------------------------------------------------------------------
Shader::Shader(const char* vertexPath, const char* fragmentPath) {
    // 1. retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    // ensure ifstream objects can throw exceptions:
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        // open files
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;

        // read file's buffer contents into streams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        
        // close file handlers
        vShaderFile.close();
        fShaderFile.close();
        
        // convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }

    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();
    
    // 2. compile shaders
    unsigned int vertex, fragment;

    // vertex shader and it�s error handling
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    CheckCompileErrors(vertex, "VERTEX");

    // fragment Shader and it�s error handling
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    CheckCompileErrors(fragment, "FRAGMENT");

    // shader Program
    m_shaderID = glCreateProgram();
    glAttachShader(m_shaderID, vertex);
    glAttachShader(m_shaderID, fragment);
    glLinkProgram(m_shaderID);
    CheckCompileErrors(m_shaderID, "PROGRAM");

    // delete the shaders as they're linked
    // into the program and are no longer necessary
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

Shader::~Shader() {
    Release();
}

Shader::Shader(Shader&& other) noexcept 
    :   m_shaderID(other.m_shaderID) {
    other.m_shaderID = 0;
}

Shader& Shader::operator=(Shader&& other) noexcept {
    if (this != &other) {
        Release();
        std::swap(m_shaderID, other.m_shaderID);
    }
    return *this;
}

// activate the shader
// ------------------------------------------------------------------------
void Shader::Use() const{
    glUseProgram(m_shaderID);
}

// utility uniform functions
// ------------------------------------------------------------------------
void Shader::SetGLUniform(const std::string& name, const bool& value) const {
    glUniform1i(glGetUniformLocation(m_shaderID, name.c_str()), (int)value);
}

void Shader::SetGLUniform(const std::string& name, const int& value) const {
    glUniform1i(glGetUniformLocation(m_shaderID, name.c_str()), value);
}

void Shader::SetGLUniform(const std::string& name, const float& value) const {
    glUniform1f(glGetUniformLocation(m_shaderID, name.c_str()), value);
}

void Shader::SetGLUniform(const std::string& name, const glm::mat4& value) const {
    glUniformMatrix4fv
        (glGetUniformLocation(m_shaderID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::GetGLUniform(const std::string& name, int& value) const {
    glGetUniformiv(m_shaderID, glGetUniformLocation(m_shaderID, name.c_str()), &value);
}

void Shader::GetGLUniform(const std::string& name, float& value) const {
    glGetUniformfv(m_shaderID, glGetUniformLocation(m_shaderID, name.c_str()), &value);
}

// utility function for checking shader compilation/linking errors.
// ------------------------------------------------------------------------
void Shader::CheckCompileErrors(unsigned int shader, std::string type) {
    int success;
    char infoLog[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " 
                << type << "\n" << infoLog 
                << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " 
                << type << "\n" << infoLog 
                << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
}