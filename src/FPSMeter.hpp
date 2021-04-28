#ifndef RAYMARCHER_FPSMETER_HPP
#define RAYMARCHER_FPSMETER_HPP

#include <cstdlib>
#include <array>

namespace raymarcher
{

class FPSMeter
{
    public:
    FPSMeter();
    auto Measure()          -> void;
    auto RenderOverlay()    -> void;

    auto DumpConclusion()   -> void;

    private:
    size_t perFramePeriodInMs;

    template<size_t SIZE>
    class RingBuffer
    {
        using StorageType = std::array<size_t, SIZE>;
        public:
        RingBuffer()
        {
            for(auto& period: this->buffer)
            {
                period = 0;
            }
        }
        auto Add(size_t period) -> void
        {
            this->sum += period - this->buffer[this->position];
            this->buffer[this->position++] = period;
            this->position = this->position % SIZE;
        }
        auto GetAverage() const -> double
        {
            return this->sum / double(SIZE);
        }
        auto GetArray() const -> const StorageType&
        {
            return buffer;
        }
        private:
        StorageType buffer;
        size_t position = 0;
        size_t sum = 0;
    };
    RingBuffer<20> framePeriods;
    RingBuffer<5> fps;

    size_t framesPassed = 0;
    RingBuffer<10000> totalFramePeriods;
};
}
#endif
