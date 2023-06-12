#pragma once
#include <unordered_map>
#include <string>

#include "math_3d/math.hpp"


namespace rasterization::gfx {
    struct uniform_buffer final {
        std::unordered_map<std::string, math::mat4f> mat4f_uniforms;
        std::unordered_map<std::string, math::vec4f> vec4f_uniforms;
        std::unordered_map<std::string, math::vec3f> vec3f_uniforms;
        std::unordered_map<std::string, math::vec2f> vec2f_uniforms;

        std::unordered_map<std::string, float> float_uniforms;
    };

    struct _shader {
        friend struct _shader_engine;
        friend struct _render_engine;

        _shader() = default;
        virtual ~_shader() = default;

        virtual math::vec4f vertex(const void* vertex) const noexcept = 0;
        virtual math::color pixel(const void* vertex) const noexcept = 0;

        virtual void geometry() const noexcept;

    protected:
        const math::mat4f& get_mat4_uniform(const std::string& name) const noexcept;
        const math::vec4f& get_vec4_uniform(const std::string& name) const noexcept;
        const math::vec3f& get_vec3_uniform(const std::string& name) const noexcept;
        const math::vec2f& get_vec2_uniform(const std::string& name) const noexcept;
        float get_float_uniform(const std::string& name) const noexcept;

    private:
        uniform_buffer m_uniforms;
    };
}