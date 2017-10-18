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
class Condition;
class Creature : public Object {
  std::vector<std::string> vulnerabilities;
  std::vector<std::string> actions;
  std::vector<std::string> prints;
  std::vector<Condition> conditions;
  std::vector<Trigger> triggers;
  
public:
  Creature(rapidxml::xml_node<>*);
  bool check_attack(std::string item);
  bool execute_attack(std::unordered_map<std::string, Object*>);
  inline bool find_object(std::string object) {
    return false;
  };
  void find_triggers(std::string input, std::unordered_map<std::string, Object*>& objectMap, bool&);
};

#endif /* Creature_hpp */
