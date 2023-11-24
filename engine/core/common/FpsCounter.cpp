#include "FpsCounter.hpp"

#include <SDL3/SDL.h>

namespace ige
{

    FpsCounter::FpsCounter() : m_highestFPS(0.0f), m_lowestFPS(std::numeric_limits<float>::max())
    {
        m_startTime = std::chrono::high_resolution_clock::now();
    }

    void FpsCounter::Update()
    {
        auto currentTime = std::chrono::high_resolution_clock::now();
        float deltaTime = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - m_startTime).count();
        float fps = 1.0f / deltaTime;
        m_fpsHistory.push_front(fps);

        if (SDL_GetTicks() > 1000)
        {
            m_highestFPS = std::max(m_highestFPS, fps);
            m_lowestFPS = std::min(m_lowestFPS, fps);
        }

        const size_t maxHistorySize = 5000;
        while (m_fpsHistory.size() > maxHistorySize)
        {
            m_fpsHistory.pop_back();
        }

        m_startTime = currentTime;
    }

    float FpsCounter::GetFPS() const
    {
        if (m_fpsHistory.empty())
        {
            return 0.0f;
        }
        return m_fpsHistory.front();
    }

    float FpsCounter::GetAverageFPS() const
    {
        float averageFPS = 0.0f;
        for (float value : m_fpsHistory)
        {
            averageFPS += value;
        }
        return m_fpsHistory.empty() ? 0.0f : averageFPS / static_cast<float>(m_fpsHistory.size());
    }

    float FpsCounter::GetHighestFPS() const
    {
        return m_highestFPS;
    }

    float FpsCounter::GetLowestFPS() const
    {
        return m_lowestFPS;
    }

}