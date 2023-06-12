#include "shader_engine.hpp"
#include "shader_engine_macros.hpp"

#include <cassert>

namespace rasterization::gfx {
    _shader_engine &_shader_engine::get() noexcept {
        static _shader_engine engine;
        return engine;
    }

    size_t _shader_engine::create_shader(
        const std::function<math::vec4f(const uniform_buffer& uniform_buffer, const void* vertex)>& vertex,
        const std::function<math::vec4f(const uniform_buffer& uniform_buffer, const void* vertex)>& pixel
    ) noexcept {
        size_t id;
        do {
            id = math::random((size_t)0, SIZE_MAX - 1) + 1;
        } while (shader_programs.count(id) != 0);

        shader_programs[id] = shader_program { _shader(vertex, pixel), uniform_buffer() };

        return id;
    }

    void _shader_engine::bind_shader(size_t shader_id) noexcept
    {
        ASSERT_SHADER_PROGRAM_ID_VALIDITY(shader_programs, shader_id);
        curr_shader = shader_id;
    }

    void _shader_engine::uniform(const std::string &uniform_name, const math::mat4f &mat) noexcept {
        ASSERT_SHADER_PROGRAM_ID_VALIDITY(shader_programs, curr_shader);
        shader_programs[curr_shader].uniform_buffer.mat4f_uniforms[uniform_name] = mat;
    }
    
    void _shader_engine::uniform(const std::string &uniform_name, const math::vec4f &vec) noexcept {
        ASSERT_SHADER_PROGRAM_ID_VALIDITY(shader_programs, curr_shader);
        shader_programs[curr_shader].uniform_buffer.vec4f_uniforms[uniform_name] = vec;
    }
    
    void _shader_engine::uniform(const std::string &uniform_name, const math::vec3f &vec) noexcept {
        ASSERT_SHADER_PROGRAM_ID_VALIDITY(shader_programs, curr_shader);
        shader_programs[curr_shader].uniform_buffer.vec3f_uniforms[uniform_name] = vec;
    }
    
    void _shader_engine::uniform(const std::string &uniform_name, const math::vec2f &vec) noexcept {
        ASSERT_SHADER_PROGRAM_ID_VALIDITY(shader_programs, curr_shader);
        shader_programs[curr_shader].uniform_buffer.vec2f_uniforms[uniform_name] = vec;
    }
    
    void _shader_engine::uniform(const std::string &uniform_name, float value) noexcept {
        ASSERT_SHADER_PROGRAM_ID_VALIDITY(shader_programs, curr_shader);
        shader_programs[curr_shader].uniform_buffer.float_uniforms[uniform_name] = value;
    }
}