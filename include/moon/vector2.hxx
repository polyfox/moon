#ifndef MOON_VECTOR2_H
#define MOON_VECTOR2_H

#include <memory>
#include <glm/glm.hpp>

typedef std::shared_ptr<glm::vec2> moon_vec2;
#define moon_vec2_p(mvec2) (*mvec2)

#endif
