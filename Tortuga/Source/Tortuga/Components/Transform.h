#ifndef _TRANSFORM
#define _TRANSFORM

#include "../Core.h"

namespace Tortuga
{
    struct Transform
    {
        glm::vec3 Position = {0, 0, 0};
        glm::vec4 Rotation = {0, 0, 0, 0};
        glm::vec3 Scale = {1, 1, 1};

        glm::mat4 GetMatrix()
        {
            auto matrix = glm::mat4(1);
            matrix = glm::scale(matrix, this->Scale);
            matrix = glm::rotate(matrix, this->Rotation.w, {
                this->Rotation.x,
                this->Rotation.y,
                this->Rotation.z
            });
            matrix = glm::translate(matrix, this->Position);
            return matrix;
        }
    };
};

#endif