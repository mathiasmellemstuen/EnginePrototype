//
// Created by Tobias Hallingstad on 17/02/2022.
//

#ifndef ENGINEPROTOTYPE_FACE_H
#define ENGINEPROTOTYPE_FACE_H

#include <glm/vec3.hpp>

class Face {
public:
    glm::vec3 v{}, n{}, t{};

    Face();
    Face(int v1, int v2, int v3);
    Face(glm::vec3 v, glm::vec3 n, glm::vec3 t);

    void print();
};


#endif //ENGINEPROTOTYPE_FACE_H
