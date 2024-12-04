#ifndef STEVE_H
#define STEVE_H

#include <QOpenGLFunctions_3_3_Core>
#include <vector>
#include <string>
#include <iostream>
#include "include/stb_image.h"
#include <QOpenGLContext>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Steve : protected QOpenGLFunctions_3_3_Core {
public:
    float size;
    float x, y, z; // Steve 的中心位置
    GLuint textures[6 * 6]; // 每个身体部位6个面，总共6个部位
    GLuint VAO, VBO; // 顶点数组对象和顶点缓冲对象
    GLuint shaderProgram; // 着色器程序ID

    // 构造函数需要传递GL上下文和着色器程序ID
    Steve(QOpenGLFunctions_3_3_Core* glContext, GLuint shaderProg, float sz = 30.0f)
        : size(sz), x(0.0f), y(0.0f), z(0.0f), shaderProgram(shaderProg) {
        glContext->initializeOpenGLFunctions(); // 初始化OpenGL函数
        setupMesh(glContext); // 设置网格
    }

    // 初始化立方体的顶点缓冲和数组缓冲
    void setupMesh(QOpenGLFunctions_3_3_Core* glContext) {
        GLfloat vertices[] = {
            // 顶点坐标和纹理坐标
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
            0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
            0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
            0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
            -0.5f,  0.5f, -0.5f, 0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

            -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
            0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
            0.5f,  0.5f,  0.5f, 1.0f, 1.0f,
            0.5f,  0.5f,  0.5f, 1.0f, 1.0f,
            -0.5f,  0.5f,  0.5f, 0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
        };

        glContext->glGenVertexArrays(1, &VAO);
        glContext->glGenBuffers(1, &VBO);
        glContext->glBindVertexArray(VAO);

        glContext->glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glContext->glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        // 顶点位置属性
        glContext->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glContext->glEnableVertexAttribArray(0);
        // 纹理坐标属性
        glContext->glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glContext->glEnableVertexAttribArray(1);

        glContext->glBindBuffer(GL_ARRAY_BUFFER, 0);
        glContext->glBindVertexArray(0);
    }

    // 在给定位置、尺寸和部件索引的情况下绘制一个立方体
    void drawTexturedCube(QOpenGLFunctions_3_3_Core* glContext, float x, float y, float z, float width, float height, float depth, int partIndex) {
        glContext->glUseProgram(shaderProgram);

        glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z));
        model = glm::scale(model, glm::vec3(width, height, depth));
        glContext->glUniformMatrix4fv(glContext->glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, &model[0][0]);

        glContext->glActiveTexture(GL_TEXTURE0);
        glContext->glBindTexture(GL_TEXTURE_2D, textures[partIndex]);
        glContext->glBindVertexArray(VAO);
        glContext->glDrawArrays(GL_TRIANGLES, 0, 36); // 36 vertices make up a cube
        glContext->glBindVertexArray(0);
        glContext->glUseProgram(0);
    }
};

#endif // STEVE_H
