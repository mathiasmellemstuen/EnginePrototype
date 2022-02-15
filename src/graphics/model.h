#ifndef ENGINEPROTOTYPE_MODEL
#define ENGINEPROTOTYPE_MODEL


#include <vector>
#include "vertex.h"
#include <string>

class Model {
    public:
        enum FileType {OBJ, DAE};
        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;
        explicit Model(const std::string& modelPath);

        Model(const std::string& modelName, FileType fileType);

    private:
        void loadDAE(const std::string& modelPath);
        void loadOBJ(const std::string& modelPath);

};

#endif