#ifndef STEVE_H
#define STEVE_H

#include "vec3.h"
#include <GL/gl.h>
#include <cmath> // for sin() and cos()
class Steve {
public:
    vec3 position;
    float size;
    float bodyWidth, bodyHeight;
    float armSize, legSize;
    
    float leftLegAngle, rightLegAngle;  // 左右腿的旋转角度
    float leftArmAngle, rightArmAngle;  // 左右臂的旋转角度
    
    Steve(float size = 30.0f) 
        : position(0.0f, 0.0f, 0.0f), size(size),
          bodyWidth(size * 0.5f), bodyHeight(size * 1.2f), 
          armSize(size * 0.2f), legSize(size * 0.2f),
          leftLegAngle(0.0f), rightLegAngle(0.0f),
          leftArmAngle(0.0f), rightArmAngle(0.0f) {}

    void updateMovement(float deltaTime) {
        // 更新角色四肢的旋转角度，用于动画效果
        // 例如，根据时间逐渐改变四肢的角度，模拟走路
        leftLegAngle = sinf(deltaTime * 2.0f) * 45.0f;  // 左腿的角度
        rightLegAngle = sinf(deltaTime * 2.0f + 3.14f) * 45.0f;  // 右腿的角度
        leftArmAngle = sinf(deltaTime * 2.0f) * 45.0f;  // 左臂的角度
        rightArmAngle = sinf(deltaTime * 2.0f + 3.14f) * 45.0f;  // 右臂的角度
    }

    void draw() {
        // 绘制头部
        drawCube(position.x, position.y + 30.0f, position.z, 8.0f,8.0f,8.0f);

        // 绘制躯干
        drawCube(position.x, position.y +20.0f, position.z, 8.0f, 12.0f, 6.0f);

        // 绘制手臂（根据角度旋转）
        //drawArm(position.x - bodyWidth / 2 - armSize, position.y + bodyHeight / 2, position.z, leftArmAngle);  // 左臂
        //drawArm(position.x + bodyWidth / 2 + armSize, position.y + bodyHeight / 2, position.z, rightArmAngle);  // 右臂
        drawCube(-7.0f, 20.0f, 0.0f, 6.0f, 12.0f, 6.0f);
        drawCube(7.0f, 20.0f, 0.0f, 6.0f, 12.0f, 6.0f);


        // 绘制腿部（根据角度旋转）
        //drawLeg(position.x - bodyWidth / 4, position.y - bodyHeight / 2 - legSize, position.z, leftLegAngle);  // 左腿
        //drawLeg(position.x + bodyWidth / 4, position.y - bodyHeight / 2 - legSize, position.z, rightLegAngle);  // 右腿
        drawCube(-2.0f, 7.0f, 0.0f, 4.0f, 14.0f, 4.0f);
        drawCube(2.0f, 7.0f, 0.0f, 4.0f, 14.0f, 4.0f);
    
    }

    void drawArm(float x, float y, float z, float angle) {
        glPushMatrix();
        glTranslatef(x, y, z);
        glRotatef(angle, 1.0f, 0.0f, 0.0f);  // 绕X轴旋转，模拟手臂摆动
        drawCube(0.0f, 0.0f, 0.0f, armSize, armSize * 4, armSize);  // 绘制手臂
        glPopMatrix();
    }

    void drawLeg(float x, float y, float z, float angle) {
        glPushMatrix();
        glTranslatef(x, y, z);
        glRotatef(angle, 1.0f, 0.0f, 0.0f);  // 绕X轴旋转，模拟腿部摆动
        drawCube(0.0f, 0.0f, 0.0f, legSize, legSize * 4, legSize);  // 绘制腿部
        glPopMatrix();
    }

    void drawCube(float x, float y, float z, float w, float h, float d) {
        GLfloat vertices[] = {
            -w, -h, -d,  w, -h, -d,  w,  h, -d, -w,  h, -d,
            -w, -h,  d,  w, -h,  d,  w,  h,  d, -w,  h,  d,
            -w, -h, -d, -w,  h, -d, -w,  h,  d, -w, -h,  d,
             w, -h, -d,  w,  h, -d,  w,  h,  d,  w, -h,  d,
            -w, -h, -d, -w, -h,  d,  w, -h,  d,  w, -h, -d,
            -w,  h, -d, -w,  h,  d,  w,  h,  d,  w,  h, -d
        };

        glPushMatrix();
        glTranslatef(x, y, z);
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_FLOAT, 0, vertices);
        glDrawArrays(GL_QUADS, 0, 24);
        glDisableClientState(GL_VERTEX_ARRAY);
        glPopMatrix();
    }
};


#endif // STEVE_H
