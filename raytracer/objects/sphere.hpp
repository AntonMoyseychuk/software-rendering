#pragma once
#include "drawable.hpp"

namespace gfx {
    class Sphere : public IDrawable {
    public:
        Sphere() = default;
        Sphere(const math::vec3f& position, float radius, const Material& material);
        ~Sphere() override;

        bool IsIntersect(const Ray& ray, math::vec3f& intersect_point, math::vec3f& local_normal, Color& local_color) const noexcept override;

        void SetRadius(float radius) noexcept;
        float GetRadius() const noexcept;
        float& GetRadius() noexcept;

    private:
        float m_radius = 0.0f;
    };
    
}