//
//  Creature.cpp
//  Zork
//
//  Created by Austin Hoffmann on 10/17/17.
//  Copyright © 2017 Austin Hoffmann. All rights reserved.
//

#include "Creature.hpp"
#include "Condition.hpp"


Creature::Creature(rapidxml::xml_node<>* creatureNode) : Object(creatureNode) {
  
  //Get the list of vulnerabilitiees
  rapidxml::xml_node<>* creatureProperty = creatureNode->first_node("vulnerability");
  while(creatureProperty) {
    vulnerabilities.push_back(creatureProperty->value());
    creatureProperty = creatureProperty->next_sibling("vulnerability");
  }
  //Get the list of triggers
  creatureProperty = creatureNode->first_node("trigger");
  while(creatureProperty) {
    triggers.push_back(Trigger(creatureProperty));
    creatureProperty = creatureProperty->next_sibling("trigger");
  }
  
  //Get the conditions
  rapidxml::xml_node<>* attackNode = creatureNode->first_node("attack");
  if(attackNode) {
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
  if(conditions.size() == 0) {
    valid_attack = true;
  }
  for(auto condition: conditions) {
    valid_attack = condition.check_condition(objectMap);
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

void Creature::find_triggers(std::string input, std::unordered_map<std::string, Object*>& objectMap, bool& fired) {
  for(auto trigger = std::begin(triggers); trigger < std::end(triggers); ++trigger) {
    bool needsDeletion = trigger->trigger_check(input, objectMap, fired);
    if(needsDeletion) {
      triggers.erase(trigger);
    }
  }
}
