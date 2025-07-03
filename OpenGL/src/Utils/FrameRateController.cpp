#include "FrameRateController.h"

// Initialize static instance pointer
FrameRateController* FrameRateController::sInstance = nullptr;

FrameRateController::FrameRateController()
    : mMaxFrameRate(60.0),
    mNeededSecondsPerFrame(1.0 / 60.0),
    mFrameStartTime(0.0),
    mFrameEndTime(0.0),
    mDeltaTime(0.0)
{
}

FrameRateController::~FrameRateController()
{
}

FrameRateController* FrameRateController::GetInstance()
{
    if (!sInstance)
        sInstance = new FrameRateController();
    return sInstance;
}

void FrameRateController::DestroyInstance()
{
    if (sInstance)
    {
        delete sInstance;
        sInstance = nullptr;
    }
}

void FrameRateController::SetMaxFrameRate(double maxFPS)
{
    mMaxFrameRate = maxFPS;
    if (mMaxFrameRate > 0)
        mNeededSecondsPerFrame = 1.0 / mMaxFrameRate;
    else
        mNeededSecondsPerFrame = 0;
}

void FrameRateController::FrameStart()
{
    mFrameStartTime = glfwGetTime();
}

void FrameRateController::FrameEnd()
{
    mFrameEndTime = glfwGetTime();
    double elapsed = mFrameEndTime - mFrameStartTime;

    while (elapsed < mNeededSecondsPerFrame)
    {
      //  std::this_thread::sleep_for(std::chrono::milliseconds(1));
        mFrameEndTime = glfwGetTime();
        elapsed = mFrameEndTime - mFrameStartTime;
    }

    mDeltaTime = elapsed;
}


//void FrameRateController::FrameEnd()
//{
//    const double frameDuration = glfwGetTime() - mFrameStartTime;
//
//    // Time to wait to match target FPS
//    const double targetFrameTime = mNeededSecondsPerFrame;
//    const double timeToWait = targetFrameTime - frameDuration;
//
//    if (timeToWait > 0.0)
//    {
//        // Sleep for 90% of the wait time (to avoid oversleep)
//        double sleepTime = timeToWait * 0.9;
//        std::this_thread::sleep_for(std::chrono::duration<double>(sleepTime));
//
//        // Spin-wait the remaining time
//        while ((glfwGetTime() - mFrameStartTime) < targetFrameTime) {}
//    }
//
//    mDeltaTime = glfwGetTime() - mFrameStartTime;
//}

double FrameRateController::GetDeltaTime() const
{
    return mDeltaTime;
}

double FrameRateController::GetFPS() const
{
    return (mDeltaTime > 0) ? (1.0 / mDeltaTime) : 0;
}

void FrameRateController::ImGuiRender()
{
    ImGui::Begin("Frame Rate Controller");
    ImGui::Text("Current FPS: %.1f", GetFPS());

    static float targetFPS = static_cast<float>(mMaxFrameRate);
    if (ImGui::SliderFloat("Target FPS", &targetFPS, 10.0f, 144.0f))
    {
        SetMaxFrameRate(targetFPS);
    }
    ImGui::End();
}
