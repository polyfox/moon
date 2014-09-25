#ifndef MOON_VECTOR3_H
#define MOON_VECTOR3_H

#include <memory>
#include <glm/glm.hpp>

typedef std::shared_ptr<glm::vec3> moon_vec3;
#define moon_vec3_p(mvec3) (*mvec3)

#endif
