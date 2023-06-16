#pragma once
#include "../../own-math-3d/math_3d/vec3.hpp"
#include "../../own-math-3d/math_3d/vec2.hpp"
#include <vector>

namespace util {
    class OBJParser final {
    public:
        static bool Parse(const char* filepath, 
            std::vector<math::vec3f>& positions, 
            std::vector<math::vec3f>& normals,
            std::vector<math::vec2f>& texture_coords, 
            std::vector<size_t>& vert_indexes,
            std::vector<size_t>& texture_indexes,
            std::vector<size_t>& normal_indexes
        ) noexcept;
    };
}