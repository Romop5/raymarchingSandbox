#include <chrono>
#include <iostream>

#include "imgui.h"

#include "widgets/FPSMeter.hpp"

using namespace raymarcher;
using namespace std::chrono;

FPSMeter::FPSMeter() {}

auto
FPSMeter::Measure() -> void
{
  framesPassed += 1;

  static high_resolution_clock::time_point lastFrame =
    high_resolution_clock::now();
  high_resolution_clock::time_point now = high_resolution_clock::now();

  perFramePeriodInMs =
    duration_cast<std::chrono::milliseconds>(now - lastFrame).count();
  framePeriods.Add(perFramePeriodInMs);
  totalFramePeriods.Add(perFramePeriodInMs);

  bool hasAnotherSecondPassed =
    duration_cast<std::chrono::seconds>(now.time_since_epoch()).count() -
    duration_cast<std::chrono::seconds>(lastFrame.time_since_epoch()).count();
  if (hasAnotherSecondPassed) {
    fps.Add(framesPassed);
    framesPassed = 0;
  }

  lastFrame = now;

  std::cout << "Frame ID: " << perFramePeriodInMs << std::endl;
}

auto
FPSMeter::RenderOverlay() -> void
{
  ImGui::SetNextWindowSize(ImVec2(0.0, 0.0));
  ImGui::SetNextWindowBgAlpha(0.1);
  ImGui::Begin("Overlay", nullptr, ImGuiWindowFlags_NoDecoration);
  ImGui::Text("Frame period: %d", perFramePeriodInMs);

  auto avgPeriod = framePeriods.GetAverage();
  auto avgFrames = fps.GetAverage();

  ImGui::Text("Period avg: %f", avgPeriod);
  ImGui::Text("FPS passed: %d", framesPassed);
  ImGui::Text("FPS avg: %f", avgFrames);
  ImGui::End();
}

auto
FPSMeter::DumpConclusion() -> void
{
  // Compute avg. with discarding frames that has not been reached
  const auto& data = totalFramePeriods.GetArray();
  size_t count = 0;
  size_t sum = 0;
  for (auto& period : data) {
    if (period == 0)
      continue;

    count++;
    sum += period;
  }

  const auto totalSeconds = sum / 1000.0;
  std::cout << "Total time:" << totalSeconds << " seconds " << std::endl;
  std::cout << "Total frames:" << count << std::endl;
  std::cout << "Average frame period:" << (sum / double(count)) << std::endl;
  std::cout << "Average FPS:" << (count / totalSeconds) << std::endl;
}
