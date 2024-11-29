#ifndef VEC3_H
#define VEC3_H

#include <cmath>
#include <iostream>

/*###################################################
##  类: vec3
##  类描述： 三维向量
#####################################################*/
class vec3 {
public:
    float x, y, z;  // 向量分量

    // 默认构造函数
    vec3() : x(0), y(0), z(0) {}

    // 构造函数
    vec3(float xx, float yy, float zz) : x(xx), y(yy), z(zz) {}

    // 重载负号操作符
    inline vec3 operator-() const { return vec3(-x, -y, -z); }

    // 基本运算符重载
    inline vec3& operator+=(const vec3& v);
    inline vec3& operator-=(const vec3& v);
    inline vec3& operator*=(const float t);
    inline vec3& operator/=(const float t);

    // 向量长度
    inline float length() const { return std::sqrt(x * x + y * y + z * z); }

    // 向量长度平方
    inline float squared_length() const { return x * x + y * y + z * z; }

    // 归一化
    inline vec3 normalize() const;

    // 流输出操作符
    friend inline std::ostream& operator<<(std::ostream& os, const vec3& v);

    // 友元运算符
    friend inline vec3 operator+(const vec3& v1, const vec3& v2);
    friend inline vec3 operator-(const vec3& v1, const vec3& v2);
    friend inline vec3 operator*(const vec3& v, float t);
    friend inline vec3 operator*(float t, const vec3& v);
    friend inline vec3 operator/(const vec3& v, float t);
    friend inline float dot(const vec3& v1, const vec3& v2);
    friend inline vec3 cross(const vec3& v1, const vec3& v2);
};

// 向量加法
inline vec3 operator+(const vec3& v1, const vec3& v2) {
    return vec3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}

// 向量减法
inline vec3 operator-(const vec3& v1, const vec3& v2) {
    return vec3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}

// 向量乘以标量
inline vec3 operator*(const vec3& v, float t) {
    return vec3(v.x * t, v.y * t, v.z * t);
}

// 标量乘以向量
inline vec3 operator*(float t, const vec3& v) {
    return vec3(v.x * t, v.y * t, v.z * t);
}

// 向量除以标量
inline vec3 operator/(const vec3& v, float t) {
    return vec3(v.x / t, v.y / t, v.z / t);
}

// 点积
inline float dot(const vec3& v1, const vec3& v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

// 叉积
inline vec3 cross(const vec3& v1, const vec3& v2) {
    return vec3(
        v1.y * v2.z - v1.z * v2.y,
        v1.z * v2.x - v1.x * v2.z,
        v1.x * v2.y - v1.y * v2.x
    );
}

// 向量归一化
inline vec3 vec3::normalize() const {
    float len = length();
    if (len == 0) return vec3(0, 0, 0);
    return *this / len;
}

// += 操作符
inline vec3& vec3::operator+=(const vec3& v) {
    x += v.x;
    y += v.y;
    z += v.z;
    return *this;
}

// -= 操作符
inline vec3& vec3::operator-=(const vec3& v) {
    x -= v.x;
    y -= v.y;
    z -= v.z;
    return *this;
}

// *= 操作符（标量）
inline vec3& vec3::operator*=(const float t) {
    x *= t;
    y *= t;
    z *= t;
    return *this;
}

// /= 操作符（标量）
inline vec3& vec3::operator/=(const float t) {
    x /= t;
    y /= t;
    z /= t;
    return *this;
}

// 输出向量内容
inline std::ostream& operator<<(std::ostream& os, const vec3& v) {
    os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
    return os;
}

#endif // VEC3_H
