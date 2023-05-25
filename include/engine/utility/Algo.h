#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>

namespace s2de::algo
{
    static void pointsInCircle(std::vector<sf::Vector2i>& points, const sf::Vector2i& position, int32_t radius)
    {
        points.reserve(radius * radius);

        for (int32_t y = -radius; y <= radius; y++)
            for (int32_t x = -radius; x <= radius; x++)
                if (sqrt(x*x + y*y) <= radius)
                    points.push_back(sf::Vector2i(x, y) + position);

        points.shrink_to_fit();
    }

    typedef std::vector<std::vector<float>> Matrix;
    static void inverse2x2(Matrix& new_matrix, const Matrix& matrix)
    {
        assert(matrix.size() == 2 && matrix[0].size() == 2);

        const float determinant = matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];
        const float rec_det     = 1.f / determinant;

        new_matrix.resize(2);
        new_matrix[0] = { rec_det * matrix[1][1], -1.f * rec_det * matrix[0][1] };
        new_matrix[1] = { -1.f * rec_det * matrix[1][0], rec_det * matrix[0][0] };
    }
}