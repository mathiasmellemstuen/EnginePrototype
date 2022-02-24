#include "model.h"
#include "../utility/debug.h"
#include <tinyobjloader/tiny_obj_loader.h>
#include <rapidxml/rapidxml.hpp>
#include <rapidxml/rapidxml_utils.hpp>
#include <stdexcept>
#include <unordered_map>
#include "Face.h"

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
            loadOBJ("models/" + modelName + ".obj");
            break;
        case DAE:
            loadDAE(modelName, "Cube");
            break;
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

void Model::loadDAE(const std::string &modelName, const std::string &kake) {
    std::ifstream f("models/" + modelName + ".dae");
    std::string str;
    if (f) {
        std::ostringstream ss;
        ss << f.rdbuf();    // Read data
        str = ss.str();
    }
    char *tab2 = new char[str.length() + 1];
    strcpy(tab2, str.c_str());

    rapidxml::xml_document<> doc;
    rapidxml::xml_node<> *rootNode;

    // Parse the file
    doc.parse<rapidxml::parse_declaration_node | rapidxml::parse_no_data_nodes>(tab2);

    rootNode = doc.first_node("COLLADA");
    if (rootNode == nullptr)
        std::cout << "rootNode is null" << std::endl;

    // Get correct node to read the data
    rapidxml::xml_node<> *workingNode = rootNode->first_node("library_geometries")->first_node("geometry")->first_node("mesh");
    if (workingNode == nullptr)
        std::cout << "workingNode is null" << std::endl;
    else
        std::cout << "Node val: " << workingNode->name() << std::endl;

    // Iterate over source
    for (rapidxml::xml_node<> *sourceNode = workingNode->first_node("source"); sourceNode; sourceNode = sourceNode->next_sibling()) {
        rapidxml::xml_attribute<> *currentAtter = sourceNode->first_attribute();
        rapidxml::xml_node<> *currentNode = sourceNode->first_node();

        std::string atVal = currentAtter->value();

        // Parsing coords for the triangle corners
        if (atVal == kake + "-mesh-positions") {
            // Split the value string into a vector string
            std::vector<std::string> vert = splitString(currentNode->value());

            for (int i = 0; i < vert.size(); i += 3) {
                Vertex vertex{};
                vertex.pos = {
                    std::stof(vert[i]),
                    std::stof(vert[i+1]),
                    std::stof(vert[i+2])
                };
                vertices.push_back(vertex);
            }
        }

        // Some position thing
        if (atVal == kake + "-mesh-map-0") {
            std::vector<std::string> vert = splitString(currentNode->value());
        }

        if (atVal == kake + "-mesh-normals") {
            std::vector<std::string> vert = splitString(currentNode->value());
        }
    }

    // Loading faces from triangles in the xml
    rapidxml::xml_node<> *p = workingNode->first_node("triangles")->first_node("p");
    // rapidxml::xml_attribute<> *countAt = workingNode->first_node("triangles")->first_attribute("count");
    if (p == nullptr)
        std::cout << "p is null" << std::endl;
    else
        std::cout << "p: " << p->value() << std::endl;

    std::vector<std::string> pStr = splitString(p->value());

    for (int i = 0, l = 0; i < pStr.size(); i += 9, l++) {
        indices.push_back((uint32_t)std::stoi(pStr[i]));
        indices.push_back((uint32_t)std::stoi(pStr[i+3]));
        indices.push_back((uint32_t)std::stoi(pStr[i+6]));

        // NOTE: Use of Vec3 for texCoord?
        vertices[i].texCoord = {
                std::stoi(pStr[l+2]),
                std::stoi(pStr[l+2+3])
        };
    }
}

// Split the string on space into a vector of strings
std::vector<std::string> Model::splitString(const std::string &s) {
    std::stringstream ss(s);

    return {
        std::istream_iterator<std::string> (ss),
        std::istream_iterator<std::string> ()
    };
}

