/* Include this file instead of the vendor glm headers directly to ensure
   consistency in the moon implementation */
#ifndef MOON_GLM_H
#define MOON_GLM_H

/* GLM */
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/random.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/compatibility.hpp>
#include <glm/gtx/rotate_vector.hpp>

static inline glm::mat4
moon_rotate(float angle, const glm::vec2 origin) {
  return glm::translate(glm::rotate(
    glm::mat4(1.0f),
    glm::radians(angle),
    glm::vec3(0, 0, 1)
  ), glm::vec3(-origin.x, -origin.y, 0));
}

#endif
