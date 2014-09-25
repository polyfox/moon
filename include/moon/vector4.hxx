#ifndef MOON_VECTOR4_H
#define MOON_VECTOR4_H

#include <memory>
#include <glm/glm.hpp>

typedef std::shared_ptr<glm::vec4> moon_vec4;
#define moon_vec4_p(mvec4) (*mvec4)

#endif
