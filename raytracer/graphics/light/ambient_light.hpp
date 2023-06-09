#pragma once
#include "light.hpp"

namespace raytracing::gfx {
    class AmbientLight : public ILight {
    public:
        AmbientLight() = default;
        AmbientLight(const math::color& color, float intensity);

        bool ComputeIllumination(const IntersectionData& int_data, const std::list<std::shared_ptr<IDrawable>>& drawables,
            float& out_intensity) const noexcept override;
    };
}