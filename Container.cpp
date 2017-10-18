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
  
  //Get the list of accepts
  containerProperty = containerNode->first_node("accept");
  while(containerProperty) {
    accepts.push_back(containerProperty->value());
    containerProperty = containerProperty->next_sibling("accept");
  }
  
  
  //Add list of items to the container
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

//Check if an item can be placed in the container
bool Container::check_accept(std::string item) {
  
  //If there are no accepts, all items are allowed
  if(accepts.size() == 0) {
    return true;
  }
  for(auto accepted_item : accepts) {
    if(accepted_item == item) {
      return true;
    }
  }
  return false;
}

//Check if the container can be opened
bool Container::check_open() {
  if(accepts.size() == 0) {
    return true;
  }
  for(auto item : items) {
    //Condition to check is if an accept is in the inventory
    for(auto accepted_item : accepts) {
      if(accepted_item == item) {
        return true;
      }
    }
  }
  return false;
}

