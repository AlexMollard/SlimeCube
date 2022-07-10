#pragma once

#include "glm/gtx/matrix_decompose.hpp"

namespace SlimeMath {
bool DecomposeTransform(const glm::mat4 &transform, glm::vec3 &translation, glm::vec3 &rotation, glm::vec3 &scale);
}