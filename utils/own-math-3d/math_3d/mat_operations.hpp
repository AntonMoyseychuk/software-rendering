#pragma once
#include "mat4.hpp"
#include "vec_operations.hpp"

#include <cstdint>

namespace math {
    mat4f scale(const mat4f& mat, const vec3f& scale) noexcept;
    mat4f translate(const mat4f& mat, const vec3f& translation) noexcept;
    
    mat4f rotate_x(const mat4f& mat, float angle_radians) noexcept;
    mat4f rotate_y(const mat4f& mat, float angle_radians) noexcept;
    mat4f rotate_z(const mat4f& mat, float angle_radians) noexcept;

    mat4f look_at_rh(const vec3f& eye, const vec3f& look_at, const vec3f& up) noexcept;
    mat4f perspective(float fovy_radians, float aspect, float near, float far) noexcept;
    mat4f viewport(uint32_t width, uint32_t height) noexcept;
}