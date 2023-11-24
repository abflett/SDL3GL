#pragma once

#include <deque>
#include <chrono>

namespace ige
{

    class FpsCounter
    {
    public:
        FpsCounter();

        void Update();
        float GetFPS() const;
        float GetAverageFPS() const;
        float GetHighestFPS() const;
        float GetLowestFPS() const;

    private:
        std::deque<float> m_fpsHistory;
        float m_highestFPS;
        float m_lowestFPS;
        std::chrono::time_point<std::chrono::high_resolution_clock> m_startTime;
    };

}