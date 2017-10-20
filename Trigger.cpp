//
//  Trigger.cpp
//  Zork
//
//  Created by Austin Hoffmann on 10/17/17.
//  Copyright © 2017 Austin Hoffmann. All rights reserved.
//

#include "Trigger.hpp"


Trigger::Trigger(rapidxml::xml_node<>* triggerNode) {
  rapidxml::xml_node<>* triggerProperty = triggerNode->first_node("type");
  if(triggerProperty) {
    type = triggerProperty->value();
  }
  
  
  //Get the conditions
  triggerProperty = triggerNode->first_node("command");
  if(triggerProperty) {
    command = triggerProperty->value();
  }
  triggerProperty = triggerNode->first_node("print");
  while(triggerProperty) {
    prints.push_back(triggerProperty->value());
    triggerProperty = triggerProperty->next_sibling("print");
  }
  triggerProperty = triggerNode->first_node("action");
  while(triggerProperty) {
    actions.push_back(triggerProperty->value());
    triggerProperty = triggerProperty->next_sibling("action");
  }
  triggerProperty = triggerNode->first_node("condition");
  while(triggerProperty) {
    conditions.push_back(Condition(triggerProperty));
    triggerProperty = triggerProperty->next_sibling("condition");
  }
  
}

bool Trigger::trigger_check(std::string command, GameInformation& gameInfo, bool& fired) {
  bool valid_trigger = true;
  bool delete_trigger = false;
  
  for(auto condition : conditions) {
    if(!condition.check_condition(gameInfo)) {
      valid_trigger = false;
    }
  }
  if(command != this->command && this->command != "") {
    valid_trigger = false;
  }
  
  if(valid_trigger) {
    fired = true;
    for(auto print: prints) {
      std::cout << print << std::endl;
    }
    for(auto action : actions) {
      parse_commands(action, gameInfo);
    }
    if(type == "single") {
      delete_trigger = true;
    }
  }
  return delete_trigger;
}
