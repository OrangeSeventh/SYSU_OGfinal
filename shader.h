#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <QOpenGLFunctions_3_3_Core>
#include "include/glm/glm.hpp"
#include "include/glm/gtc/matrix_transform.hpp"
#include "include/glm/gtc/type_ptr.hpp"

class Shader {
public:
    unsigned int ID;
    QOpenGLFunctions_3_3_Core *gl; // 指向 OpenGL 函数集的指针

    Shader(QOpenGLFunctions_3_3_Core *glFunctions, const char* vertexPath, const char* fragmentPath)
        : gl(glFunctions) {
        // 1. 从文件读取顶点和片段着色器的源码
        std::string vertexCode;
        std::string fragmentCode;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;

        // 保证 ifstream 对象可以抛出异常：
        vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try {
            // 打开文件
            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);
            std::stringstream vShaderStream, fShaderStream;
            // 读取文件的缓冲内容到数据流中
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();
            // 关闭文件处理器
            vShaderFile.close();
            fShaderFile.close();
            // 转换数据流到 string
            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();
        } catch (std::ifstream::failure&) {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
        }
        const char* vShaderCode = vertexCode.c_str();
        const char* fShaderCode = fragmentCode.c_str();

        // 2. 编译着色器
        unsigned int vertex, fragment;
        int success;
        char infoLog[512];

        // 顶点着色器
        vertex = gl->glCreateShader(GL_VERTEX_SHADER);
        gl->glShaderSource(vertex, 1, &vShaderCode, nullptr);
        gl->glCompileShader(vertex);
        // 打印编译错误（如果有的话）
        gl->glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
        if (!success) {
            gl->glGetShaderInfoLog(vertex, 512, nullptr, infoLog);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        }

        // 片段着色器
        fragment = gl->glCreateShader(GL_FRAGMENT_SHADER);
        gl->glShaderSource(fragment, 1, &fShaderCode, nullptr);
        gl->glCompileShader(fragment);
        // 打印编译错误（如果有的话）
        gl->glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
        if (!success) {
            gl->glGetShaderInfoLog(fragment, 512, nullptr, infoLog);
            std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        }

        // 着色器程序
        ID = gl->glCreateProgram();
        gl->glAttachShader(ID, vertex);
        gl->glAttachShader(ID, fragment);
        gl->glLinkProgram(ID);
        // 打印链接错误（如果有的话）
        gl->glGetProgramiv(ID, GL_LINK_STATUS, &success);
        if (!success) {
            gl->glGetProgramInfoLog(ID, 512, nullptr, infoLog);
            std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        }

        // 删除着色器，它们已经链接到程序中了
        gl->glDeleteShader(vertex);
        gl->glDeleteShader(fragment);
    }

    // 激活着色器程序
    void use() {
        gl->glUseProgram(ID);
    }

    // 工具函数，用于设置 uniform 值
    void setBool(const std::string& name, bool value) const {
        gl->glUniform1i(gl->glGetUniformLocation(ID, name.c_str()), (int)value);
    }

    void setInt(const std::string& name, int value) const {
        gl->glUniform1i(gl->glGetUniformLocation(ID, name.c_str()), value);
    }

    void setFloat(const std::string& name, float value) const {
        gl->glUniform1f(gl->glGetUniformLocation(ID, name.c_str()), value);
    }

    void setMat4(const std::string& name, const glm::mat4& mat) const {
        gl->glUniformMatrix4fv(gl->glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
    }
};

#endif // SHADER_H
