#include "model.h"
#include "../utility/debug.h"
#include <tinyobjloader/tiny_obj_loader.h>
#include <rapidxml/rapidxml.hpp>
#include <rapidxml/rapidxml_utils.hpp>
#include <stdexcept>
#include <unordered_map>
#include "face.h"

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
    rapidxml::xml_node<> *rootNode = nullptr;



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

        // std::cout << "node atter name: " << sourceNode->first_attribute()->value() << std::endl;
        // std::cout << "sourceNode type: " << sourceNode->first_node()->value() << std::endl;

        std::string atVal = currentAtter->value();

        if (atVal == "Cube-mesh-map-0") {
            std::string map = currentNode->value();

            std::cout << map << std::endl;

            // Split the value string into a vector string
            std::stringstream ss(map);
            std::istream_iterator<std::string> begin(ss);
            std::istream_iterator<std::string> end;
            std::vector<std::string> vert(begin, end);


        }

    }

    // Loading faces from triangles in the xml
    rapidxml::xml_node<> *triangles = workingNode->first_node("triangles");
    if (triangles == nullptr)
        std::cout << "triangles is null" << std::endl;
    else
        std::cout << "triangles: " << triangles->value() << std::endl;

    rapidxml::xml_node<> *p = triangles->first_node("p");
    if (p == nullptr)
        std::cout << "p is null" << std::endl;
    else
        std::cout << "p: " << p->value() << std::endl;

    std::stringstream ss(p->value());
    std::istream_iterator<std::string> begin(ss);
    std::istream_iterator<std::string> end;
    std::vector<std::string> pStr(begin, end);

    rapidxml::xml_attribute<> *countAt = triangles->first_attribute("count");
    // std::vector<Face> faces(std::stoi(countAt->value()));
    std::vector<Face> faces;

    // Add all lines to faces
    for (int i = 0; i < pStr.size() - 9; i += 9) {
        Face face;
        face.v = glm::vec3(std::stoi(pStr[i]), std::stoi(pStr[3+i]), std::stoi(pStr[6+i]));
        face.n = glm::vec3(std::stoi(pStr[i+1]), std::stoi(pStr[3+i+1]), std::stoi(pStr[6+i+1]));
        face.t = glm::vec3(std::stoi(pStr[i+2]), std::stoi(pStr[3+i+2]), std::stoi(pStr[6+i+2]));
        // face.print();
    }

    // Parse all faces to vertex
    for (const auto& face : faces) {
        Vertex vertex{};

        vertex.pos = {

        };

        vertex.texCoord = {

        };
    }

}
