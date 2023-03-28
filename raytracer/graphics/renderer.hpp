#pragma once
#include "graphics/scene.hpp"
#include "objects/camera.hpp"

#include "thread_pool/thread_pool.hpp"

#include <vector>
        

namespace gfx {
    enum class AntialiasingLevel { NONE = 1, X2 = 2, X4 = 4, X8 = 8 };

    class Renderer {
    public:
        Renderer() = default;

        const std::vector<std::uint32_t>& Render(const Scene& scene, const Camera& camera) noexcept;

    #pragma region getters-setters
        void SetAntialiasingLevel(AntialiasingLevel level) noexcept;
        AntialiasingLevel GetAntialiasingLevel() const noexcept;

        void SetOutputFrameSize(const math::vec2ui& size) noexcept;
        const math::vec2ui& GetOutputFrameSize() const noexcept;

        void SetBackgroundColor(Color color) noexcept;
        Color GetBackgroundColor() const noexcept;
        Color& GetBackgroundColor() noexcept;

        void SetReflectionDepth(std::size_t depth) noexcept;
        std::size_t GetReflectionDepth() const noexcept;
        std::size_t& GetReflectionDepth() noexcept;
    #pragma endregion getters-setters

    private:
        Color _TraceRay(const Ray& ray, const Scene& scene, std::size_t recursion_depth) const noexcept;
        void _ThreadRenderFunc(std::uint32_t x0, std::uint32_t y0, std::uint32_t x_end, std::uint32_t y_end, 
            const std::vector<gfx::Ray>& rays, const gfx::Scene& scene) noexcept;

    private:
        std::vector<std::uint32_t> m_frame;
        math::vec2ui m_frame_size;
        
        AntialiasingLevel m_antialiasing_level = AntialiasingLevel::NONE;
        std::size_t m_reflection_depth = 0;
        
        Color m_background = Color::BLACK;

        util::ThreadPool m_thread_pool = { std::thread::hardware_concurrency() };
    };
}