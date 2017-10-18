//
//  helper.cpp
//  Zork
//
//  Created by Austin Hoffmann on 10/17/17.
//  Copyright © 2017 Austin Hoffmann. All rights reserved.
//

#include "helper.hpp"
//Add objects to a room or container
void add(Object* object, Room* room) {
  
  //Check if the object is a container
  if(dynamic_cast<Container*>(object) != NULL) {
    room->add_container(object->get_name());
  }
  
  //Check if the object is an Item
  else if(dynamic_cast<Item*>(object) != NULL) {
    room->add_item(object->get_name());
  }
  
  //else if(dynamic_cast<Creature*>(object) != NULL) {
  //room->add_creature(object->get_name());
  //}
}

//Add an item to a container
void add(Item* item, Container* container) {
  container->add_item(item->get_name());
}




//Find an object on the map
Object* find_object(std::string objectName, std::unordered_map<std::string, Object*>& objectMap) {
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
  
  //If the object is an Item or a Creature, do nothing
  
}

//Update the status of an object to a new string
void update(Object* object, std::string newStatus) {
  object->set_status(newStatus);
}

void parse_commands(std::string command, std::unordered_map<std::string, Object*>& objectMap) {
  //Separate the input words for easy analysis
  std::istringstream iss{command};
  std::vector<std::string> tokens{std::istream_iterator<std::string>{iss}, std::istream_iterator<std::string>{}};
  
  //Option 1: Update (object) to (status)
  if(tokens[0] == "Update") {
    auto search = objectMap.find(tokens[1]);
    if(search != objectMap.end()) {
      Object* object = search->second;
      update(object, tokens[3]);
    }
  }
  
  //Option 2: Add (object) to (room/container)
  if(tokens[0] == "Add") {
    auto search = objectMap.find(tokens[1]);
    if(search != objectMap.end()) {
      Object* object = search->second;
      search = objectMap.find(tokens[3]);
      if(search != objectMap.end()) {
        Object* addTo = search->second;
        if(dynamic_cast<Room*>(addTo) != NULL) {
          Room* room = dynamic_cast<Room*>(addTo);
          add(object, room);
        }
        if(dynamic_cast<Container*>(addTo) != NULL) {
          Container* container = dynamic_cast<Container*>(addTo);
          add(dynamic_cast<Item*>(object), container);
        }
      }
    }
  }
  
  //Option 3: Delete (object)
  if(tokens[0] == "Delete") {
    auto search = objectMap.find(tokens[1]);
    if(search != objectMap.end()) {
      Object* object = search->second;
      
      //If it is a room, just remove the room from the map
      if(dynamic_cast<Room*>(object) != NULL) {
        objectMap.erase(object->get_name());
      }
      
      //Search every Object and remove references
      for(auto mapElement : objectMap) {
        Object* mapObject = mapElement.second;
        delete_object(tokens[1], mapObject);
      }
    }
  }
  
}
