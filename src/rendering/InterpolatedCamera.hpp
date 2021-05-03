#ifndef RAYMARCHER_INTERPOLATEDCAMERA_HPP
#define RAYMARCHER_INTERPOLATEDCAMERA_HPP

#include <vector>
#include <glm/glm.hpp>
#include "rendering/ICamera.hpp"

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
    /// Get indices of start and end CameraPoint for which frameID is in interval
    auto GetIndexesForFrameID(size_t frameID) -> std::pair<size_t, size_t>;

    /// Calculates interpolation parameter 't' from current frameID and current start/end frame times
    auto GetFrameIDAsInterpolationParameter(size_t frameID, size_t start, size_t end) -> float;

    /// Storage of camera sequence
    std::vector<CameraPoint> cameraPoints;

    /// Cache: Associated id of last camera point, used for start point
    size_t currentCameraPoint;

    /// View transformation, updated in UpdateFrame()
    glm::mat4 currentTransformation;
};
}
#endif
