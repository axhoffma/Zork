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

struct GameInformation;
class Object;

struct Condition {
  std::string conditionHas;
  std::string conditionOwner;
  std::string conditionObject;
  std::string conditionStatus;
  Condition(rapidxml::xml_node<>*);
  bool check_has();
  bool check_condition(GameInformation&);
};

#endif /* Condition_hpp */
