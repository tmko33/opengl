#pragma once
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

namespace cwc
{
    class glShader
    {
    public:
        glShader(GLuint vs, GLuint fs)
        {
            program = glCreateProgram();
            glAttachShader(program, vs);
            glAttachShader(program, fs);
            glLinkProgram(program);

            GLint linked;
            glGetProgramiv(program, GL_LINK_STATUS, &linked);

            if (!linked)
            {
                GLint logLength;
                glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
                char *log = new char[logLength];
                glGetProgramInfoLog(program, logLength, NULL, log);
                std::cerr << "GLSL program link error: " << log << std::endl;
                delete[] log;
            }
        }

        void begin() { glUseProgram(program); }
        void end() { glUseProgram(0); }

        void enable() { glUseProgram(program); }
        void disable() { glUseProgram(0); }

        GLuint GetProgramObject() { return program; }

    private:
        GLuint program;
    };

    class glShaderManager
    {
    public:
        glShader *loadfromFile(const char *vertexPath, const char *fragmentPath)
        {
            std::string vCode, fCode;

            if (!readFile(vertexPath, vCode)) return nullptr;
            if (!readFile(fragmentPath, fCode)) return nullptr;

            GLuint vs = compile(vCode.c_str(), GL_VERTEX_SHADER);
            GLuint fs = compile(fCode.c_str(), GL_FRAGMENT_SHADER);

            return new glShader(vs, fs);
        }

    private:
        bool readFile(const char *path, std::string &out)
        {
            std::ifstream file(path);
            if (!file.is_open())
            {
                std::cerr << "Shader file not found: " << path << std::endl;
                return false;
            }
            std::stringstream ss;
            ss << file.rdbuf();
            out = ss.str();
            return true;
        }

        GLuint compile(const char *code, GLenum type)
        {
            GLuint shader = glCreateShader(type);
            glShaderSource(shader, 1, &code, NULL);
            glCompileShader(shader);

            GLint compiled;
            glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

            if (!compiled)
            {
                GLint logLength;
                glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
                char *log = new char[logLength];
                glGetShaderInfoLog(shader, logLength, NULL, log);
                std::cerr << (type == GL_VERTEX_SHADER ? "Vertex" : "Fragment")
                          << " shader compile error:\n"
                          << log << std::endl;
                delete[] log;
            }

            return shader;
        }
    };
}
