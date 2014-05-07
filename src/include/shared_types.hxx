#ifndef MOON_SHARED_TYPES_H
#define MOON_SHARED_TYPES_H

#include <memory>
#include <glm/glm.hpp>

typedef std::shared_ptr<glm::vec2> moon_vec2;
typedef std::shared_ptr<glm::vec3> moon_vec3;
typedef std::shared_ptr<glm::vec4> moon_vec4;

typedef std::shared_ptr<glm::mat4> moon_mat4;

#define moon_vec2_p(mvec2) (*mvec2)
#define moon_vec3_p(mvec3) (*mvec3)
#define moon_vec4_p(mvec4) (*mvec4)

#define moon_mat4_p(mmat4) (*mmat4)

#endif