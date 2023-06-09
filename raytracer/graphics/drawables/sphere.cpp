#include "sphere.hpp"
#include "math_3d/math.hpp"

#include <cassert>

namespace raytracing::gfx {
    Sphere::Sphere(const math::vec3f& position, float r, std::shared_ptr<IMaterial> material)
        : IDrawable(position, material), m_radius(r)
    {
    }

    Sphere::~Sphere()
    {
    }

    std::optional<IntersectionData> Sphere::IsIntersect(const Ray& ray) const noexcept {
        using namespace math;

        vec3f k = ray.origin - m_position;
        float b = 2.0f * dot(k, ray.direction);
        float c = dot(k,k) - m_radius * m_radius;
        float d = b * b - 4.0f * c;

        if (d >= 0.0f) {
            float half_sqrtd = std::sqrtf(d) / 2.0f;
            float half_b = b / 2.0f;

            // float t1 = (-b + sqrtd) / 2.0f;
            // float t2 = (-b - sqrtd) / 2.0f;

            float t = -half_b - half_sqrtd;

            auto intersect_point = ray.origin + ray.direction * t;
            auto local_normal = math::normalize(intersect_point - m_position);
            
            return t > 0 ? IntersectionData(intersect_point, local_normal, t, ray, m_material) : std::optional<IntersectionData>{};
        }

        return {};
    }

    void Sphere::SetRadius(float radius) noexcept {
        assert(radius >= 0.0f);
        m_radius = radius;
    }

    float Sphere::GetRadius() const noexcept {
        return m_radius;
    }
}