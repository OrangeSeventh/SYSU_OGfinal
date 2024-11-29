#ifndef CAMERA_H
#define CAMERA_H

#include "vec3.h"
#include <cmath>

/*###################################################
##  类: Camera
##  类描述： 摄像机类，用于围绕原点旋转观察人物
#####################################################*/
class Camera {
public:
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
    void setView() {
        vec3 position = vec3(
            radius * std::cos(radian(angle)), // X 坐标
            height,                          // Y 坐标
            radius * std::sin(radian(angle)) // Z 坐标
        );
        vec3 target = vec3(0.0f, 0.0f, 0.0f);  // 观察的目标点（原点）
        vec3 up = vec3(0.0f, 1.0f, 0.0f);      // 上方向

        gluLookAt(position.x, position.y, position.z, target.x, target.y, target.z, up.x, up.y, up.z);
    }

private:
    float radius;       // 摄像机距离原点的半径
    float height;       // 摄像机的高度
    float angle;        // 摄像机当前的旋转角度（围绕Y轴）
    float rotationSpeed;// 摄像机旋转速度

    float radian(float degree) {
        return degree * 3.141592653589793f / 180.0f;
    }
};

#endif // CAMERA_H
