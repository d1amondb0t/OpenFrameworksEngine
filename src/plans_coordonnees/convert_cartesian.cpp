#include "convert_cartesian.h"

namespace engine {


glm::vec2 ConvertCartesian::convertCartesianToPolar(const glm::vec3& coordinates) {
    glm::vec2 polarCoordinates;

    polarCoordinates.x = sqrt(pow(coordinates.x, 2) + pow(coordinates.y, 2));    
    polarCoordinates.y = atan2(coordinates.y, coordinates.x);

    return polarCoordinates;
}

glm::vec3 ConvertCartesian::convertCartesianToCylindrical(const glm::vec3& coordinates) {
    glm::vec3 cylindricalCoordinates;

    cylindricalCoordinates.x = atan2(coordinates.y, coordinates.x);
    cylindricalCoordinates.y = sqrt(pow(coordinates.x, 2) + pow(coordinates.y, 2));
    cylindricalCoordinates.z = coordinates.z;

    return cylindricalCoordinates;
}

glm::vec3 ConvertCartesian::convertCartesianToSpherical(const glm::vec3& coordinates) {
    glm::vec3 sphericalCoordinates;

    sphericalCoordinates.x = glm::length(coordinates);
    sphericalCoordinates.y = atan2(coordinates.y, coordinates.x);
    sphericalCoordinates.z = acos(coordinates.z / sphericalCoordinates.x);

    return sphericalCoordinates;
}

glm::vec3 ConvertCartesian::convertCylindricalToCartesian(const glm::vec3& cylindricalCoordinates) {
    glm::vec3 coordinatesCartesians;

    coordinatesCartesians.x = cylindricalCoordinates.x * cos(cylindricalCoordinates.y);
    coordinatesCartesians.y = cylindricalCoordinates.x * sin(cylindricalCoordinates.y);
    coordinatesCartesians.z = cylindricalCoordinates.z;
    return coordinatesCartesians;
}
glm::vec3 ConvertCartesian::convertSphericalToCartesian(const glm::vec3& sphericalCoordinates) {
    glm::vec3 coordinatesCartesians;

    coordinatesCartesians.x = sphericalCoordinates.x * sin(sphericalCoordinates.z) * cos(sphericalCoordinates.y);
    coordinatesCartesians.y = sphericalCoordinates.x * sin(sphericalCoordinates.z) * sin(sphericalCoordinates.y);
    coordinatesCartesians.z = sphericalCoordinates.x * cos(sphericalCoordinates.z);

    return coordinatesCartesians;
}

}  // namespace engine