//
// Created by Tobias Hallingstad on 17/02/2022.
//

#include "face.h"
#include <iostream>

Face::Face() = default;

Face::Face(int v1, int v2, int v3) {

}

Face::Face(glm::vec3 v, glm::vec3 n, glm::vec3 t) {

}

void Face::print() {
    std::cout << "V: " << v.x << " " << v.y << " " << v.z << std::endl;
    std::cout << "N: " << n.x << " " << n.y << " " << n.z << std::endl;
    std::cout << "T: " << t.x << " " << t.y << " " << t.z << std::endl;
}

