#pragma once

#include "logger.h"
#include "noncopyable.h"
#include "stopwatch.h"
#include "vmath.h"

#define VULKAN_HPP_DISPATCH_LOADER_DYNAMIC 1
#include <vulkan/vulkan.hpp>
// glfw must come after vulkan
#include <GLFW/glfw3.h>

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#include <optional>
#include <vector>

struct GlfwHandler
{
    static GlfwHandler &
    Inst()
    {
        static GlfwHandler singleton;
        return singleton;
    }

    std::vector<const char *>
    query_glfw_extensions() const
    {
        // query extensions and number of extensions
        uint32_t      num_glfw_extension = 0;
        const char ** glfw_extensions    = glfwGetRequiredInstanceExtensions(&num_glfw_extension);

        // turn it into vector
        std::vector<const char *> result(num_glfw_extension);
        for (size_t i_ext = 0; i_ext < num_glfw_extension; i_ext++)
        {
            result[i_ext] = glfw_extensions[i_ext];
        }

        // return result
        return result;
    }

    void
    poll_events()
    {
        glfwPollEvents();
    }

private:
    GlfwHandler()
    {
        Logger::Info("GLFWwindow instance is being constructed");

        // init glfw3
        auto init_result = glfwInit();
        if (init_result == GLFW_FALSE)
        {
            Logger::Critical<true>("glfwInit failed");
        }

        // check if vulkan is supported or not
        auto is_vulkan_supported = glfwVulkanSupported();
        if (is_vulkan_supported == GLFW_FALSE)
        {
            Logger::Critical<true>("glfw does not support vulkan");
            exit(0);
        }
    }

    GlfwHandler(const GlfwHandler &) = delete;

    ~GlfwHandler() { glfwTerminate(); }
};

struct Window
{
    GLFWwindow *                  m_glfw_window = nullptr;
    std::string                   m_title;
    AvgFrameTimeStopWatch         m_stop_watch;
    std::optional<vk::SurfaceKHR> m_vk_surface;

    Window() {}

    Window(const std::string title, const int2 & resolution)
    {
        // call instance at least once to init singleton
        GlfwHandler::Inst();

        // setup glfw hints
        glfwDefaultWindowHints();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        // create glfw windows
        m_title       = title;
        m_glfw_window = glfwCreateWindow(resolution.x, resolution.y, title.c_str(), nullptr, nullptr);
        if (m_glfw_window == nullptr)
        {
            Logger::Critical<true>("glfw could not create window");
        }

        glfwSetWindowUserPointer(m_glfw_window, this);
        // glfwSetFramebufferSizeCallback(m_glfw_window, framebufferResizeCallback);

        // focus on created glfw window
        glfwMakeContextCurrent(m_glfw_window);
    }

    Window(Window && rhs)
    {
        assert(rhs.m_glfw_window != nullptr);
        m_glfw_window     = rhs.m_glfw_window;
        m_title           = rhs.m_title;
        m_stop_watch      = m_stop_watch;
        rhs.m_glfw_window = nullptr;
    }

    Window &
    operator=(Window && rhs)
    {
        if (this != &rhs)
        {
            assert(rhs.m_glfw_window != nullptr);
            m_glfw_window     = rhs.m_glfw_window;
            m_title           = rhs.m_title;
            m_stop_watch      = m_stop_watch;
            rhs.m_glfw_window = nullptr;
        }
        return *this;
    }

    ~Window()
    {
        if (m_glfw_window != nullptr)
        {
            glfwDestroyWindow(m_glfw_window);
        }
    }

    MAKE_NONCOPYABLE(Window);

    HWND
    get_hwnd() const
    {
        return glfwGetWin32Window(m_glfw_window);
    }

    int2
    get_resolution() const
    {
        int2 result;
        glfwGetWindowSize(m_glfw_window, &result.x, &result.y);
        return result;
    }

    bool
    should_close_window() const
    {
        return glfwWindowShouldClose(m_glfw_window);
    }

    void
    update()
    {
        m_stop_watch.tick();
        if (m_stop_watch.m_just_updated)
        {
            std::string title =
                m_title + " [" + std::to_string(m_stop_watch.m_average_frame_time) + "]";
            glfwSetWindowTitle(m_glfw_window, title.c_str());
        }
    }
};