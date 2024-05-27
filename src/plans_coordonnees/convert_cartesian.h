#include <cmath>

#include "ofMain.h"

namespace engine {
class ConvertCartesian {
  public:
    static glm::vec2 convertCartesianToPolar(const glm::vec3& coordinates);

    static glm::vec3 convertCartesianToCylindrical(const glm::vec3& coordinates);

    static glm::vec3 convertCartesianToSpherical(const glm::vec3& coordinates);

    static glm::vec3 convertCylindricalToCartesian(const glm::vec3& cylindricalCoordinates);

    static glm::vec3 convertSphericalToCartesian(const glm::vec3& sphericalCoordiantes);
};

}  // namespace engine