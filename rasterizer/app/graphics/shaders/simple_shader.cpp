#include "simple_shader.hpp"

namespace rasterization {
    struct VSInData {
        math::vec3f position;
        math::color color;
    };

    math::vec4f SimpleShader::vertex(const void *vertex, pd& _pd) const noexcept {
        using namespace math;
        
        const VSInData* v = (const VSInData*)vertex;

        out(v->color, "color", _pd);
        return vec4f(v->position, 1.0f) * get_uniform<mat4f>("model") * get_uniform<mat4f>("view") * get_uniform<mat4f>("projection");
    }
    
    math::color SimpleShader::pixel(const pd& _pd) const noexcept {
        return in<math::color>("color", _pd);
    }
}