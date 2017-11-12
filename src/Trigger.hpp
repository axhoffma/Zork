//
//  Trigger.hpp
//  Zork
//
//  Created by Austin Hoffmann on 10/17/17.
//  Copyright © 2017 Austin Hoffmann. All rights reserved.
//

#ifndef Trigger_hpp
#define Trigger_hpp

#include "rapidXML/rapidxml.hpp"
#include "Condition.hpp"
#include <vector>
#include <string>
#include <unordered_map>
#include <iostream>
#include "helper.hpp"

struct GameInformation;
class Trigger {
  std::vector<std::string> actions;
  std::vector<std::string> prints;
  std::vector<Condition> conditions;
  std::string command;
  std::string type = "single";
  
public:
  Trigger(rapidxml::xml_node<>*);
  bool trigger_check(std::string command, GameInformation&, bool&);
};


#endif /* Trigger_hpp */
