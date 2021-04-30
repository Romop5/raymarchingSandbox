#include <iostream>
#include "BVHGenerator.hpp"
#include "BVHLibrary.hpp"
#include "Args.hpp"
#include "FileHelper.hpp"

using namespace raymarcher;

int main(int argc, char** argv)
{
    Arguments args;
    args.AddLongOpt("maxLevel", "max-level", "l");
    auto parseStatus = args.Parse(argc, argv);
    if(parseStatus != Arguments::parseResult::OK)
    {
        std::cout << "Failed to parse argumetns" << std::endl;
    }

    BVHGenerator generator(1337);

    auto scene = generator.GenerateClusteredScene(2, 0.5, { glm::vec3(0.0), glm::vec3(5)});

    BVHLibrary library;
    library.SetScene(std::move(scene));
    BVHLibrary::OptimizationParameters params;
    params.maxLevel = 1;

    if(args.HasArgument("maxLevel"))
    {
        std::cout << "Overriding maxLevel with CLI argument" << std::endl;
        params.maxLevel = std::stoi(args["maxLevel"]);
    }

    std::cout << "Using optimization parameters:" << std::endl;
    std::cout << "\tmaxLevel = " << params.maxLevel << std::endl;

    library.SetParams(params);

    library.Optimize();
    auto optimizedSDF = library.GenerateCode(true);
    FileHelper::SaveFile("optimized.sdf", optimizedSDF);
    std::cout << "Storing optimized to optimized.sdf" << std::endl;

    auto nonOptimizedSDF = library.GenerateCode(false);
    FileHelper::SaveFile("nonoptimized.sdf", nonOptimizedSDF);
    std::cout << "Storing nonoptimized to optimized.sdf" << std::endl;

}
