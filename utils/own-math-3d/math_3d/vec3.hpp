#pragma once
#include "vec2.hpp"

namespace math {
    union vec4;

    union vec3 {
        vec3() noexcept = default;
        explicit vec3(float value) noexcept;
        explicit vec3(const float* arr) noexcept;
        vec3(float x, float y, float z) noexcept;

        explicit vec3(const vec2& vec) noexcept;
        vec3(const vec2& xy, float z) noexcept;
        vec3(float x, const vec2& yz) noexcept;
        
        explicit vec3(const vec4& vec) noexcept;

        vec3& operator=(const vec2& vec) noexcept;
        vec3& operator=(const vec4& vec) noexcept;

        vec3 operator-() const noexcept;

        vec3 operator+(const vec3& vec) const noexcept;
        vec3 operator-(const vec3& vec) const noexcept;
        vec3& operator+=(const vec3& vec) noexcept;
        vec3& operator-=(const vec3& vec) noexcept;

        vec3 operator*(const vec3& vec) const noexcept;
        vec3& operator*=(const vec3& vec) noexcept;

        vec3 operator*(float value) const noexcept;
        vec3& operator*=(float value) noexcept;
        vec3 operator/(float value) const noexcept;
        vec3& operator/=(float value) noexcept;

        bool operator==(const vec3& vec) const noexcept;
        bool operator!=(const vec3& vec) const noexcept;

        float length() const noexcept;

        float& operator[](size_t i) noexcept;
        float operator[](size_t i) const noexcept;

        static vec3 get_random_in_range(float min, float max) noexcept;

        static const vec3 ZERO; 
        static const vec3 LEFT; 
        static const vec3 UP; 
        static const vec3 RIGHT; 
        static const vec3 DOWN; 
        static const vec3 FORWARD; 
        static const vec3 BACKWARD; 

        struct {
            float x, y, z;
        };

        struct {
            vec2 xy;
            float _ignore_0;
        };

        struct {
            float _ignore_1;
            vec2 yz;
        };

        float arr[3];
    };

    vec3 operator*(float value, const vec3& vec) noexcept;

    vec3 normalize(const vec3& vec) noexcept;
}