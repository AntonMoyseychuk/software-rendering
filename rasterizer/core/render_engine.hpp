#pragma once
#include "window/window.hpp"

#include "math_3d/vec4.hpp"

#include <any>

namespace rasterization::gfx {
    enum class render_mode : uint8_t { POINTS, LINES, LINE_STRIP, TRIANGLES };

    struct _render_engine final {
        _render_engine(const _render_engine& engine) = delete;
        _render_engine& operator=(const _render_engine& engine) = delete;

        static _render_engine& get() noexcept;
        
        bool bind_window(win_framewrk::Window* window) noexcept;
        const win_framewrk::Window* is_window_binded() const noexcept;

        void render(render_mode mode) const noexcept;
        void swap_buffers() const noexcept;
        void clear_depth_buffer() const noexcept;

        void set_clear_color(const math::color& color) noexcept;

    private:
        _render_engine() noexcept = default;

    private:
        void _resize_z_buffer(uint32_t width, uint32_t height) const noexcept;
        bool _test_and_update_depth(const math::vec3f& pixel) const noexcept;

    private:
        static bool _is_inside_clipping_space(const math::vec3f& point) noexcept;
        
        /**
         * returns:
         *  > 0: point is to the right of the edge
         *    0: point is on the edge
         *  < 0: point is to the left of the edge
        */
        static float _edge(const math::vec2f& v0, const math::vec2f& v1, const math::vec2f& p) noexcept;

    private:
        void _render_pixel(const math::vec2f& pixel, const math::color& color) const noexcept;
        void _render_line(const math::vec3f& pix0, const math::vec3f& pix1, const math::color& color) const noexcept;

        struct _shader;
        struct vs_intermediate_data {
            vs_intermediate_data() = default;
            vs_intermediate_data(const std::any& vs_out, const math::vec4f& coord) 
                : vs_out(vs_out), coord(coord) {}

            std::any vs_out;
            math::vec4f coord;
        };
        void _render_triangle(const vs_intermediate_data& v0, const vs_intermediate_data& v1, const vs_intermediate_data& v2) const noexcept;

    private:
        mutable std::vector<float> m_z_buffer;

        win_framewrk::Window* m_window_ptr = nullptr;
        math::color m_clear_color = math::color::BLACK;
    };
}