//
//  Creature.hpp
//  Zork
//
//  Created by Austin Hoffmann on 10/17/17.
//  Copyright © 2017 Austin Hoffmann. All rights reserved.
//

#ifndef Creature_hpp
#define Creature_hpp

#include "Object.h"
#include "helper.hpp"
#include <unordered_map>


struct Condition {
  std::string conditionHas;
  std::string conditionOwner;
  std::string conditionObject;
  std::string conditionStatus;
  Condition(rapidxml::xml_node<>*);
  bool check_has();
};

class Creature : public Object {
  std::vector<std::string> vulnerabilities;
  std::vector<std::string> actions;
  std::vector<std::string> prints;
  std::vector<Condition> conditions;
  
public:
  Creature(rapidxml::xml_node<>*);
  bool check_attack(std::string item);
  bool execute_attack(std::unordered_map<std::string, Object*>);
  inline bool find_object(std::string object) {
    return false;
  }
};

#endif /* Creature_hpp */
