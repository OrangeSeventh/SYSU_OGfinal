#include "myglwidget.h"
#include <iostream>
#include "include/glm/glm.hpp"
#include "include/glm/gtc/matrix_transform.hpp"
#include "include/glm/gtc/type_ptr.hpp"
#define STB_IMAGE_IMPLEMENTATION // 定义 STB_IMAGE_IMPLEMENTATION
#include "include/stb_image.h"


// 天空盒顶点数据
float skyboxVertices[] = {
    -100.0f,  100.0f, -100.0f,  -100.0f, -100.0f, -100.0f,   100.0f, -100.0f, -100.0f,
     100.0f, -100.0f, -100.0f,   100.0f,  100.0f, -100.0f,  -100.0f,  100.0f, -100.0f,
    -100.0f, -100.0f,  100.0f,  -100.0f, -100.0f, -100.0f,  -100.0f,  100.0f, -100.0f,
    -100.0f,  100.0f, -100.0f,  -100.0f,  100.0f,  100.0f,  -100.0f, -100.0f,  100.0f,
     100.0f, -100.0f, -100.0f,   100.0f, -100.0f,  100.0f,   100.0f,  100.0f,  100.0f,
     100.0f,  100.0f,  100.0f,   100.0f,  100.0f, -100.0f,   100.0f, -100.0f, -100.0f,
    -100.0f, -100.0f,  100.0f,  -100.0f,  100.0f,  100.0f,   100.0f,  100.0f,  100.0f,
     100.0f,  100.0f,  100.0f,   100.0f, -100.0f,  100.0f,  -100.0f, -100.0f,  100.0f,
    -100.0f,  100.0f, -100.0f,   100.0f,  100.0f, -100.0f,   100.0f,  100.0f,  100.0f,
     100.0f,  100.0f,  100.0f,  -100.0f,  100.0f,  100.0f,  -100.0f,  100.0f, -100.0f,
    -100.0f, -100.0f, -100.0f,  -100.0f, -100.0f,  100.0f,   100.0f, -100.0f, -100.0f,
     100.0f, -100.0f, -100.0f,  -100.0f, -100.0f,  100.0f,   100.0f, -100.0f,  100.0f
};


std::vector<std::string> texturePaths = {
    "textures/face_f.jpg", "textures/face_l.jpg", "textures/face_r.jpg", "textures/face_b.jpg", "textures/face_u.jpg", "textures/face_d.jpg",
    "textures/body_f.jpg", "textures/body_l.jpg", "textures/body_r.jpg", "textures/body_b.jpg", "textures/body_u.jpg", "textures/body_d.jpg",
    "textures/hand.jpg", "textures/hand.jpg", "textures/hand.jpg", "textures/hand.jpg", "textures/body_l.jpg", "textures/hand_d.jpg",
    "textures/hand.jpg", "textures/hand.jpg", "textures/hand.jpg", "textures/hand.jpg", "textures/body_l.jpg", "textures/hand_d.jpg",
    "textures/leg.jpg", "textures/leg.jpg", "textures/leg.jpg", "textures/leg.jpg", "textures/black.jpg", "textures/black.jpg",
    "textures/leg.jpg", "textures/leg.jpg", "textures/leg.jpg", "textures/leg.jpg", "textures/black.jpg", "textures/black.jpg"
};


// 构造函数，初始化 Camera
MyGLWidget::MyGLWidget(QWidget *parent)
    : QOpenGLWidget(parent),
      camera(40.0f, 6.0f, 5.0f),
      steve(nullptr) {

    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    format.setMajorVersion(3);
    format.setMinorVersion(3);
    format.setProfile(QSurfaceFormat::CompatibilityProfile);
    setFormat(format);
}
      // 析构函数
MyGLWidget::~MyGLWidget() {
    delete skyboxShader;
}






// 初始化 OpenGL 环境
void MyGLWidget::initializeGL() {
    // 初始化 OpenGL 函数
    initializeOpenGLFunctions();
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D); // 确保启用纹理功能
    // glClearDepth(1.0f);      
    // glEnable(GL_CULL_FACE);  
    // glCullFace(GL_BACK);    
    
    glClearDepth(1.0f);      // 设置深度缓冲区清除值
    // glEnable(GL_CULL_FACE);  // 启用背面剔除，避免显示隐藏面
    // glCullFace(GL_BACK);     // 剔除背面




    // 加载天空盒纹理
    faces = {
        "skybox/right.jpg",
        "skybox/left.jpg",
        "skybox/up.jpg",
        "skybox/down.jpg",
        "skybox/front.jpg",
        "skybox/back.jpg"
    };
    cubemapTexture = loadCubemap(faces);
    
    // std::cout << "Cubemap Texture ID: " << cubemapTexture << std::endl;
    
    // 创建天空盒 VAO 和 VBO
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glBindVertexArray(0);

    // 初始化天空盒着色器，传入 QOpenGLFunctions_3_3_Core 的指针
    skyboxShader = new Shader(this, "shaders/skybox.vs", "shaders/skybox.fs");
    skyboxShader->use();
    skyboxShader->setInt("skybox", 0);


    steve = new Steve(this, 3.0f);
    
    
    // 测试是否存在纹理路径
    // std::cout << "Texture paths size: " << texturePaths.size() << std::endl;
    // for (const auto& path : texturePaths) {
    // std::cout << "Path: " << path << std::endl;
    // }
    
    
    steve->loadTextures(texturePaths);
}

// 渲染场景
void MyGLWidget::paintGL() {
    // 清除颜色和深度缓冲区
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    vec3 stevePosition(0.0f, 10.0f, 0.0f);

    // // 打印摄像机位置
    // camera.printPosition();

    // // 打印 Steve 位置
    // steve->printPosition();


    // 渲染其他物体
    camera.setView(stevePosition); // 设置摄像机视角

    drawAxis(1000.0f); // 绘制坐标轴

    

    steve->draw(); // 绘制 Steve



    // 渲染天空盒
    glDepthMask(GL_FALSE); // 禁止写入深度缓冲区
    glDepthFunc(GL_LEQUAL); // 设置深度测试模式为小于等于

    
    skyboxShader->use();
    glm::mat4 view = glm::mat4(glm::mat3(camera.GetViewMatrix(glm::vec3(stevePosition.x, stevePosition.y, stevePosition.z)))); // 移除位移部分
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)width() / height(), 0.1f, 500.0f);
    skyboxShader->setMat4("view", view);
    skyboxShader->setMat4("projection", projection);

    glBindVertexArray(skyboxVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);


    glDepthMask(GL_TRUE); // 恢复深度缓冲区写入
    glDepthFunc(GL_LESS); // 恢复深度测试到默认


    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (float)width() / height(), 0.1, 1000.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();




    




    



}

// 调整窗口大小
void MyGLWidget::resizeGL(int width, int height) {
    glViewport(0, 0, width, height);
}

// 加载天空盒纹理
unsigned int MyGLWidget::loadCubemap(std::vector<std::string> faces) {
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++) {
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 
                         0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        } else {
            std::cout << "Failed to load cubemap texture at " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}


// 处理键盘输入
void MyGLWidget::keyPressEvent(QKeyEvent *e) {
    switch (e->key()) {
    case Qt::Key_Z: // 顺时针旋转
        camera.rotateClockwise();
        update(); // 触发重新绘制
        break;
    case Qt::Key_C: // 逆时针旋转
        camera.rotateCounterClockwise();
        update(); // 触发重新绘制
        break;
    default:
        QOpenGLWidget::keyPressEvent(e);
    }
}


// 用于绘制坐标轴
void MyGLWidget::drawAxis(float length) {
    glLineWidth(2.0f); // 设置线条宽度

    glBegin(GL_LINES);

    // X 轴：红色
    glColor3f(1.0f, 0.0f, 0.0f); // 红色
    glVertex3f(0.0f, 0.0f, 0.0f); // 起点
    glVertex3f(length, 0.0f, 0.0f); // 终点

    // Y 轴：绿色
    glColor3f(0.0f, 1.0f, 0.0f); // 绿色
    glVertex3f(0.0f, 0.0f, 0.0f); // 起点
    glVertex3f(0.0f, length, 0.0f); // 终点

    // Z 轴：蓝色
    glColor3f(0.0f, 0.0f, 1.0f); // 蓝色
    glVertex3f(0.0f, 0.0f, 0.0f); // 起点
    glVertex3f(0.0f, 0.0f, length); // 终点

    glEnd();
}