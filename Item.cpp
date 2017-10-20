//
//  Item.cpp
//  Zork
//
//  Created by Austin Hoffmann on 10/15/17.
//  Copyright © 2017 Austin Hoffmann. All rights reserved.
//

#include "Item.hpp"


Item::Item(rapidxml::xml_node<>* itemNode) {
  rapidxml::xml_node<>* itemProperty = itemNode->first_node("name");
  set_name(itemProperty->value());
  
  //Add writing to the item
  itemProperty = itemNode->first_node("writing");
  if(itemProperty != nullptr) {
    writing = itemProperty->value();
  }
  
  //Add turnon elements to the item
  itemProperty = itemNode->first_node("turnon");
  if(itemProperty) {
    //check for print actions
    rapidxml::xml_node<>* turnElement = itemProperty->first_node("print");
    if(turnElement) {
      activationCommands.prints.push_back(turnElement->value());
      turnElement = turnElement->next_sibling("print");
    }
    //check for behind the scenes actions
    turnElement = itemProperty->first_node("action");
    if(turnElement) {
      activationCommands.actions.push_back(turnElement->value());
      turnElement = turnElement->next_sibling("action");
    }
  }
  
  itemProperty = itemNode->first_node("trigger");
  while(itemProperty) {
    triggers.push_back(Trigger(itemProperty));
    itemProperty = itemProperty->next_sibling("trigger");
  }
  
}

void Item::read_writing() {
  std::cout << writing << std::endl;
}

std::vector<std::string> Item::turn_on() {
  for(auto print : activationCommands.prints) {
    std::cout << print << std::endl;
  }
  return activationCommands.actions;
}

void Item::find_triggers(std::string input, GameInformation& gameInfo, bool& fired) {
  for(auto trigger = std::begin(triggers); trigger < std::end(triggers); ++trigger) {
    bool needsDeletion = trigger->trigger_check(input, gameInfo, fired);
    if(needsDeletion) {
      triggers.erase(trigger);
    }
  }
}
