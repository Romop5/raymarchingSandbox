#include <iostream>

#include "bvh/BVHGenerator.hpp"
#include "bvh/BVHLibrary.hpp"
#include "helpers/Args.hpp"
#include "helpers/FileHelper.hpp"
#include <spdlog/spdlog.h>

using namespace raymarcher;

int
main(int argc, const char** argv)
{
  Arguments args;
  args.AddOpt("seed", "seed");
  args.AddLongOpt("clusters", "clusters", "c");
  args.AddLongOpt("maxLevel", "max-level", "l");
  args.AddLongOpt("SAHCoeficient", "sah", "s");
  auto parseStatus = args.Parse(argc, argv);
  if (parseStatus != Arguments::parseResult::OK) {
    spdlog::error("main: Failed to parse argumetns");
  }

  auto seed = 1337;
  if (args.HasArgument("seed")) {
    seed = stoi(args["seed"]);
  }
  auto clusters = 2;
  if (args.HasArgument("clusters")) {
    clusters = stoi(args["clusters"]);
  }
  BVHGenerator generator(seed);

  auto scene = generator.GenerateClusteredScene(
    clusters, 0.5, { glm::vec3(0.0, 3.0, 0.0), glm::vec3(5, 3, 5) });

  BVHLibrary library;
  library.SetScene(std::move(scene));
  BVHLibrary::OptimizationParameters params;

  if (args.HasArgument("maxLevel")) {
    spdlog::info("main: Overriding maxLevel with CLI argument");
    params.maxLevel = std::stoi(args["maxLevel"]);
  }
  if (args.HasArgument("SAHCoeficient")) {
    spdlog::info("main: Overriding SAHCoeficient with CLI argument");
    params.SAHthreshold = std::stof(args["SAHCoeficient"]);
  }

  spdlog::info("main: Using optimization parameters:");
  spdlog::info("main: \tclusters = {}", clusters);
  spdlog::info("main: \tseed = {}", seed);
  spdlog::info("main: \tmaxLevel = {}", params.maxLevel);
  spdlog::info("main: \tSAHCoeficient = {}", params.SAHthreshold);

  library.SetParams(params);

  library.Optimize();
  auto optimizedSDF = library.GenerateCode(true);
  FileHelper::SaveFile("optimized.sdf", optimizedSDF);
  spdlog::info("main: Storing optimized to optimized.sdf");

  auto nonOptimizedSDF = library.GenerateCode(false);
  FileHelper::SaveFile("nonoptimized.sdf", nonOptimizedSDF);
  spdlog::info("main: Storing nonoptimized to optimized.sdf");
}
