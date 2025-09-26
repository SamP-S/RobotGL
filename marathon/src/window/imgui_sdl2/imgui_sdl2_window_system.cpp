#include "window/imgui_sdl2/imgui_sdl2_window_system.hpp"

// std lib
#include <iostream>
#include <stdexcept>

// external
#include <imgui.h>
#include <SDL2/SDL_opengl.h>
#include <GL/glew.h>
#include <backends/imgui_impl_sdl2.h>
#include <backends/imgui_impl_opengl3.h>

// internal
#include "core/logger.hpp"

namespace marathon {

namespace window {

namespace imgui_sdl2 {

ImGuiSDL2WindowSystem::ImGuiSDL2WindowSystem()
    : IWindowSystem("marathon.window.imgui_sdl2.imgui_sdl2_window_system") {}

SDL_Window* ImGuiSDL2WindowSystem::FindWindow(WindowID win) {
    auto it = _idTable.find(win);
    if (it == _idTable.end()) {
        MT_CORE_WARN("window/sdl2/sdl2_window_system.cpp: Can't find window matching ID.");
        return nullptr;
    }
    return it->second;
}


bool ImGuiSDL2WindowSystem::Init() {
    if (SDL_InitSubSystem(SDL_INIT_VIDEO) != 0) {
        MT_CORE_WARN("window/sdl2/sdl2_window_system.cpp: SDL_Init Error = {}", SDL_GetError());
        return false;
    }

    // OpenGL version
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

    // Set max depth & stencil buffer size
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    // Create window with graphics context
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    return true;
}

void ImGuiSDL2WindowSystem::Quit() {
    for (auto it = _idTable.begin(); it != _idTable.end(); ) {
        DestroyWindow(it->first);
        it = _idTable.begin();
    }
    _idTable.clear();
    SDL_QuitSubSystem(SDL_INIT_VIDEO);
}

// frame handling
void ImGuiSDL2WindowSystem::BeginFrame(WindowID win) {
    SDL_Window* w = FindWindow(win);
    if (!w) {
        return;
    }
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();
}

void ImGuiSDL2WindowSystem::EndFrame(WindowID win) {
    SDL_Window* w = FindWindow(win);
    if (!w) {
        return;
    }

    ImGui::Render();
    glViewport(0, 0, (int)ImGui::GetIO().DisplaySize.x, (int)ImGui::GetIO().DisplaySize.y);
    glClearColor(0.15f, 0.15f, 0.15f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    SDL_GL_SwapWindow(w);
}


WindowID ImGuiSDL2WindowSystem::CreateWindow(const std::string& title, int w, int h) {
    /// TODO: write renderer dependent code
    /// default to opengl for now
    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

    SDL_Window* window = SDL_CreateWindow(
        title.c_str(),
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        w, h,
        window_flags);

    if (!window) {
        MT_CORE_ERROR("window/sdl2/sdl2_window_system.cpp: SDL_CreateWindow error = {}", SDL_GetError());
        return 0;
    }

    WindowID id = _nextID++;
    _idTable[id] = window;

    SDL_GLContext context = SDL_GL_CreateContext(window);
    if (!context) {
        std::cerr << "ImGuiSDL2WindowSystem: Failed to create OpenGL context: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        _idTable.erase(id);
        throw std::runtime_error("Failed to create OpenGL context");
    }

    SDL_GL_MakeCurrent(window, context);
    SDL_GL_SetSwapInterval(0);

    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        std::cerr << "ImGuiSDL2WindowSystem: Failed to initialize GLEW: " << glewGetErrorString(err) << std::endl;
        SDL_GL_DeleteContext(context);
        SDL_DestroyWindow(window);
        _idTable.erase(id);
        throw std::runtime_error("Failed to initialize GLEW");
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    ImGui::StyleColorsDark();

    ImGui_ImplSDL2_InitForOpenGL(window, context);
    ImGui_ImplOpenGL3_Init("#version 330");

    return id;
}

void ImGuiSDL2WindowSystem::DestroyWindow(WindowID win) {
    SDL_Window* w = FindWindow(win);
    if (!w) {
        return;
    }
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_GLContext context = SDL_GL_GetCurrentContext();
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(w);
    _idTable.erase(win);
}

void* ImGuiSDL2WindowSystem::GetRenderContext(WindowID win) {
    SDL_Window* w = FindWindow(win);
    if (!w) {
        return nullptr;
    }
    return SDL_GL_GetCurrentContext();
}

void* ImGuiSDL2WindowSystem::GetNativeWindow(WindowID win) {
    return static_cast<void*>(FindWindow(win));
}

std::string ImGuiSDL2WindowSystem::GetWindowTitle(WindowID win) {
    SDL_Window* w = FindWindow(win);
    if (!w) {
        return "";
    }
    const char* title = SDL_GetWindowTitle(w);
    return title ? std::string(title) : "";
}

void ImGuiSDL2WindowSystem::SetWindowTitle(WindowID win, const std::string& title) {
    SDL_Window* w = FindWindow(win);
    if (!w) {
        return;
    }
    SDL_SetWindowTitle(w, title.c_str());
}

void ImGuiSDL2WindowSystem::SetWindowMinSize(WindowID win, int minWidth, int minHeight) {
    SDL_Window* w = FindWindow(win);
    if (!w) {
        return;
    }
    SDL_SetWindowMinimumSize(w, minWidth, minHeight);
}

void ImGuiSDL2WindowSystem::GetWindowMinSize(WindowID win, int& minWidth, int& minHeight) {
    SDL_Window* w = FindWindow(win);
    if (!w) {
        return;
    }
    SDL_GetWindowMinimumSize(w, &minWidth, &minHeight);
}

void ImGuiSDL2WindowSystem::SetWindowSize(WindowID win, int width, int height) {
    SDL_Window* w = FindWindow(win);
    if (!w) {
        return;
    }
    SDL_SetWindowSize(w, width, height);
}

void ImGuiSDL2WindowSystem::GetWindowSize(WindowID win, int& width, int& height) {
    SDL_Window* w = FindWindow(win);
    if (!w) {
        return;
    }
    SDL_GetWindowSize(w, &width, &height);
}

void ImGuiSDL2WindowSystem::SetCursorCapture(WindowID win, bool capture) {
    SDL_Window* w = FindWindow(win);
    if (!w) {
        return;
    }
    SDL_SetRelativeMouseMode((SDL_bool)capture);
}

bool ImGuiSDL2WindowSystem::GetCursorCapture(WindowID win) {
    SDL_Window* w = FindWindow(win);
    if (!w) {
        return false;
    }
    return SDL_GetRelativeMouseMode() == SDL_TRUE;
}

bool ImGuiSDL2WindowSystem::IsVisible(WindowID win) {
    SDL_Window* w = FindWindow(win);
    if (!w) {
        return false;
    }
    Uint32 flags = SDL_GetWindowFlags(w);
    return (flags & SDL_WINDOW_SHOWN) != 0;
}

void ImGuiSDL2WindowSystem::Show(WindowID win) {
    SDL_Window* w = FindWindow(win);
    if (!w) {
        return;
    }
    SDL_ShowWindow(w);
}

void ImGuiSDL2WindowSystem::Hide(WindowID win) {
    SDL_Window* w = FindWindow(win);
    if (!w) {
        return;
    }
    SDL_HideWindow(w);
}

} // imgui sdl2

} // window

} // marathon
