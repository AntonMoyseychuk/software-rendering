#include "../include/math_3d/vector_operations.hpp"

namespace math {
    float math::Dot(const vec3f& a, const vec3f& b) noexcept {
        return a.x * b.x + a.y * b.y + a.z * b.z;
    }
    
    vec3f Cross(const vec3f& a, const vec3f& b) noexcept {
        return vec3f(
            a.y * b.z - a.z * b.y,
            a.z * b.x - a.x * b.z,
            a.x * b.y - a.y * b.x
        );
    }

    vec3f math::Reflect(const vec3f &unit_vec, const vec3f &normal) noexcept {
        const auto L = -unit_vec;
        return 2.0f * normal * math::Dot(L, normal) - L;
    }
    
    vec3f Refract(const vec3f &unit_vec, const vec3f &normal, float n1_over_n2) noexcept {
        const auto cos_alpha = std::fmin(math::Dot(-unit_vec, normal), 1.0f);
        const vec3f ray_dir_out_perp =  n1_over_n2 * (unit_vec + cos_alpha * normal);
        const vec3f ray_dir_out_parallel = -std::sqrt(math::Abs(1.0f - ray_dir_out_perp.Length())) * normal;
        return ray_dir_out_perp + ray_dir_out_parallel;
    }

    float Angle(const vec3f &unit_vec1, const vec3f &unit_vec2) noexcept {
        return acosf(Dot(unit_vec1, unit_vec2));
    }
}