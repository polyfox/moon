#ifndef MOON_VECTOR1_H
#define MOON_VECTOR1_H

#include <memory>
#include <glm/glm.hpp>

typedef std::shared_ptr<glm::highp_vec1_t> moon_vec1;
#define moon_vec1_ptr(vec) (*vec)
#define moon_vec1_ref(vec) (**vec)
#endif
