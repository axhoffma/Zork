//
//  Condition.cpp
//  Zork
//
//  Created by Austin Hoffmann on 10/17/17.
//  Copyright © 2017 Austin Hoffmann. All rights reserved.
//

#include "Condition.hpp"
#include "Object.h"
#include "Container.hpp"

bool Condition::check_has() {
  if(conditionHas == "") {
    return false;
  }
  return true;
}

Condition::Condition(rapidxml::xml_node<>* node) {
  rapidxml::xml_node<>* conditionNode = node->first_node("object");
  conditionObject = conditionNode->value();
  conditionNode = node->first_node("has");
  
  //is a HAS condition
  if(conditionNode) {
    conditionHas = conditionNode->value();
    conditionNode = node->first_node("owner");
    conditionOwner = conditionNode->value();
  }
  else {
    conditionNode = node->first_node("status");
    conditionStatus = conditionNode->value();
  }
}

bool Condition::check_condition(GameInformation& gameInfo) {
  bool good_condition = false;
  bool isHas = check_has();
  if(isHas) {
    Object* owner;
    if(conditionOwner == "inventory") {
      owner = dynamic_cast<Object*>(gameInfo.inventory);
    }
    else {
      owner = gameInfo.objectMap[conditionOwner];
    }
    if(conditionHas == "yes") {
      bool found = owner->find_object(conditionObject);
      if(found) {
        good_condition = true;
      }
    }
    else {
      bool found = owner->find_object(conditionObject);
      if(!found) {
        good_condition = true;
      }
    }
  }
  else {
    std::string objectName = conditionObject;
    auto object = gameInfo.objectMap[objectName];
    if(object->get_status() == conditionStatus) {
      good_condition = true;
    }
  }
  return good_condition;
  //MITCH DANIELS KILLED THE HORSE
}
