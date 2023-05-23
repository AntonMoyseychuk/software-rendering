#include "application.hpp"
#include "math_3d/math.hpp"

#include "core/gl_api.hpp"
#include "graphics/model.hpp"

#include <iostream>
#include <memory>
#include <cassert>


namespace rasterization {
    Application::Application(const std::string &title, std::uint32_t width, std::uint32_t height, size_t fps_lock)
        : m_window(win_framewrk::Window::Get()), m_last_frame(std::chrono::steady_clock::now()), m_fps_lock(1.0f / (fps_lock > 0 ? fps_lock : 1))
    {
        m_window->Init(title, width, height);

        m_rasterizer.BindWindow(m_window);
        assert(m_rasterizer.IsWindowBinded() != nullptr);


        using namespace gfx;
        using namespace math;
        static const GLApi& core = GLApi::Get();

        Model model("..\\..\\..\\rasterizer\\assets\\human.obj");
        
        m_VBO_IBO["model"] = std::make_pair(
            core.CreateBuffer(BufferType::VERTEX, model.vertexes.data(), model.vertexes.size() * sizeof(model.vertexes[0])),
            core.CreateIndexBuffer(model.vert_indexes.data(), model.vert_indexes.size())
        );
        core.VertexAttribPointer(m_VBO_IBO["model"].first, 0, sizeof(vec3f), AttribDataType::FLOAT, sizeof(vec3f), (void*)0);

        const vec3f cube[] = {
            { -0.75f, -0.75f, 0.75f },
            { -0.75f,  0.75f, 0.75f },
            {  0.75f,  0.75f, 0.75f },
            {  0.75f, -0.75f, 0.75f },

            { -0.75f, -0.75f, -0.75f },
            { -0.75f,  0.75f, -0.75f },
            {  0.75f,  0.75f, -0.75f },
            {  0.75f, -0.75f, -0.75f }
        };
        const size_t indexes[] = {
            0, 1,
            1, 2,
            2, 3,
            3, 0,

            4, 5,
            5, 6,
            6, 7,
            7, 4,
        };

        m_VBO_IBO["cube"] = std::make_pair(
            core.CreateBuffer(BufferType::VERTEX, cube, sizeof(cube)),
            core.CreateIndexBuffer(indexes, sizeof(indexes) / sizeof(size_t))
        );
        core.VertexAttribPointer(m_VBO_IBO["cube"].first, 0, sizeof(vec3f), AttribDataType::FLOAT, sizeof(vec3f), (void*)0);
    }

    void Application::Run() noexcept {
        using namespace gfx;
        using namespace math;
        using namespace win_framewrk;

        static const GLApi& core = GLApi::Get();

        core.SetShaderUniform("light_dir", normalize(vec3f::BACKWARD + vec3f::LEFT));
        core.SetShaderUniform("polygon_color", color::GOLDEN);
        core.SetShaderUniform("line_color", color::LIME);

        mat4f rotation, translation;
        while (m_window->IsOpen()) {
            m_window->PollEvent();

            const auto dt = _LockFPS();
            std::cout << "FPS: " << std::to_string(1.0f / dt) << std::endl;

            const float angle = to_radians(dt) * 25.0f;

        #pragma region input
            if (m_window->IsKeyPressed(Key::RIGHT_ARROW)) {
                rotation = rotate_y(rotation, -angle);
            } else if (m_window->IsKeyPressed(Key::LEFT_ARROW)) {
                rotation = rotate_y(rotation, angle);
            }

            if (m_window->IsKeyPressed(Key::UP_ARROW)) {
                rotation = rotate_x(rotation, -angle);
            } else if (m_window->IsKeyPressed(Key::DOWN_ARROW)) {
                rotation = rotate_x(rotation, angle);
            }

            if (m_window->IsKeyPressed(Key::D)) {
                translation = translate(translation, vec3f::RIGHT * dt);
            } else if (m_window->IsKeyPressed(Key::A)) {
                translation = translate(translation, vec3f::LEFT * dt);
            }

            if (m_window->IsKeyPressed(Key::W)) {
                translation = translate(translation, vec3f::UP * dt);
            } else if (m_window->IsKeyPressed(Key::S)) {
                translation = translate(translation, vec3f::DOWN * dt);
            }
        #pragma endregion input

            core.SetShaderUniform("scale", scale(mat4f::IDENTITY, vec3f(0.5f)));
            core.SetShaderUniform("rotate", rotation);
            core.SetShaderUniform("translate", translation);
            m_rasterizer.Render(RenderMode::TRIANGLES, m_VBO_IBO["model"].first, m_VBO_IBO["model"].second);

            core.SetShaderUniform("scale", mat4f::IDENTITY);
            core.SetShaderUniform("rotate", mat4f::IDENTITY);
            core.SetShaderUniform("translate", mat4f::IDENTITY);
            m_rasterizer.Render(RenderMode::LINES, m_VBO_IBO["cube"].first, m_VBO_IBO["cube"].second);

            m_rasterizer.SwapBuffers(); 
            m_rasterizer.ClearBackBuffer();
        }
    }
    
    void Application::SetFPSLock(size_t fps) const noexcept {
        m_fps_lock = 1.0f / fps;
    }
    
    float Application::_LockFPS() const noexcept {
        using namespace std::chrono;

        float dt = 0.0f;
        steady_clock::time_point curr_frame;
        
        do {
            curr_frame = steady_clock::now();
            dt = duration<float>(curr_frame - m_last_frame).count();
        } while(dt < m_fps_lock);
        m_last_frame = curr_frame;

        return dt;
    }
}