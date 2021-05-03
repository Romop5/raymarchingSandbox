#include <iostream>
#include "bvh/BVHGenerator.hpp"
#include "bvh/BVHLibrary.hpp"
#include "helpers/Args.hpp"
#include "helpers/FileHelper.hpp"

using namespace raymarcher;

int main(int argc, const char** argv)
{
    Arguments args;
    args.AddOpt("seed", "seed");
    args.AddLongOpt("clusters", "clusters", "c");
    args.AddLongOpt("maxLevel", "max-level", "l");
    args.AddLongOpt("SAHCoeficient", "sah", "s");
    auto parseStatus = args.Parse(argc, argv);
    if(parseStatus != Arguments::parseResult::OK)
    {
        std::cout << "Failed to parse argumetns" << std::endl;
    }
    
    auto seed = 1337;
    if(args.HasArgument("seed"))
    {
        seed = stoi(args["seed"]);
    }
    auto clusters = 2;
    if(args.HasArgument("clusters"))
    {
        clusters = stoi(args["clusters"]);
    }
    BVHGenerator generator(seed);

    auto scene = generator.GenerateClusteredScene(clusters, 0.5, { glm::vec3(0.0, 3.0, 0.0), glm::vec3(5, 3, 5)});

    BVHLibrary library;
    library.SetScene(std::move(scene));
    BVHLibrary::OptimizationParameters params;

    if(args.HasArgument("maxLevel"))
    {
        std::cout << "Overriding maxLevel with CLI argument" << std::endl;
        params.maxLevel = std::stoi(args["maxLevel"]);
    }
    if(args.HasArgument("SAHCoeficient"))
    {
        std::cout << "Overriding SAHCoeficient with CLI argument" << std::endl;
        params.SAHthreshold = std::stof(args["SAHCoeficient"]);
    }

    std::cout << "Using optimization parameters:" << std::endl;
    std::cout << "\tclusters = " << clusters << std::endl;
    std::cout << "\tseed = " << seed << std::endl;
    std::cout << "\tmaxLevel = " << params.maxLevel << std::endl;
    std::cout << "\tSAHCoeficient = " << params.SAHthreshold << std::endl;

    library.SetParams(params);

    library.Optimize();
    auto optimizedSDF = library.GenerateCode(true);
    FileHelper::SaveFile("optimized.sdf", optimizedSDF);
    std::cout << "Storing optimized to optimized.sdf" << std::endl;

    auto nonOptimizedSDF = library.GenerateCode(false);
    FileHelper::SaveFile("nonoptimized.sdf", nonOptimizedSDF);
    std::cout << "Storing nonoptimized to optimized.sdf" << std::endl;

}
