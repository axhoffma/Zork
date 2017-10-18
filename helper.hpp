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
#include "Creature.hpp"
#include <unordered_map>


//Add objects to a room or container
void add(Object* object, Room* room);

//Add an item to a container
void add(Item* item, Container* container);




//Find an object on the map
Object* find_object(std::string objectName, std::unordered_map<std::string, Object*>& objectMap);

//Delete an object from a Room or Container
void delete_object(std::string objectToDelete, Object* objectToSearch);

//Update the status of an object to a new string
void update(Object* object, std::string newStatus);

void parse_commands(std::string command, std::unordered_map<std::string, Object*>& objectMap);

#endif /* helper_hpp */
