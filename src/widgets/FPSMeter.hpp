#ifndef RAYMARCHER_FPSMETER_HPP
#define RAYMARCHER_FPSMETER_HPP

#include "helpers/RingBuffer.hpp"
#include <array>
#include <cstdlib>

namespace raymarcher {

/**
 * @brief Collects statistics about FPS during run of the program
 *
 * A helper structure RingBuffer is used to effictively store last
 * N values with computed average. This serves as low-pass filter
 * of FPS average.
 */
class FPSMeter
{
public:
  FPSMeter();
  /// Update statistics for current frame
  auto Measure() -> void;

  /// Render overlay with FPS etc.
  auto RenderOverlay() -> void;

  /// Print statistics to stdout
  auto DumpConclusion() -> void;

private:
  /// Frames, passed since beginning of measurement
  size_t framesPassed = 0;

  /// Duration of last frame in milliseconds
  size_t perFramePeriodInMs;

  /// Last 20 frame periods
  RingBuffer<20> framePeriods;

  /// Last 5 frames per second
  RingBuffer<5> fps;

  /// Statistics over whole recording
  RingBuffer<10000> totalFramePeriods;
};
}
#endif
