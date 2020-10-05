#include "Shader.h"
#include <glad/glad.h>
#include <iostream>
//----------------------------------------------------------------------------------------


// konstruktor
Shader::Shader(const char* vertex_location, const char* fragment_location)
{
    id_ = 0;
    this->vertex_location_ = vertex_location;
    this->fragment_location_ = fragment_location;

    initialize_program();
}//----------------------------------------------------------------------------------------

// wczytuje shader wierzcholkow i uruchamia
int Shader::initialize_vertex_shader()
{
    int success;
    const auto temporary = load_shader(vertex_location_);
    const auto* src_buf = temporary.c_str();
    const int vertex_shader = glCreateShader(GL_VERTEX_SHADER);  // tworzy obiekt shadera fragmentow
    glShaderSource(vertex_shader, 1, &src_buf, nullptr); // wczytuje kod shadera
    glCompileShader(vertex_shader); // kompilacja shadera
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success); // zwraca do inta czy udalo sie skompilowac
    if (!success)
    {
        GLint log_length;
        glGetShaderiv(vertex_shader, GL_INFO_LOG_LENGTH, &log_length);
        auto* log = new char[log_length];
        glGetShaderInfoLog(vertex_shader, log_length, nullptr, log); // wczytuje logi shadera
        std::cout<<"ERROR: SHADER: Blad kompilacji shadera wierzcholkow, log = "<< log <<std::endl;
        std::cout << "ERROR: SHADER: Lokalizacja = " << vertex_location_ << std::endl;
        delete[] log;
    }
    else
    {
        std::cout << "INFO: SHADER: Shader wierzcholkow wczytany pomyslnie = " << vertex_location_ << std::endl;
    }
    return vertex_shader;
}//----------------------------------------------------------------------------------------=

// wczytuje shader fragmentow i uruchamia
int Shader::initialize_fragment_shader()
{
    int success;
    const auto temporary = load_shader(fragment_location_);
    const auto* src_buf = temporary.c_str();
    const int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);  // tworzy obiekt shadera fragmentow

    glShaderSource(fragment_shader, 1, &src_buf, nullptr); // wczytuje kod shadera
    glCompileShader(fragment_shader); // kompilacja shadera
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success); // zwraca do inta czy udalo sie skompilowac

    if (!success)
    {
        GLint log_length;
        glGetShaderiv(fragment_shader, GL_INFO_LOG_LENGTH, &log_length);
        auto* log = new char[log_length];
        glGetShaderInfoLog(fragment_shader, log_length, nullptr, log); // wczytuje logi shadera
        std::cout << "ERROR: SHADER: Blad kompilacji shadera fragmentow, log = " << log << std::endl;
        std::cout << "ERROR: SHADER: Lokalizacja = " << fragment_location_ << std::endl;
        delete[] log;
    }
    else
    {
        std::cout << "INFO: SHADER: Shader fragmentow wczytany pomyslnie = " << fragment_location_<< std::endl;
    }
    return fragment_shader;
}//----------------------------------------------------------------------------------------=

// wczytuje program, linkuje shadery
void Shader::initialize_program()
{
    int success;
    const auto vertex_shader = initialize_vertex_shader();
    const auto fragment_shader = initialize_fragment_shader();
    const int shader_program = glCreateProgram(); // stworzenie programu shadera

    glAttachShader(shader_program, vertex_shader); // dolacza shader wierzcholkow
    glAttachShader(shader_program, fragment_shader); // dolacza shader fragmentow
    glLinkProgram(shader_program); // linkowanie programu
    glGetProgramiv(shader_program, GL_LINK_STATUS, &success); // sprawdza czy udalo sie

    if (!success) {
        GLint log_length;
        glGetProgramiv(shader_program, GL_INFO_LOG_LENGTH, &log_length);
        auto* log = new char[log_length];
        glGetProgramInfoLog(shader_program, log_length, nullptr, log); // wczytuje logi shadera
        std::cout << "ERROR: SHADER: Blad kompilacji linkera, log = " << log << std::endl;
        delete[] log;
    }
    else
    {
        std::cout << "INFO: SHADER: Program zlinkowany pomyslnie, id = " << shader_program << std::endl;
    }
    glDeleteShader(vertex_shader); // usuniecie obiektu shadera wierzcholkow
    glDeleteShader(fragment_shader); // usuniecie obiektu shadera fragmentow
    id_ = shader_program;
}//----------------------------------------------------------------------------------------=

// wczytywanie pliku shadera
std::string Shader::load_shader(const char* file_name)
{
    std::string shader_code;
    std::ifstream shader_file;
    shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        shader_file.open(file_name);
        std::stringstream shader_stream;
        shader_stream << shader_file.rdbuf();
        shader_file.close();
        shader_code = shader_stream.str();
    }
    catch (std::ifstream::failure e)
    {
        std::cout << "ERROR: SHADER: Nie mozna otworzyc pliku glsl, lokalizacja = "<< file_name << std::endl;
    }
    return shader_code;
}//----------------------------------------------------------------------------------------

// uzywa tego programu shadera
void Shader::use() const
{
    glUseProgram(id_);
}//----------------------------------------------------------------------------------------

// setter uniforma
void Shader::setBool(const std::string& name, bool value) const
{
    glUniform1i(glGetUniformLocation(id_, name.c_str()), (int)value);
}//----------------------------------------------------------------------------------------

void Shader::setInt(const std::string& name, int value) const
{
    glUniform1i(glGetUniformLocation(id_, name.c_str()), value);
}//----------------------------------------------------------------------------------------

void Shader::setFloat(const std::string& name, float value) const
{
    glUniform1f(glGetUniformLocation(id_, name.c_str()), value);
}//----------------------------------------------------------------------------------------

void Shader::setVec2(const std::string& name, const glm::vec2& value) const
{
    glUniform2fv(glGetUniformLocation(id_, name.c_str()), 1, &value[0]);
}//----------------------------------------------------------------------------------------

void Shader::setVec2(const std::string& name, float x, float y) const
{
    glUniform2f(glGetUniformLocation(id_, name.c_str()), x, y);
}//----------------------------------------------------------------------------------------

void Shader::setVec3(const std::string& name, const glm::vec3& value) const
{
    glUniform3fv(glGetUniformLocation(id_, name.c_str()), 1, &value[0]);
}//----------------------------------------------------------------------------------------

void Shader::setVec3(const std::string& name, float x, float y, float z) const
{
    glUniform3f(glGetUniformLocation(id_, name.c_str()), x, y, z);
}//----------------------------------------------------------------------------------------

void Shader::setVec4(const std::string& name, const glm::vec4& value) const
{
    glUniform4fv(glGetUniformLocation(id_, name.c_str()), 1, &value[0]);
}//----------------------------------------------------------------------------------------

void Shader::setVec4(const std::string& name, float x, float y, float z, float w) const
{
    glUniform4f(glGetUniformLocation(id_, name.c_str()), x, y, z, w);
}//----------------------------------------------------------------------------------------

void Shader::setMat2(const std::string& name, const glm::mat2& mat) const
{
    glUniformMatrix2fv(glGetUniformLocation(id_, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}//----------------------------------------------------------------------------------------

void Shader::setMat3(const std::string& name, const glm::mat3& mat) const
{
    glUniformMatrix3fv(glGetUniformLocation(id_, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}//----------------------------------------------------------------------------------------

void Shader::setMat4(const std::string& name, const glm::mat4& mat) const
{
    glUniformMatrix4fv(glGetUniformLocation(id_, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}//----------------------------------------------------------------------------------------

int Shader::get_id()
{
    return id_;
}//----------------------------------------------------------------------------------------