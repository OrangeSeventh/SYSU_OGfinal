#include "myglwidget.h"
#include <iostream>

// 构造函数，初始化 Steve 和 Camera
MyGLWidget::MyGLWidget(QWidget *parent)
    : QOpenGLWidget(parent),
      steve(0.3f),  // 设置人物大小为边长2.0
      camera(15.0f, 15.0f, 10.0f) { // 初始化摄像机，半径15.0，高度10.0，旋转速度10.0
}

MyGLWidget::~MyGLWidget() {}

// 初始化 OpenGL 环境
void MyGLWidget::initializeGL() {
    initializeOpenGLFunctions();
    glEnable(GL_DEPTH_TEST); // 开启深度测试
    glEnable(GL_LIGHTING);   // 开启光照
    glEnable(GL_LIGHT0);     // 开启默认光源

    // 配置光源
    GLfloat lightPosition[] = { 5.0f, 10.0f, 5.0f, 1.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

    GLfloat lightAmbient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat lightDiffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    GLfloat lightSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };

    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f); // 背景颜色
}

// 渲染场景
void MyGLWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // 设置摄像机视角
    camera.setView();

    // 绘制人物
    steve.draw();

    glFlush();
}

// 调整窗口大小
void MyGLWidget::resizeGL(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, GLfloat(width) / height, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

// 处理键盘输入
void MyGLWidget::keyPressEvent(QKeyEvent *e) {
    switch (e->key()) {
    case Qt::Key_Z:
        camera.rotateClockwise();
        update(); // 触发重新绘制
        break;
    case Qt::Key_C:
        camera.rotateCounterClockwise();
        update(); // 触发重新绘制
        break;
    default:
        QOpenGLWidget::keyPressEvent(e);
    }
}
