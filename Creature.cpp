//
//  Creature.cpp
//  Zork
//
//  Created by Austin Hoffmann on 10/17/17.
//  Copyright © 2017 Austin Hoffmann. All rights reserved.
//

#include "Creature.hpp"


Creature::Creature(rapidxml::xml_node<>* creatureNode) {
  rapidxml::xml_node<>* creatureProperty = creatureNode->first_node("name");
  set_name(creatureProperty->value());
  
  //Get the description of the creature
  creatureProperty = creatureNode->first_node("description");
  if(creatureProperty != nullptr) {
    set_description(creatureProperty->value());
  }
  
  //Get the status of the creature
  creatureProperty = creatureNode->first_node("status");
  if(creatureProperty != nullptr) {
    set_status(creatureProperty->value());
  }
  
  //Get the list of vulnerabilitiees
  creatureProperty = creatureNode->first_node("vulnerability");
  while(creatureProperty) {
    vulnerabilities.push_back(creatureProperty->value());
    creatureProperty = creatureProperty->next_sibling("vulnerability");
  }
  
  //Get the conditions
  rapidxml::xml_node<>* attackNode = creatureNode->first_node("attack");
  creatureProperty = attackNode->first_node("print");
  while(creatureProperty) {
    prints.push_back(creatureProperty->value());
    creatureProperty = creatureProperty->next_sibling("print");
  }
  creatureProperty = attackNode->first_node("action");
  while(creatureProperty) {
    actions.push_back(creatureProperty->value());
    creatureProperty = creatureProperty->next_sibling("action");
  }
  creatureProperty = attackNode->first_node("condition");
  while(creatureProperty) {
    conditions.push_back(Condition(creatureProperty));
    creatureProperty = creatureProperty->next_sibling("condition");
  }
  
}

bool Creature::check_attack(std::string item) {
  for(auto weakness : vulnerabilities) {
    if(item == weakness) {
      return true;
    }
  }
  return false;
}

bool Creature::execute_attack(std::unordered_map<std::string, Object*> objectMap) {
  int valid_attack = false;
  for(auto condition: conditions) {
    bool isHas = condition.check_has();
    if(isHas) {
      auto owner = objectMap[condition.conditionOwner];
      if(condition.conditionHas == "yes") {
        bool found = owner->find_object(condition.conditionObject);
        if(found) {
          valid_attack = true;
        }
      }
      else {
        bool found = owner->find_object(condition.conditionObject);
        if(!found) {
          valid_attack = true;
        }
      }
    }
    else {
      std::string objectName = condition.conditionObject;
      auto object = objectMap[objectName];
      if(object->get_status() == condition.conditionStatus) {
        valid_attack = true;
      }
    }
  }
  if(valid_attack) {
    for(auto print: prints) {
      std::cout << print << std::endl;
    }
    for(auto action : actions) {
      parse_commands(action, objectMap);
    }
  }
  return valid_attack;
}

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
