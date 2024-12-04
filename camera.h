#ifndef CAMERA_H
#define CAMERA_H

#include "vec3.h"
#include <cmath>

#include "include/glm/glm.hpp"
#include "include/glm/gtc/matrix_transform.hpp"

/*###################################################
##  类: Camera
##  类描述： 摄像机类，用于围绕原点旋转观察人物
#####################################################*/
class Camera {
public:

    float radius;       // 摄像机距离原点的半径
    float height;       // 摄像机的高度
    float angle;        // 摄像机当前的旋转角度（围绕Y轴）
    float rotationSpeed;// 摄像机旋转速度


    // 默认构造函数
    Camera() : radius(10.0f), height(5.0f), angle(5.0f), rotationSpeed(1.0f) {}

    // 参数化构造函数
    Camera(float radius, float height, float speed)
        : radius(radius), height(height), angle(0.0f), rotationSpeed(speed) {}


    // 更新摄像机的角度
    void rotateClockwise() {
        angle -= rotationSpeed;
        if (angle < 0) angle += 360.0f;
    }

    void rotateCounterClockwise() {
        angle += rotationSpeed;
        if (angle >= 360.0f) angle -= 360.0f;
    }

    // 设置摄像机视角
    void setView(const vec3& target) {
    vec3 position = vec3(
        target.x + radius * std::cos(radian(angle)), // X 坐标
        target.y + height,                           // Y 坐标（增加高度）
        target.z + radius * std::sin(radian(angle))  // Z 坐标
    );
    vec3 up = vec3(0.0f, 1.0f, 0.0f); // Y 轴为上方向

    gluLookAt(position.x, position.y, position.z, target.x, target.y, target.z, up.x, up.y, up.z);
}


    // 返回视图矩阵
    glm::mat4 GetViewMatrix(const glm::vec3& target) {
    glm::vec3 position(
        target.x + radius * std::cos(glm::radians(angle)), // X 坐标
        target.y + height,                                // Y 坐标（增加高度）
        target.z + radius * std::sin(glm::radians(angle)) // Z 坐标
    );
    glm::vec3 up(0.0f, 1.0f, 0.0f); // Y 轴为上方向

    return glm::lookAt(position, target, up);
}


    vec3 getPosition() const {
    return vec3(
        radius * cos(glm::radians(angle)),
        height,
        radius * sin(glm::radians(angle))
    );
    }

    vec3 getTarget() const {
        return vec3(0.0f, 1.0f, 0.0f); // 默认观察目标
    }


    void printPosition() const {
    vec3 position = getPosition();
    std::cout << "Camera Position: (" << position.x << ", " << position.y << ", " << position.z << ")" << std::endl;
}




    
private:
//     float radius;       // 摄像机距离原点的半径
//     float height;       // 摄像机的高度
//     float angle;        // 摄像机当前的旋转角度（围绕Y轴）
//     float rotationSpeed;// 摄像机旋转速度

    float radian(float degree) {
        return degree * 3.141592653589793f / 180.0f;
    }
};

#endif // CAMERA_H
