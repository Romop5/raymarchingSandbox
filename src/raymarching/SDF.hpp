#ifndef RAYMARCHER_SDF_HPP
#define RAYMARCHER_SDF_HPP

#include "ISDF.hpp"
namespace raymarcher {
/**
 * @brief Represents wrapper over SDF function
 */

class SDF : public ISDF
{
public:
  explicit SDF(std::string sourceCode);
  auto GetGLSLCode() -> const std::string& override;
  auto OnUpdateParametersCallback(ge::gl::Program& program) -> void override;
  auto GetUniforms() const -> const std::vector<Uniform>& override;

private:
  auto ParseCode() -> void;
  std::string sourceCode;
  std::vector<Uniform> userUniforms;
};

}

#endif
