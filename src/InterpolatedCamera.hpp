#ifndef RAYMARCHER_INTERPOLATEDCAMERA_HPP
#define RAYMARCHER_INTERPOLATEDCAMERA_HPP

#include <vector>
#include <glm/glm.hpp>
#include "ICamera.hpp"

namespace raymarcher
{
/**
 * @brief Deterministic interpolated camera
 *
 * This camera is useful for pre-created sequences, which should be deterministic,
 * e.g. due to repeatability or for accurate performance tracking.
 *
 * The camera uses linear interpolation over a sequence of given camera points.
 */
class InterpolatedCamera: public ICamera
{
    public:

    /**
     * @brief Definition of a single viewpoint at given frame
     */
    struct CameraPoint
    {
        size_t frame;
        glm::vec3 position;
        glm::vec3 lookVector;
        glm::vec3 upVector;
    };

    InterpolatedCamera(std::vector<CameraPoint> cameraPoints);
    virtual auto GetTransformation() const -> const glm::mat4& override;

    /// Update transformation for given frameID
    auto UpdateFrame(size_t frameID) -> void;

    private:
    auto GetIndexesForFrameID(size_t frameID) -> std::pair<size_t, size_t>;
    auto GetFrameIDAsInterpolationParameter(size_t frameID, size_t start, size_t end) -> float;

    std::vector<CameraPoint> cameraPoints;
    size_t currentCameraPoint;
    glm::mat4 currentTransformation;
};
}
#endif
