#include "window.h"

#include <cstdint>
#include <optional>
#include <print>
#include <queue>
#include <ranges>

#include "graphics/opengl.h"

#include "third_party/opengl/wglext.h"
#include <Windows.h>
#include <Windowsx.h>
#include <hidusage.h>

#include "events/event.h"
#include "events/key.h"
#include "events/key_event.h"
#include "events/mouse_button_event.h"
#include "events/mouse_event.h"
#include "events/stop_event.h"
#include "utils/auto_release.h"
#include "utils/error.h"
#include "utils/exception.h"

namespace
{

// special wgl functions we need to resolve, stored here so as not to pollute the main opengl functions
PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB{};
PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB{};

// event queue for the main window
auto g_event_queue = std::queue<game::Event>{};

/**
 * OpenGL debug callback function. Converts errors into exceptions.
 */
auto APIENTRY opengl_debug_callback(
    ::GLenum source,
    ::GLenum type,
    ::GLuint id,
    ::GLenum severity,
    ::GLsizei,
    const ::GLchar *message,
    const void *) -> void
{
    if (type == GL_DEBUG_TYPE_ERROR)
    {
        throw game::Exception("{} {} {} {} {}", source, type, id, severity, message);
    }
}

/**
 * Window procedure for the main window. This handles all the events and pushes them to the event queue.
 */
auto CALLBACK window_proc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) -> LRESULT
{
    switch (Msg)
    {
        case WM_CLOSE: g_event_queue.emplace(game::StopEvent{}); break;
        case WM_KEYUP:
        {
            g_event_queue.emplace(game::KeyEvent{static_cast<game::Key>(wParam), game::KeyState::UP});
            break;
        }
        case WM_KEYDOWN:
        {
            g_event_queue.emplace(game::KeyEvent{static_cast<game::Key>(wParam), game::KeyState::DOWN});
            break;
        }
        case WM_INPUT:
        {
            auto raw = ::RAWINPUT{};
            auto dwSize = ::UINT{sizeof(::RAWINPUT)};
            game::ensure(
                ::GetRawInputData(
                    reinterpret_cast<::HRAWINPUT>(lParam), RID_INPUT, &raw, &dwSize, sizeof(::RAWINPUTHEADER)) !=
                    static_cast<::UINT>(-1),
                "failed to get raw input");

            if (raw.header.dwType == RIM_TYPEMOUSE)
            {
                const auto x = raw.data.mouse.lLastX;
                const auto y = raw.data.mouse.lLastY;

                g_event_queue.emplace(game::MouseEvent{static_cast<float>(x), static_cast<float>(y)});
            }

            break;
        }
        case WM_LBUTTONUP:
        {
            g_event_queue.emplace(
                game::MouseButtonEvent{
                    static_cast<float>(GET_X_LPARAM(lParam)),
                    static_cast<float>(GET_Y_LPARAM(lParam)),
                    game::MouseButtonState::UP});
            break;
        }
        case WM_LBUTTONDOWN:
        {
            g_event_queue.emplace(
                game::MouseButtonEvent{
                    static_cast<float>(GET_X_LPARAM(lParam)),
                    static_cast<float>(GET_Y_LPARAM(lParam)),
                    game::MouseButtonState::DOWN});
            break;
        }
    };

    return ::DefWindowProc(hWnd, Msg, wParam, lParam);
}

/**
 * Helper method to resolve an opengl function.
 *
 * @param function
 *   Reference to the function pointer to set.
 * @param name
 *   Name of the function to resolve.
 */
template <class T>
auto resolve_gl_function(T &function, const std::string &name) -> void
{
    const auto address = ::wglGetProcAddress(name.c_str());
    game::ensure(address != nullptr, "could not resolve {}", name);

    function = reinterpret_cast<T>(address);
}

/**
 * Helper method to resolve wgl functions.
 *
 * As wgl functions are not part of the core opengl library, we need to create a dummy window and context to resolve.
 *
 * @param instance
 *   Handle to the instance.
 */
auto resolve_wgl_functions(HINSTANCE instance) -> void
{
    auto wc = ::WNDCLASS{
        .style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC,
        .lpfnWndProc = ::DefWindowProc,
        .hInstance = instance,
        .lpszClassName = "dummy window"};

    game::ensure(::RegisterClassA(&wc) != 0, "could not register dummy window");

    // create a dummy window to resolve the wgl functions

    auto dummy_window = game::AutoRelease<::HWND>{
        ::CreateWindowExA(
            0,
            wc.lpszClassName,
            wc.lpszClassName,
            0,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            0,
            0,
            wc.hInstance,
            0),
        ::DestroyWindow};

    game::ensure(dummy_window, "could not create dummy window");

    auto dc =
        game::AutoRelease<HDC>{::GetDC(dummy_window), [&dummy_window](auto dc) { ::ReleaseDC(dummy_window, dc); }};
    game::ensure(dc, "could not get dummy dc");

    auto pfd = ::PIXELFORMATDESCRIPTOR{
        .nSize = sizeof(::PIXELFORMATDESCRIPTOR),
        .nVersion = 1,
        .dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
        .iPixelType = PFD_TYPE_RGBA,
        .cColorBits = 32,
        .cAlphaBits = 8,
        .cDepthBits = 24,
        .cStencilBits = 8,
        .iLayerType = PFD_MAIN_PLANE};

    auto pixel_format = ::ChoosePixelFormat(dc, &pfd);
    game::ensure(pixel_format != 0, "failed to choose pixel format");

    game::ensure(::SetPixelFormat(dc, pixel_format, &pfd) == TRUE, "failed to set pixel format");

    const auto context = game::AutoRelease<HGLRC>{::wglCreateContext(dc), ::wglDeleteContext};
    game::ensure(context, "failed to create wgl context");

    // we can now resolve the functions
    game::ensure(::wglMakeCurrent(dc, context) == TRUE, "could not make current context");
    resolve_gl_function(wglCreateContextAttribsARB, "wglCreateContextAttribsARB");
    resolve_gl_function(wglChoosePixelFormatARB, "wglChoosePixelFormatARB");

    game::ensure(::wglMakeCurrent(dc, 0) == TRUE, "could not unbind context");
}

/**
 * Helper method to initialize opengl.
 *
 * @param dc
 *   Handle to the device context.
 */
auto init_opengl(HDC dc) -> void
{
    int pixel_format_attribs[]{
        WGL_DRAW_TO_WINDOW_ARB,
        GL_TRUE,
        WGL_SUPPORT_OPENGL_ARB,
        GL_TRUE,
        WGL_DOUBLE_BUFFER_ARB,
        GL_TRUE,
        WGL_ACCELERATION_ARB,
        WGL_FULL_ACCELERATION_ARB,
        WGL_PIXEL_TYPE_ARB,
        WGL_TYPE_RGBA_ARB,
        WGL_COLOR_BITS_ARB,
        32,
        WGL_DEPTH_BITS_ARB,
        24,
        WGL_STENCIL_BITS_ARB,
        8,
        0};

    auto pixel_format = 0;
    auto num_formats = UINT{};

    ::wglChoosePixelFormatARB(dc, pixel_format_attribs, 0, 1, &pixel_format, &num_formats);
    game::ensure(num_formats != 0u, "failed to choose a pixel format");

    auto pfd = ::PIXELFORMATDESCRIPTOR{};
    game::ensure(::DescribePixelFormat(dc, pixel_format, sizeof(pfd), &pfd) != 0, "failed to describe pixel format");
    game::ensure(::SetPixelFormat(dc, pixel_format, &pfd) == TRUE, "failed to set pixel format");

    int gl_attribs[]{
        WGL_CONTEXT_MAJOR_VERSION_ARB,
        4,
        WGL_CONTEXT_MINOR_VERSION_ARB,
        6,
        WGL_CONTEXT_PROFILE_MASK_ARB,
        WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
        0,
    };

    auto context = ::wglCreateContextAttribsARB(dc, 0, gl_attribs);
    game::ensure(context != nullptr, "failed to create wgl context");

    game::ensure(::wglMakeCurrent(dc, context) == TRUE, "failed to make current context");
}

/**
 * Helper method to resolve all the global opengl functions.
 */
auto resolve_global_gl_functions() -> void
{
    // expand x-macro to resolve all the functions
#define RESOLVE(TYPE, NAME) resolve_gl_function(NAME, #NAME);

    FOR_OPENGL_FUNCTIONS(RESOLVE)
}

/**
 * Helper method to setup opengl debug output.
 */
auto setup_debug() -> void
{
    ::glEnable(GL_DEBUG_OUTPUT);
    ::glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    ::glDebugMessageCallback(opengl_debug_callback, nullptr);
}
}

namespace game
{

Window::Window(std::uint32_t width, std::uint32_t height, std::uint32_t x, std::uint32_t y)
    : window_({})
    , dc_({})
    , wc_({})
    , width_(width)
    , height_(height)
{
    wc_ = {
        .style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC,
        .lpfnWndProc = window_proc,
        .hInstance = ::GetModuleHandleA(nullptr),
        .lpszClassName = "window class"};

    ensure(::RegisterClassA(&wc_) != 0, "failed to register class");

    ::RECT rect{.left = {}, .top = {}, .right = static_cast<int>(width), .bottom = static_cast<int>(height)};

    ensure(::AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false) != 0, "failed to resize window");

    window_ = {
        ::CreateWindowExA(
            0,
            wc_.lpszClassName,
            "game window",
            WS_OVERLAPPEDWINDOW,
            x,
            y,
            rect.right - rect.left,
            rect.bottom - rect.top,
            nullptr,
            nullptr,
            wc_.hInstance,
            nullptr),
        ::DestroyWindow};

    dc_ = game::AutoRelease<HDC>{::GetDC(window_), [this](auto dc) { ::ReleaseDC(window_, dc); }};

    ::ShowWindow(window_, SW_SHOW);
    ::UpdateWindow(window_);

    const auto rid = ::RAWINPUTDEVICE{
        .usUsagePage = HID_USAGE_PAGE_GENERIC,
        .usUsage = HID_USAGE_GENERIC_MOUSE,
        .dwFlags = RIDEV_INPUTSINK,
        .hwndTarget = window_};

    ensure(::RegisterRawInputDevices(&rid, 1, sizeof(rid)) == TRUE, "failed to register input device");

    resolve_wgl_functions(wc_.hInstance);
    init_opengl(dc_);
    resolve_global_gl_functions();
    setup_debug();

    ::glEnable(GL_DEPTH_TEST);
}

auto Window::pump_event() const -> std::optional<Event>
{
    // peek and dispatch all the messages in the queue
    auto message = ::MSG{};
    while (::PeekMessageA(&message, nullptr, 0, 0, PM_REMOVE) != 0)
    {
        ::TranslateMessage(&message);
        ::DispatchMessageA(&message);
    }

    // see if we have any of our internal events to return
    if (!std::ranges::empty(g_event_queue))
    {
        const auto event = g_event_queue.front();
        g_event_queue.pop();
        return event;
    }

    return {};
}

auto Window::swap() const -> void
{
    ::SwapBuffers(dc_);
}

auto Window::native_handle() const -> HWND
{
    return window_;
}
auto Window::width() const -> std::uint32_t
{
    return width_;
}

auto Window::height() const -> std::uint32_t
{
    return height_;
}

}
