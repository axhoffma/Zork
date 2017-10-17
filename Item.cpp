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
  
}

void Item::read_writing() {
  std::cout << writing << std::endl;
}
