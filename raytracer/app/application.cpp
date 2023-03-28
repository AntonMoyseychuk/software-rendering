#include "application.hpp"
#include "objects/sphere.hpp"
#include "objects/point_light.hpp"
#include "objects/directional_light.hpp"
#include "objects/ambient_light.hpp"

#include <iostream>
#include <memory>

namespace app {
    Application::Application(const std::string &title, std::uint32_t width, std::uint32_t height)
        : m_window(win_framewrk::Window::Get()), m_renderer(), m_scene(), 
            m_camera(math::vec3f(0.0f), math::VECTOR_BACKWARD, math::VECTOR_UP, 45.0f, (float)width / height), 
                m_last_frame(std::chrono::steady_clock::now())
    {
        m_window->Init(title, width, height);
        m_window->SetBackgroundColor(gfx::Color(80).rgba);
        
        m_renderer.SetAntialiasingLevel(gfx::AntialiasingLevel::X2);
        m_renderer.SetReflectionDepth(2);

        // for (std::size_t i = 0; i < 100; ++i) {
        //     m_scene.AddDrawble(std::make_shared<gfx::Sphere>(
        //             math::vec3f(math::Random(-9.0f, 9.0f), math::Random(-9.0f, 9.0f), math::Random(-9.0f, 9.0f)),
        //             math::Random(0.5f, 1.0f), 
        //             gfx::Material(gfx::Color(math::Random(0, 255), math::Random(0, 255), math::Random(0, 255)), 500.0f)
        //         )
        //     );
        // }
        
        m_scene.AddDrawble(std::make_shared<gfx::Sphere>(math::vec3f(0.0f, 0.0f, -3.0f), 0.4f, gfx::Material(gfx::Color::MAGENTA, 500)));
        
        m_scene.AddDrawble(std::make_shared<gfx::Sphere>(math::vec3f(-1.0f, 0.5f, -4.0f), 0.5f, gfx::Material(gfx::Color::RED, 500)));
        m_scene.AddDrawble(std::make_shared<gfx::Sphere>(math::vec3f(0.0f, 0.5f, -4.0f), 0.5f, gfx::Material(gfx::Color::GREEN, 500, 0.2f)));
        m_scene.AddDrawble(std::make_shared<gfx::Sphere>(math::vec3f(1.0f, 0.5f, -4.0f), 0.5f, gfx::Material(gfx::Color::BLUE, 500, 0.4f)));
        m_scene.AddDrawble(std::make_shared<gfx::Sphere>(math::vec3f(-1.0f, -0.5f, -4.0f), 0.5f, gfx::Material(gfx::Color::YELLOW, 500, 0.6f)));
        m_scene.AddDrawble(std::make_shared<gfx::Sphere>(math::vec3f(0.0f, -0.5f, -4.0f), 0.5f, gfx::Material(gfx::Color::CYAN, 500, 0.8f)));
        m_scene.AddDrawble(std::make_shared<gfx::Sphere>(math::vec3f(1.0f, -0.5f, -4.0f), 0.5f, gfx::Material(gfx::Color::WHITE, 500, 1.0f)));


        m_scene.AddLight(std::make_shared<gfx::DirectionalLigth>(math::vec3f(1.0f, -1.0f, -2.0f), gfx::Color::WHITE, 1.0f));
        // m_scene.AddLight(std::make_shared<gfx::DirectionalLigth>(math::VECTOR_RIGHT, gfx::Color::WHITE, 1.0f));
        // m_scene.AddLight(std::make_shared<gfx::PointLigth>(math::vec3f(8.0f, -10.0f, 8.0f), gfx::Color::WHITE, 1.0f));
        // m_scene.AddLight(std::make_shared<gfx::PointLigth>(math::vec3f(-8.0f, -10.0f, 8.0f), gfx::Color::WHITE, 1.0f));
        m_scene.AddLight(std::make_shared<gfx::AmbientLight>(gfx::Color::WHITE, 0.1f));
    }
    
    void Application::Run() noexcept {
        m_renderer.SetBackgroundColor(gfx::UInt32ToColor(m_window->GetBackgroundColor()));
        math::vec2ui actual_window_size;

        while (m_window->IsOpen()) {
            const auto curr_frame = std::chrono::steady_clock::now();
            const auto dt = std::chrono::duration<float>(curr_frame - m_last_frame).count();
            m_last_frame = curr_frame;

            m_window->PollEvent();

            actual_window_size.x = m_window->GetWidth();
            actual_window_size.y = m_window->GetHeight();
            m_camera.SetAspectRatio((float)actual_window_size.x / actual_window_size.y);

            m_renderer.SetOutputFrameSize(actual_window_size);

            this->_UpdateLight(m_scene.GetLights().begin()->get(), dt);
            this->_UpdateDrawable(m_scene.GetDrawables().begin()->get(), dt);

            auto& buffer = m_renderer.Render(m_scene, m_camera);
            
            m_window->FillPixelBuffer(buffer);
            m_window->PresentPixelBuffer();          
           
            std::cout << "FPS: " << std::to_string(1.0f / dt) << std::endl;
        }
    }
    
    void Application::_UpdateCamera(gfx::Camera* camera, float dt) noexcept {

    }
    
    void Application::_UpdateDrawable(gfx::IDrawable* drawable, float dt) noexcept {
        using namespace win_framewrk;
    
        if (drawable != nullptr) {
            if (m_window->IsKeyPressed(Key::SPASE) == false) {
                if (m_window->IsKeyPressed(Key::W)) {
                    drawable->MoveFor(math::VECTOR_UP * 2.0f * dt);
                }
                if (m_window->IsKeyPressed(Key::A)) {
                    drawable->MoveFor(math::VECTOR_LEFT * 2.0f * dt);
                }
                if (m_window->IsKeyPressed(Key::S)) {
                    drawable->MoveFor(math::VECTOR_DOWN * 2.0f * dt);
                }
                if (m_window->IsKeyPressed(Key::D)) {
                    drawable->MoveFor(math::VECTOR_RIGHT * 2.0f * dt);
                }
                if (m_window->IsKeyPressed(Key::UP_ARROW)) {
                    drawable->MoveFor(math::VECTOR_BACKWARD * 2.0f * dt);
                }
                if (m_window->IsKeyPressed(Key::DOWN_ARROW)) {
                    drawable->MoveFor(math::VECTOR_FORWARD * 2.0f * dt);
                }
            }
        }
    }

    void Application::_UpdateLight(gfx::ILight* light, float dt) noexcept {
        using namespace win_framewrk;
        
        gfx::PointLigth* point_light = nullptr;
        if ((point_light = dynamic_cast<gfx::PointLigth*>(light)) != nullptr) {
            if (m_window->IsKeyPressed(Key::SPASE)) {
                if (m_window->IsKeyPressed(Key::W)) {
                point_light->MoveFor(math::VECTOR_UP * 2.0f * dt);
                }
                if (m_window->IsKeyPressed(Key::A)) {
                    point_light->MoveFor(math::VECTOR_LEFT * 2.0f * dt);
                }
                if (m_window->IsKeyPressed(Key::S)) {
                    point_light->MoveFor(math::VECTOR_DOWN * 2.0f * dt);
                }
                if (m_window->IsKeyPressed(Key::D)) {
                    point_light->MoveFor(math::VECTOR_RIGHT * 2.0f * dt);
                }
                if (m_window->IsKeyPressed(Key::UP_ARROW)) {
                    point_light->MoveFor(math::VECTOR_BACKWARD * 2.0f * dt);
                }
                if (m_window->IsKeyPressed(Key::DOWN_ARROW)) {
                    point_light->MoveFor(math::VECTOR_FORWARD * 2.0f * dt);
                }
            }
        }
    }
}