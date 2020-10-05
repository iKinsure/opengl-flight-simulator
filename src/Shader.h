#pragma once
#ifndef SHADER_H
#define SHADER_H
//----------------------------------------------------------------------------------------
#include <string>
#include "glm/mat2x2.hpp"
#include <fstream>
#include <sstream>
//----------------------------------------------------------------------------------------


class Shader
{
private:

	int id_; // id progrmau shadera
	const char* vertex_location_;
	const char* fragment_location_;

	int initialize_vertex_shader(); // uruchamianie shadera wierzcholkow
	int initialize_fragment_shader(); // uruchamianie shadera fragmentow
	std::string load_shader(const char* file_name); // wczytywanie z pliku
	void initialize_program(); // linkowanie shaderow i uruchamianie ich

public:

	Shader(const char* vertex_location, const char* fragment_location);
	
	void use() const; // uzywa tego shadera poprzez id

	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
	void setVec2(const std::string& name, const glm::vec2& value) const;
	void setVec2(const std::string& name, float x, float y) const;
	void setVec3(const std::string& name, const glm::vec3& value) const;
	void setVec3(const std::string& name, float x, float y, float z) const;
	void setVec4(const std::string& name, const glm::vec4& value) const;
	void setVec4(const std::string& name, float x, float y, float z, float w) const;
	void setMat2(const std::string& name, const glm::mat2& mat) const;
	void setMat3(const std::string& name, const glm::mat3& mat) const;
	void setMat4(const std::string& name, const glm::mat4& mat) const;
	int get_id(); // zwraca id shadera
};

#endif