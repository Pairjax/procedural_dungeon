#ifndef SHADER_HEADER
#define SHADER_HEADER

#include <GL/glew.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader {
public:
    // Shader Program ID
    unsigned int ID;

    Shader();
    // Reads & Builds Shader
    Shader(const char* vertex_path, const char* fragment_path);
    // Use/activate the shader
    void use();

    // Sets values for the shader uniform
    void set_bool(const std::string& name, bool value) const;
    void set_int(const std::string& name, int value) const;
    void set_float(const std::string& name, float value) const;
};

#endif