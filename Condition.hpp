//
//  Condition.hpp
//  Zork
//
//  Created by Austin Hoffmann on 10/17/17.
//  Copyright © 2017 Austin Hoffmann. All rights reserved.
//

#ifndef Condition_hpp
#define Condition_hpp

#include <string>
#include "./rapidXML/rapidxml.hpp"
#include <unordered_map>

class Object;

struct Condition {
  std::string conditionHas;
  std::string conditionOwner;
  std::string conditionObject;
  std::string conditionStatus;
  Condition(rapidxml::xml_node<>*);
  bool check_has();
  bool check_condition(std::unordered_map<std::string, Object*>&);
};

#endif /* Condition_hpp */
