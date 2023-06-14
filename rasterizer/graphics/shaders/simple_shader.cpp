#include "simple_shader.hpp"

namespace rasterization::gfx {
    struct InputData {
        math::vec3f position;
    };

    void SimpleShader::vertex(const void *vertex) const noexcept {
        using namespace math;
        
        const InputData* v = (const InputData*)vertex;
        gl_Position = v->position * get_mat4_uniform("model") * get_mat4_uniform("view") * get_mat4_uniform("projection");
    }
    
    math::color SimpleShader::pixel(const void *vertex) const noexcept {
        return math::color();
    }
}