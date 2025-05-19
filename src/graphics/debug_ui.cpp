#include "debug_ui.h"

#include <format>
#include <ranges>
#include <string>

#include <windows.h>

#include "ImGuizmo.h"
#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_win32.h"
#include "imgui.h"

#include "graphics/camera.h"
#include "graphics/scene.h"
#include "maths/matrix4.h"

namespace game
{

DebugUI::DebugUI(HWND window, Scene &scene, const Camera &camera, float &gamma)
    : scene_(scene)
    , camera_(camera)
    , gamma_(gamma)
{
    // basic imgui setup

    IMGUI_CHECKVERSION();
    ::ImGui::CreateContext();
    auto &io = ::ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

    ::ImGui::StyleColorsDark();

    // setup imgui opengl and win32
    ::ImGui_ImplWin32_InitForOpenGL(window);
    ::ImGui_ImplOpenGL3_Init();
}

DebugUI::~DebugUI()
{
    ::ImGui_ImplOpenGL3_Shutdown();
    ::ImGui_ImplWin32_Shutdown();
    ::ImGui::DestroyContext();
}

auto DebugUI::render() const -> void
{
    auto &io = ::ImGui::GetIO();

    ::ImGui_ImplOpenGL3_NewFrame();
    ::ImGui_ImplWin32_NewFrame();
    ::ImGui::NewFrame();

    ::ImGuizmo::SetOrthographic(false);
    ::ImGuizmo::BeginFrame();
    ::ImGuizmo::Enable(true);
    ::ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);

    // hard coded layout

    static auto selected_point = std::size_t{};

    // add light button
    if (::ImGui::Button("Add Light"))
    {
        scene_.points.push_back(scene_.points.back());
        selected_point = scene_.points.size() - 1u;
    }

    // fps counter
    ::ImGui::LabelText("FPS", "%0.1f", io.Framerate);

    // gamma slider
    ::ImGui::SliderFloat("gamma", &gamma_, 0.0f, 5.0f);

    // control scene ambient light
    if (::ImGui::CollapsingHeader("ambient"))
    {
        float colour[3]{};
        std::memcpy(colour, &scene_.ambient, sizeof(colour));

        if (::ImGui::ColorPicker3("ambient colour", colour))
        {
            std::memcpy(&scene_.ambient, colour, sizeof(colour));
        }
    }

    // control scene directional light
    if (::ImGui::CollapsingHeader("direction"))
    {
        float colour[3]{};
        std::memcpy(colour, &scene_.directional.colour, sizeof(colour));

        if (::ImGui::ColorPicker3("direction colour", colour))
        {
            std::memcpy(&scene_.directional.colour, colour, sizeof(colour));
        }
    }

    // control scene point lights
    for (const auto &[index, point] : scene_.points | std::views::enumerate)
    {
        float colour[3]{};
        std::memcpy(colour, &point.colour, sizeof(colour));

        const auto header_name = std::format("colour {} ", index);
        const auto picker_name = std::format("colour {} ", index);
        const auto const_name = std::format("constant {}", index);
        const auto linear_name = std::format("linear {}", index);
        const auto quad_name = std::format("quadratic {}", index);

        if (::ImGui::CollapsingHeader(header_name.c_str()))
        {
            // setting the colour causes that light to be selected i.e. can be moved with the gizmo
            if (::ImGui::ColorPicker3(picker_name.c_str(), colour))
            {
                point.colour.r = colour[0];
                point.colour.g = colour[1];
                point.colour.b = colour[2];

                selected_point = index;
            }

            ::ImGui::SliderFloat(const_name.c_str(), &point.const_attenuation, 0.0f, 1.0f);
            ::ImGui::SliderFloat(linear_name.c_str(), &point.linear_attenuation, 0.0f, 1.0f);
            ::ImGui::SliderFloat(quad_name.c_str(), &point.quad_attenuation, 0.0f, 0.1f);
        }
    }

    auto &point = scene_.points[selected_point];
    auto translate = Matrix4{point.position};

    // gizmo for currently selected light
    ::ImGuizmo::Manipulate(
        camera_.view().data().data(),
        camera_.projection().data().data(),
        ::ImGuizmo::TRANSLATE,
        ::ImGuizmo::WORLD,
        const_cast<float *>(translate.data().data()),
        nullptr,
        nullptr,
        nullptr,
        nullptr);

    point.position.x = translate.data()[12];
    point.position.y = translate.data()[13];
    point.position.z = translate.data()[14];

    // issue opengl render calls
    ::ImGui::Render();
    ::ImGui_ImplOpenGL3_RenderDrawData(::ImGui::GetDrawData());
}

auto DebugUI::add_mouse_event(const MouseButtonEvent &evt) const -> void
{
    auto &io = ::ImGui::GetIO();
    io.AddMouseButtonEvent(0, evt.state() == MouseButtonState::DOWN);
}

}
