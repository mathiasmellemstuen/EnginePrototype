#include "model.h"
#include "../utility/debug.h"
#include <tinyobjloader/tiny_obj_loader.h>
#include <rapidxml/rapidxml.hpp>
#include <rapidxml/rapidxml_utils.hpp>
#include <stdexcept>
#include <unordered_map>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>
#include <fstream>
#include <sstream>
#include <iostream>

namespace std {
    template<> struct hash<Vertex> {
        size_t operator()(Vertex const& vertex) const {
            return ((hash<glm::vec3>()(vertex.pos) ^ (hash<glm::vec3>()(vertex.color) << 1)) >> 1) ^ (hash<glm::vec2>()(vertex.texCoord) << 1);
        }
    };
}

Model::Model(const std::string& modelPath) {
    loadOBJ(modelPath);
}

Model::Model(const std::string &modelName, Model::FileType fileType) {
    switch (fileType) {
        case OBJ:
            loadOBJ("model/" + modelName + ".obj");
            break;
        case DAE:
            loadDAE("model/" + modelName + ".dae");

    }
}

void Model::loadOBJ(const std::string& modelPath) {
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warning;
    std::string error;

    if(!tinyobj::LoadObj(&attrib, &shapes, &materials, &warning, &error, modelPath.c_str())) {
        Debug::log(WARNING, warning);
        Debug::log(ERROR, error);
        throw std::runtime_error(warning + error);
    }

    std::unordered_map<Vertex, uint32_t> uniqueVertices{};

    for (const auto& shape : shapes) {
        for(const auto& index : shape.mesh.indices) {
            Vertex vertex{};

            vertex.pos = {
                    attrib.vertices[3 * index.vertex_index + 0],
                    attrib.vertices[3 * index.vertex_index + 1],
                    attrib.vertices[3 * index.vertex_index + 2]
            };

            vertex.texCoord = {
                    attrib.texcoords[2 * index.texcoord_index + 0],
                    1.0f - attrib.texcoords[2 * index.texcoord_index + 1]

            };
            vertex.color = {1.0f, 1.0f, 1.0f};

            if(uniqueVertices.count(vertex) == 0) {
                uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
                vertices.push_back(vertex);
            }

            indices.push_back(uniqueVertices[vertex]);
        }
    }
}

void Model::loadDAE(const std::string &modelPath) {
    std::ifstream f("models/Cube.dae");
    std::string str;
    if (f) {
        std::ostringstream ss;
        ss << f.rdbuf();    // Read data
        str = ss.str();
    }
    char *tab2 = new char[str.length() + 1];
    strcpy(tab2, str.c_str());

    rapidxml::xml_document<> doc;
    doc.parse<0>(tab2);

    rapidxml::xml_node<> *pRoot = doc.first_node();
    if (pRoot == nullptr) {
        std::cout << "Error with root node" << std::endl;
    } else {
        std::cout << "Name: " << pRoot->name() << std::endl;
    }

    rapidxml::xml_node<> *libGeometries = pRoot->first_node("library_geometries");
    if (libGeometries == nullptr) {
        std::cout << "Error with lib" << std::endl;
    } else {
        std::cout << libGeometries->name() << std::endl;
    }

    rapidxml::xml_node<> *mesh = libGeometries->first_node("geometry");
    if (mesh == nullptr) {
        std::cout << "Error with mesh" << std::endl;
    } else {
        std::cout << "Mesh: " << libGeometries->name() << std::endl;
    }


}
