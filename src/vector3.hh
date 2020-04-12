#ifndef VECTOR3_HH
#define VECTOR3_HH

#include <iostream>
#include <cmath>
#include <optional>
#include <algorithm>
#include <iomanip>


class Vector3 {

public:
    constexpr float x() const noexcept {
        return val[0];
    }

    constexpr float y() const noexcept {
        return val[1];
    }

    constexpr float z() const noexcept {
        return val[2];
    }

    constexpr Vector3() noexcept = default;

    constexpr Vector3(float x, float y, float z) noexcept : val{x, y, z} {}

    constexpr Vector3(float c) noexcept : val{c, c, c} {}

    constexpr const float &operator()(unsigned int col) const {
        return val[col];
    }

    constexpr float &operator[](unsigned int col) {
        return val[col];
    }

    friend std::ostream &operator<<(std::ostream &out, Vector3 const &v) {
        return out << std::setprecision(2) << "(" << v.val[0] << ", " << v.val[1]
                   << ", " << v.val[2] << ")";
    }

    constexpr Vector3 operator+(const Vector3 &v) const noexcept {
        return Vector3(val[0] + v.val[0], val[1] + v.val[1], val[2] + v.val[2]);
    }

    constexpr Vector3 operator-(const Vector3 &v) const noexcept {
        return Vector3(val[0] - v.val[0], val[1] - v.val[1], val[2] - v.val[2]);
    }

    constexpr Vector3 operator*(const Vector3 &v) const noexcept {
        return Vector3(val[0] * v.val[0], val[1] * v.val[1], val[2] * v.val[2]);
    }

    constexpr Vector3 operator*(const float &c) const noexcept {
        return Vector3(val[0] * c, val[1] * c, val[2] * c);
    }

    constexpr Vector3 operator/(const float &c) const noexcept {
        return Vector3(val[0] / c, val[1] / c, val[2] / c);
    }

    constexpr Vector3 &operator+=(const Vector3 &v) noexcept {
        this->val[0] += v.val[0];
        this->val[1] += v.val[1];
        this->val[2] += v.val[2];
        return *this;
    }

    constexpr Vector3 &operator-=(const Vector3 &v) noexcept {
        this->val[0] -= v.val[0];
        this->val[1] -= v.val[1];
        this->val[2] -= v.val[2];
        return *this;
    }

    constexpr Vector3 &operator*=(float c) noexcept {
        this->val[0] *= c;
        this->val[1] *= c;
        this->val[2] *= c;
        return *this;
    }

    constexpr Vector3 &operator*=(const Vector3 &v) noexcept {
        this->val[0] *= v.val[0];
        this->val[1] *= v.val[1];
        this->val[2] *= v.val[2];
        return *this;
    }

    constexpr Vector3 &operator/=(float c) noexcept {
        this->val[0] /= c;
        this->val[1] /= c;
        this->val[2] /= c;
        return *this;
    }

    Vector3 &normalize() noexcept {
        *this /= this->norm();
        return *this;
    }

    constexpr float dot(const Vector3 &v) const noexcept {
        return val[0] * v.val[0] + val[1] * v.val[1] + val[2] * v.val[2];
    }

    constexpr Vector3 cross(const Vector3 &v) const noexcept {
        return Vector3(val[1] * v.val[2] - val[2] * v.val[1], val[2] * v.val[0] - val[0] * v.val[2],
                       val[0] * v.val[1] - val[1] * v.val[0]);
    }


    float norm() const noexcept {
        return sqrt(val[0] * val[0] + val[1] * val[1] + val[2] * val[2]);
    }


    float val[3]{0, 0, 0};
};


#endif /* VECTOR3_HH */
