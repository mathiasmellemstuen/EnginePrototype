
#include <iterator>
#include "model.h"
#include "../utility/logging.h"
#include <tinyobjloader/tiny_obj_loader.h>

#include <rapidxml/rapidxml.hpp>
#include "../utility/xml.h"

#include <stdexcept>
#include <unordered_map>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>
#include <fstream>
#include <sstream>
#include <iostream>
#include <functional>
#include <string.h>

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
            loadDAE(modelName);
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
        logger(WARNING, warning.c_str());
        logger(ERROR, error.c_str());
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

void Model::loadDAE(const std::string &modelName) {
    // Read the file
    std::ifstream f("models/" + modelName + ".dae");
    std::string str;
    if (f) {
        std::ostringstream ss;
        ss << f.rdbuf();    // Read data
        str = ss.str();
    }
    char *tab2 = new char[str.length() + 1];
    strcpy(tab2, str.c_str());

    // Parse the file to xml
    rapidxml::xml_document<> doc;
    doc.parse<rapidxml::parse_declaration_node | rapidxml::parse_no_data_nodes>(tab2);

    // Iterate over all child's to the root node
    XMLNode rootNode(doc.first_node("COLLADA"));
    for (const auto& node : rootNode) {

        if (strcmp(node.name(), "library_geometries") == 0)
            parseGeometries(node);
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

void Model::parseGeometries(const rapidxml::xml_node<>& node) {
    rapidxml::xml_node<> * geo = node.first_node("geometry");
    std::string name = geo->first_attribute("name")->value();
    XMLNode nodes(geo->first_node("mesh"));

    for (const auto& source : nodes) {
        // Parse mesh position
        if (strcmp(source.name(), "source") == 0 &&
            source.first_attribute("id")->value() == (name + "-mesh-positions")) {
            std::vector<std::string> vert = splitString(source.first_node("float_array")->value());

            for (int i = 0; i < vert.size(); i += 3) {
                Vertex vertex{};
                vertex.pos = {
                        std::stof(vert[i]),
                        std::stof(vert[i + 1]),
                        std::stof(vert[i + 2])
                };
                vertices.push_back(vertex);
            }
        }

        // Parse mesh normals
        if (strcmp(source.name(), "source") == 0 && source.first_attribute("id")->value() == (name + "-mesh-normals")) {
            std::vector<std::string> normals = splitString(node.value());
        }

        // Parse mesh map 0
        if (strcmp(source.name(), "source") == 0 && source.first_attribute("id")->value() == (name + "-mesh-map-0")) {
            std::vector<std::string> vert = splitString(node.value());
        }

        if (strcmp(source.name(), "triangles") == 0) {
            // Parse offsets for indices
            rapidxml::xml_node<> *input = source.first_node("input");
            int vertOffset = std::stoi(input->first_attribute("offset")->value());

            input = input->next_sibling();
            int normalOffset = std::stoi(input->first_attribute("offset")->value());

            input = input->next_sibling();
            int texOffset = std::stoi(input->first_attribute("offset")->value());

            // Append indices
            std::vector<std::string> pStr = splitString(source.first_node("p")->value());

            for (int i = 0, l = 0; i < pStr.size(); i += 9, l++) {
                indices.push_back((uint32_t) std::stoi(pStr[i + vertOffset]));
                indices.push_back((uint32_t) std::stoi(pStr[i + vertOffset + 3]));
                indices.push_back((uint32_t) std::stoi(pStr[i + vertOffset + 6]));

                // NOTE: Use of Vec3 for texCoord?
                vertices[i].texCoord = {
                        std::stoi(pStr[l + texOffset]),
                        std::stoi(pStr[l + texOffset + 3])
                };
            }
        }
    }
}
