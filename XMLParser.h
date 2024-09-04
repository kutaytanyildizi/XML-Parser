#ifndef XMLParser_H
#define XMLParser_H

#include "Pugi/pugixml.hpp"

#include <string>
#include <iostream>

class XMLParserException : public std::exception
{
public:
    XMLParserException(const char* ExType) : m_ExType{ExType} {}

    const char* what() const noexcept override
    {
        return m_ExType;
    }

private:
    const char* m_ExType; 
};
 
class XMLParser
{
public:
    explicit XMLParser(const std::string &fileName);
    void selectNode(const std::string &searchNode);
    void returnParentNode();
    template<typename... Args>
    const std::string get(const Args&... args) const;

private:
    pugi::xml_document doc;
    pugi::xml_node node;
    template<typename T, typename... Args>
    const std::string get(pugi::xml_node& tempNode, const T& arg, const Args&... args) const;
    const std::string get(const pugi::xml_node& tempNode) const;
};

XMLParser::XMLParser(const std::string &fileName)
{
    if(!doc.load_file(fileName.c_str()))
    {
        throw XMLParserException{"File not found!!!"};
    }
}

void XMLParser::selectNode(const std::string &searchNode)
{
    if(node.child(searchNode.c_str()))
    {
        node = node.child(searchNode.c_str());
    }
    else if(doc.child(searchNode.c_str()))
    {
        node = doc.child(searchNode.c_str());
    }
    else
    {
        throw XMLParserException{"Node not found!!!"};
    }
}

void XMLParser::returnParentNode()
{
    if(node.parent())
    {
        node = node.parent();
    }
    else
    {
        throw XMLParserException{"Node doesn't have Parent Node!!!"};
    }
}

template<typename... Args>
const std::string XMLParser::get(const Args&... args) const
{
    pugi::xml_node tempNode = doc;
    return get(tempNode, args...);
}

template<typename T, typename... Args>
const std::string XMLParser::get(pugi::xml_node& tempNode, const T& arg, const Args&... args) const 
{
    if(tempNode.child(arg))
    {
        tempNode = tempNode.child(arg);
    }
    else
    {
        throw XMLParserException{"Node or Child not found!!!"};
    }
    return get(tempNode, args...);
}

const std::string XMLParser::get(const pugi::xml_node& tempNode) const
{
    return tempNode.text().as_string();
}

#endif // XMLParser_H
