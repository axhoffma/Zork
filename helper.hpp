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
#include <unordered_map>

class Container;
class Creature;
class Item;
class Room;

//Add objects to a room or container
void add(Object* object, Room* room);

//Add an item to a container
void add(Item* item, Container* container);




//Find an object on the map
Object* find_object(std::string, std::unordered_map<std::string, Object*>&);

//Delete an object from a Room or Container
void delete_object(std::string, Object*);

//Update the status of an object to a new string
void update(Object*, std::string);

void parse_commands(std::string, std::unordered_map<std::string, Object*>&);

#endif /* helper_hpp */
