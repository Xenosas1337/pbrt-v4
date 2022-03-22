// pbrt is Copyright(c) 1998-2020 Matt Pharr, Wenzel Jakob, and Greg Humphreys.
// The pbrt source code is licensed under the Apache License, Version 2.0.
// SPDX: Apache-2.0

#ifndef PBRT_UTIL_GUI_H
#define PBRT_UTIL_GUI_H

#include <pbrt/gpu/cudagl.h>
#include <GLFW/glfw3.h>

#include <pbrt/pbrt.h>

#ifdef PBRT_BUILD_GPU_RENDERER
#include <pbrt/gpu/cudagl.h>
#endif //  PBRT_BUILD_GPU_RENDERER
#include <pbrt/util/color.h>
#include <pbrt/util/transform.h>
#include <pbrt/util/vecmath.h>

#include <set>
#include <string>

namespace pbrt {

enum DisplayState {
    EXIT,
    RESET,
    NONE
};

class GLDisplay;

class GUI {
 public:
    GUI(std::string title, Vector2i resolution);
    ~GUI();

    RGB *MapFramebuffer() {
#ifdef PBRT_BUILD_GPU_RENDERER
        if (cudaFramebuffer)
            return cudaFramebuffer->map();
        else
#endif // PBRT_BUILD_GPU_RENDERER
            return cpuFramebuffer;
    }
    void UnmapFramebuffer() {
#ifdef PBRT_BUILD_GPU_RENDERER
        if (cudaFramebuffer)
            cudaFramebuffer->unmap();
#endif // PBRT_BUILD_GPU_RENDERER
    }

    DisplayState RefreshDisplay();

    Transform GetCameraTransform() const {
        return movingFromCamera;
    }
    bool denoiserEnabled = false;
    Float exposure = 1.f;

    void keyboardCallback(GLFWwindow *window, int key, int scan, int action,
                          int mods);
 private:
    bool processKeys();

    std::set<char> keysDown;
    Float moveScale = 1.f;
    Transform movingFromCamera;
    Vector2i resolution;

#ifdef PBRT_BUILD_GPU_RENDERER
    CUDAOutputBuffer<RGB> *cudaFramebuffer = nullptr;
#endif
    RGB *cpuFramebuffer = nullptr;
    GLDisplay *glDisplay = nullptr;
    GLFWwindow *window = nullptr;
};

} // namespace pbrt

#endif // PBRT_UTIL_GUI_H