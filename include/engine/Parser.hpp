#ifndef PARSER_HPP
#define PARSER_HPP

#include "Scene.hpp"
#include <string>
#include "Group.hpp"

class Parser {
public:
 static Scene parse(const std::string& xmlPath); // assim devolve diretamente uma scene, sem precisar de criar um objeto Parser
};

#endif