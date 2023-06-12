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

        shader_programs[id] = shader_program { uniform_buffer(), _shader(vertex, pixel) };

        return id;
    }

    void _shader_engine::bind_shader(size_t shader_id) noexcept {
        ASSERT_SHADER_PROGRAM_ID_VALIDITY(shader_programs, shader_id);
        binded_shader = shader_id;
    }

    size_t _shader_engine::create_abstract_shader(const std::shared_ptr<_abstract_shader> &shader) noexcept {
        size_t id;
        do {
            id = math::random((size_t)0, SIZE_MAX - 1) + 1;
        } while (shader_programs.count(id) != 0);

        abstract_shader_programs[id] = abstract_shader_program { shader };

        return id;
    }

    void _shader_engine::bind_abstract_shader(size_t shader_id) noexcept {
        ASSERT_SHADER_PROGRAM_ID_VALIDITY(abstract_shader_programs, shader_id);
        binded_shader = shader_id;
    }

    void _shader_engine::uniform(const std::string &uniform_name, const math::mat4f &mat) noexcept {
        ASSERT_SHADER_PROGRAM_ID_VALIDITY(shader_programs, binded_shader);
        shader_programs[binded_shader].uniform_buffer.mat4f_uniforms[uniform_name] = mat;
    }
    
    void _shader_engine::uniform(const std::string &uniform_name, const math::vec4f &vec) noexcept {
        ASSERT_SHADER_PROGRAM_ID_VALIDITY(shader_programs, binded_shader);
        shader_programs[binded_shader].uniform_buffer.vec4f_uniforms[uniform_name] = vec;
    }
    
    void _shader_engine::uniform(const std::string &uniform_name, const math::vec3f &vec) noexcept {
        ASSERT_SHADER_PROGRAM_ID_VALIDITY(shader_programs, binded_shader);
        shader_programs[binded_shader].uniform_buffer.vec3f_uniforms[uniform_name] = vec;
    }
    
    void _shader_engine::uniform(const std::string &uniform_name, const math::vec2f &vec) noexcept {
        ASSERT_SHADER_PROGRAM_ID_VALIDITY(shader_programs, binded_shader);
        shader_programs[binded_shader].uniform_buffer.vec2f_uniforms[uniform_name] = vec;
    }
    
    void _shader_engine::uniform(const std::string &uniform_name, float value) noexcept {
        ASSERT_SHADER_PROGRAM_ID_VALIDITY(shader_programs, binded_shader);
        shader_programs[binded_shader].uniform_buffer.float_uniforms[uniform_name] = value;
    }

    void _shader_engine::abstract_uniform(const std::string &uniform_name, const math::mat4f &mat) noexcept {
        ASSERT_SHADER_PROGRAM_ID_VALIDITY(abstract_shader_programs, binded_shader);
        abstract_shader_programs[binded_shader].shader->m_uniforms.mat4f_uniforms[uniform_name] = mat;
    }

    void _shader_engine::abstract_uniform(const std::string &uniform_name, const math::vec4f &vec) noexcept {
        ASSERT_SHADER_PROGRAM_ID_VALIDITY(abstract_shader_programs, binded_shader);
        abstract_shader_programs[binded_shader].shader->m_uniforms.vec4f_uniforms[uniform_name] = vec;
    }

    void _shader_engine::abstract_uniform(const std::string &uniform_name, const math::vec3f &vec) noexcept {
        ASSERT_SHADER_PROGRAM_ID_VALIDITY(abstract_shader_programs, binded_shader);
        abstract_shader_programs[binded_shader].shader->m_uniforms.vec3f_uniforms[uniform_name] = vec;
    }

    void _shader_engine::abstract_uniform(const std::string &uniform_name, const math::vec2f &vec) noexcept {
        ASSERT_SHADER_PROGRAM_ID_VALIDITY(abstract_shader_programs, binded_shader);
        abstract_shader_programs[binded_shader].shader->m_uniforms.vec2f_uniforms[uniform_name] = vec;
    }

    void _shader_engine::abstract_uniform(const std::string &uniform_name, float value) noexcept {
        ASSERT_SHADER_PROGRAM_ID_VALIDITY(abstract_shader_programs, binded_shader);
        abstract_shader_programs[binded_shader].shader->m_uniforms.float_uniforms[uniform_name] = value;
    }

    const _shader_engine::shader_program &_shader_engine::_get_binded_shader_program() const noexcept {
        ASSERT_SHADER_PROGRAM_ID_VALIDITY(shader_programs, binded_shader);
        return shader_programs.at(binded_shader);
    }
    
    const _shader_engine::abstract_shader_program& _shader_engine::_get_binded_abstract_shader_program() const noexcept {
        ASSERT_SHADER_PROGRAM_ID_VALIDITY(abstract_shader_programs, binded_shader);
        return abstract_shader_programs.at(binded_shader);
    }
}