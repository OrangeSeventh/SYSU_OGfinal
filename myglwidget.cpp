#include "myglwidget.h"
#include <iostream>
#include <QOpenGLTexture> // 用于加载纹理       


class MyGLWidget : public QOpenGLWidget, protected QOpenGLFunctions {
    Q_OBJECT
public:
    MyGLWidget(QWidget *parent = nullptr);
    ~MyGLWidget();

protected:
    void initializeGL() override;
    void paintGL() override;

private:
    void drawSkyBox(); // 绘制天空盒函数
    QOpenGLTexture *skyboxTextures[6]; // 存储6个面的纹理
};

// 构造函数
MyGLWidget::MyGLWidget(QWidget *parent)
    : QOpenGLWidget(parent),
      steve(0.3f), // 设置人物大小
      camera(15.0f, 15.0f, 10.0f) {} // 初始化摄像机

// 析构函数
MyGLWidget::~MyGLWidget() {
    for (int i = 0; i < 6; ++i) {
        delete skyboxTextures[i];
    }
}

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

    // 加载天空盒纹理
    const QString faces[6] = {
        "skybox_image/2.jpg",  // 右
        "skybox_image/1.jpg",   // 左
        "skybox_image/top.jpg",    // 上
        "skybox_image/bottom.jpg", // 下
        "skybox_image/3.jpg",  // 前
        "skybox_image/4.jpg"    // 后
    };

    for (int i = 0; i < 6; ++i) {
        skyboxTextures[i] = new QOpenGLTexture(QImage(faces[i]).mirrored());
        skyboxTextures[i]->setMinificationFilter(QOpenGLTexture::Linear);
        skyboxTextures[i]->setMagnificationFilter(QOpenGLTexture::Linear);
        skyboxTextures[i]->setWrapMode(QOpenGLTexture::ClampToEdge);
    }
}

// 绘制天空盒
void MyGLWidget::drawSkyBox() {
    static const GLfloat vertices[6][4][3] = {
        // 右
        {{ 1.0f,  1.0f, -1.0f}, { 1.0f, -1.0f, -1.0f}, { 1.0f, -1.0f,  1.0f}, { 1.0f,  1.0f,  1.0f}},
        // 左
        {{-1.0f,  1.0f,  1.0f}, {-1.0f, -1.0f,  1.0f}, {-1.0f, -1.0f, -1.0f}, {-1.0f,  1.0f, -1.0f}},
        // 上
        {{-1.0f,  1.0f, -1.0f}, {-1.0f,  1.0f,  1.0f}, { 1.0f,  1.0f,  1.0f}, { 1.0f,  1.0f, -1.0f}},
        // 下
        {{-1.0f, -1.0f,  1.0f}, {-1.0f, -1.0f, -1.0f}, { 1.0f, -1.0f, -1.0f}, { 1.0f, -1.0f,  1.0f}},
        // 前
        {{-1.0f,  1.0f,  1.0f}, {-1.0f, -1.0f,  1.0f}, { 1.0f, -1.0f,  1.0f}, { 1.0f,  1.0f,  1.0f}},
        // 后
        {{ 1.0f,  1.0f, -1.0f}, { 1.0f, -1.0f, -1.0f}, {-1.0f, -1.0f, -1.0f}, {-1.0f,  1.0f, -1.0f}}
    };

    glPushMatrix();
    glDisable(GL_LIGHTING); // 禁用光照
    glDisable(GL_DEPTH_TEST); // 禁用深度测试

    // 放大天空盒，防止摄像机外露
    glScalef(500.0f, 500.0f, 500.0f);

    // 绘制每个面
    for (int i = 0; i < 6; ++i) {
        skyboxTextures[i]->bind();
        glBegin(GL_QUADS);
        for (int j = 0; j < 4; ++j) {
            glTexCoord2f((j == 0 || j == 3) ? 1.0f : 0.0f, (j < 2) ? 1.0f : 0.0f);
            glVertex3fv(vertices[i][j]);
        }
        glEnd();
        skyboxTextures[i]->release();
    }

    glEnable(GL_DEPTH_TEST); // 恢复深度测试
    glEnable(GL_LIGHTING); // 恢复光照
    glPopMatrix();
}

// 渲染场景
void MyGLWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // 设置摄像机视角
    camera.setView();

    // 绘制天空盒
    drawSkyBox();

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
