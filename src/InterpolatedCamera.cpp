#include "InterpolatedCamera.hpp"
#include <glm/gtx/transform.hpp>

using namespace raymarcher;

namespace
{
    template<typename T>
    T lerp(T a, T b, float t)
    {
        return a*(1.0f-t) + b*t;
    }
}

InterpolatedCamera::InterpolatedCamera(std::vector<CameraPoint> points) :
    cameraPoints { std::move(points) }
{
}

auto InterpolatedCamera::GetTransformation() const -> const glm::mat4&
{
    return currentTransformation;
}

auto InterpolatedCamera::UpdateFrame(size_t frameID) -> void
{
    /* Camera position is calculated as linear interpolation of two successive camera points 
     * Each point has 'frame' member, which defines start of the camera point in sequence. */
    auto [start, end] = GetIndexesForFrameID(frameID);
    auto& startP = cameraPoints[start];
    auto& endP= cameraPoints[end];

    auto t = GetFrameIDAsInterpolationParameter(frameID,startP.frame, endP.frame);

    auto position   = lerp(startP.position,     endP.position,      t);
    auto lookVector = glm::normalize(lerp(startP.lookVector,   endP.lookVector,    t));
    auto upVector   = glm::normalize(lerp(startP.upVector,     endP.upVector,      t));

    auto rotation = glm::mat4(glm::mat3(glm::normalize(glm::cross(upVector, lookVector)),
                                        upVector,
                                        lookVector));

    currentTransformation = glm::translate(-position)*rotation;
}


auto InterpolatedCamera::GetIndexesForFrameID(size_t frameID) -> std::pair<size_t, size_t>
{
    assert(cameraPoints.size() > 0);
    size_t startIndex = currentCameraPoint;
    size_t endIndex = currentCameraPoint;
    if (cameraPoints.size() > currentCameraPoint)
    {
        startIndex = 0;
        endIndex = 0;
    }

    for(size_t i = startIndex; i < cameraPoints.size(); i++)
    {
        if(cameraPoints[i].frame > frameID)
        {
            endIndex = i;
            startIndex = i - 1;
            break;
        }
    }
    if(frameID > cameraPoints.back().frame)
    {
        startIndex = endIndex = cameraPoints.size()-1;
    }
    return std::make_pair(startIndex, endIndex);
}

auto InterpolatedCamera::GetFrameIDAsInterpolationParameter(size_t frameID, size_t start, size_t end) -> float
{
    if(frameID > end)
    {
        return 1.0;
    }
    auto range = end-start;
    auto relativeStart = frameID - start;
    return relativeStart/float(range);
}

