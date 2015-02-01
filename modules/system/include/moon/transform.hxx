#ifndef MOON_TRANSFORM_H
#define MOON_TRANSFORM_H

#include <memory>
#include <glm/glm.hpp>

typedef std::shared_ptr<glm::mat4> moon_mat4;
#define moon_mat4_p(mmat4) (*mmat4)

#endif
