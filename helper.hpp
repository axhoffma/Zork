//
//  helper.hpp
//  Zork
//
//  Created by Austin Hoffmann on 10/15/17.
//  Copyright © 2017 Austin Hoffmann. All rights reserved.
//

#ifndef helper_hpp
#define helper_hpp

#include "Object.h"
#include "Container.hpp"
#include "Room.h"
#include <unordered_map>


//Add objects to a room or container
void add(Object* object, Room& room) {
  
  //Check if the object is a container
  if(dynamic_cast<Container*>(object) != NULL) {
    room.add_container(object->get_name());
  }
  
  //Check if the object is an Item
  else if(dynamic_cast<Item*>(object) != NULL) {
    room.add_item(object->get_name());
  }
  //TODO add support for creatures
}

//Add an item to a container
void add(Item& item, Container& container) {
  container.add_item(item.get_name());
}


//TODO do delete


//Find an object on the map
Object* find_object(std::string objectName, std::unordered_map<std::string, Object*> objectMap) {
  auto search = objectMap.find(objectName);
  if(search != objectMap.end()) {
    return search->second;
  }
  else {
    return nullptr;
  }
}

//Delete an object from a Room or Container
void delete_object(std::string objectToDelete, Object* objectToSearch) {
  
  //Check if the searched object is a container
  if(dynamic_cast<Container*>(objectToSearch) != NULL) {
    Container* container = dynamic_cast<Container*>(objectToSearch);
    bool found = container->find_item(objectToDelete);
    if(found) {
      container->remove_item(objectToDelete);
    }
  }
  
  //Check if the searched object is a Room
  if(dynamic_cast<Room*>(objectToSearch) != nullptr) {
    Room* room = dynamic_cast<Room*>(objectToSearch);
    bool found = room->find_container(objectToDelete);
    if(found) {
      room->remove_container(objectToDelete);
    }
    found = room->find_item(objectToDelete);
    if(found) {
      room->remove_item(objectToDelete);
    }
  }
  
}

//Update the status of an object to a new string
void update(Object* object, std::string newStatus) {
  object->set_status(newStatus);
}

#endif /* helper_hpp */
