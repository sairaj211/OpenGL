#pragma once

#include <GLFW/glfw3.h>
#include <thread>
#include <chrono>
#include "imgui/imgui.h"

class FrameRateController
{
private:
    // Singleton instance
    static FrameRateController* sInstance;

    double mMaxFrameRate;
    double mNeededSecondsPerFrame;
    double mFrameStartTime;
    double mFrameEndTime;
    double mDeltaTime;

    // Private constructor/destructor
    FrameRateController();
    ~FrameRateController();

public:
    // Delete copy constructor and assignment operator
    FrameRateController(const FrameRateController&) = delete;
    FrameRateController& operator=(const FrameRateController&) = delete;

    // Singleton access
    static FrameRateController* GetInstance();

    // Frame rate control methods
    void SetMaxFrameRate(double maxFPS);
    void FrameStart();
    void FrameEnd();

    double GetDeltaTime() const;
    double GetFPS() const;

    void ImGuiRender();

    // Optional: clean up the singleton
    static void DestroyInstance();
};

