#ifndef ENGINEPROTOTYPE_MODEL
#define ENGINEPROTOTYPE_MODEL


#include <vector>
#include "vertex.h"
#include <string>

class Model {
    public:
        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;
        Model(const std::string& modelPath);
};

#endif