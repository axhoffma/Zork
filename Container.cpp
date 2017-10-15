//
//  Container.cpp
//  Zork
//
//  Created by Austin Hoffmann on 10/15/17.
//  Copyright © 2017 Austin Hoffmann. All rights reserved.
//

#include "Container.hpp"


Container::Container(rapidxml::xml_node<>* containerNode) {
  rapidxml::xml_node<>* containerProperty = containerNode->first_node("name");
  set_name(containerProperty->value());
  
  //Get the description of the container
  containerProperty = containerNode->first_node("description");
  if(containerProperty != nullptr) {
    set_description(containerProperty->value());
  }
  
  //Get the status of the container
  containerProperty = containerNode->first_node("status");
  if(containerProperty != nullptr) {
    set_status(containerProperty->value());
  }
  
  containerProperty = containerNode->first_node("item");
  while(containerProperty) {
    //insert the name of the item to the Items vector
    items.push_back(containerProperty->value());
    containerProperty = containerProperty->next_sibling("item");
  }
  
}

Container::Container() {
  set_name("Inventory");
}

void Container::open_container() {
  std::cout << get_name();
  if(get_name() == "Inventory" && items.size() == 0) {
    std::cout << ": empty" << std::endl;
    return;
  }
  if(items.size() != 0) {
    std::cout << " contains " + items[0];
    for(auto index = 1; index != items.size(); ++index) {
      std::cout << ", " + items[index];
    }
  }
  else {
    std::cout << " is empty";
  }
  std::cout << std::endl;
}

bool Container::find_item(std::string desiredItem) {
  bool found = false;
  for(auto item : items) {
    if(desiredItem == item) {
      found = true;
      return found;
    }
  }
  return found;
}

