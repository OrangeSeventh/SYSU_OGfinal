#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H

#ifdef MAC_OS
#include <QtOpenGL/QtOpenGL>
#else
#include <GL/glew.h>
#endif
#include <QtGui>
#include <QOpenGLWidget>
// #include <QOpenGLFunctions>
#include <QOpenGLFunctions_3_3_Core>
#include "camera.h"
#include "shader.h" // 添加着色器类
#include "steve.h" // 引入 Steve 类

class MyGLWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core {
    Q_OBJECT

public:
    MyGLWidget(QWidget *parent = nullptr);
    ~MyGLWidget();
    void drawAxis(float length = 50.0f); // 绘制坐标轴
    
protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int width, int height) override;
    void keyPressEvent(QKeyEvent *e) override;

private:
    Camera camera;  // 摄像机
    unsigned int skyboxVAO, skyboxVBO;
    unsigned int cubemapTexture;
    Shader *skyboxShader;
    std::vector<std::string> faces;

    Steve *steve;    // 添加 Steve 对象
    unsigned int loadCubemap(std::vector<std::string> faces);
};

#endif // MYGLWIDGET_H
