//
// Created by Tobias Hallingstad on 24/02/2022.
//

#ifndef ENGINEPROTOTYPE_XML_H
#define ENGINEPROTOTYPE_XML_H

#include <rapidxml/rapidxml.hpp>
#include <rapidxml/rapidxml_iterators.hpp>

class XMLNode {
public:
    rapidxml::xml_node<> *node;

    XMLNode() : node() {

    };

    explicit XMLNode(rapidxml::xml_node<> *n) : node(n) {
        // TODO: Add error handling
    };

    [[nodiscard]] rapidxml::node_iterator<char> begin() const {
        return node;
    }

    [[nodiscard]] rapidxml::node_iterator<char> end() const {
        return {};
    }

    void operator = (rapidxml::xml_node<> newNode) {
        node = &newNode;
    }
};

class XMLAttribute {
public:
    rapidxml::xml_attribute<> *attribute;

    explicit XMLAttribute(rapidxml::xml_attribute<> *a) : attribute(a) {};

    [[nodiscard]] rapidxml::attribute_iterator<char> begin() const {
        return reinterpret_cast<const rapidxml::attribute_iterator<char> &>(attribute);
    }

    [[nodiscard]] rapidxml::attribute_iterator<char> end() const {
        return {};
    }
};


#endif //ENGINEPROTOTYPE_XML_H
