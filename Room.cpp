/*
 * Room.cpp
 *
 *  Created on: Oct 13, 2017
 *      Author: austinhoffmann
 */


#include "Room.h"

int direction_to_array(std::string direction) {
	int arrayIndex = -1;
  if(direction == "north" || direction == "n") {
		arrayIndex = 0;
	}
	if(direction == "east" || direction == "e") {
		arrayIndex = 1;
	}
	if(direction == "south"|| direction == "s") {
		arrayIndex = 2;
	}
	if(direction == "west" || direction == "w") {
		arrayIndex = 3;
	}

	return arrayIndex;
}


Room::Room(rapidxml::xml_node<>* roomNode) {
	rapidxml::xml_node<>* roomProperty = roomNode->first_node("name");
	set_name(roomProperty->value());
  
  //Get the description of the room
  roomProperty = roomNode->first_node("description");
  set_description(roomProperty->value());
  
  //Get the status of the room
  roomProperty = roomNode->first_node("status");
  if(roomProperty != nullptr) {
    set_status(roomProperty->value());
  }
  
  //Get the type of the room
  roomProperty = roomNode->first_node("type");
  if(roomProperty != nullptr) {
    type = roomProperty->value();
  }
  
  
	roomProperty = roomNode->first_node("border");
  
	//Keep adding borders while there is another valid one
	while(roomProperty) {
    //Find the direction node
		rapidxml::xml_node<>* borderNode = roomProperty->first_node("direction");
		//get the index location for border from the cardinal direction
		int borderIndex = direction_to_array(borderNode->value());
		//get the name of the border location, and store it in the index
		borderNode = roomProperty->first_node("name");
		border[borderIndex] = std::string(borderNode->value());
    //advance to the next border node
		roomProperty = roomProperty->next_sibling("border");
	}
  
  //Add the list of containers that are in the room
  roomProperty = roomNode->first_node("container");
  while(roomProperty) {
    containers.push_back(roomProperty->value());
    roomProperty = roomProperty->next_sibling("container");
  }
  
  //Add the list of items that are in the room
  roomProperty = roomNode->first_node("item");
  while(roomProperty) {
    items.push_back(roomProperty->value());
    roomProperty = roomProperty->next_sibling("item");
  }
  
  //Add the list of creatures that are in the room
  roomProperty = roomNode->first_node("creature");
  while(roomProperty) {
    creatures.push_back(roomProperty->value());
    roomProperty = roomProperty->next_sibling("creature");
  }
  //Add the list of triggers that are in the room
  roomProperty = roomNode->first_node("trigger");
  while(roomProperty) {
    triggers.push_back(Trigger(roomProperty));
    roomProperty = roomProperty->next_sibling("trigger");
  }
}

Room* Room::movement(std::string direction, const std::unordered_map<std::string, Object*>& roomMap) {
  int index = direction_to_array(direction);
  std::string roomName = border[index];
  if(roomName == "") {
    std::cout << "Can't go that way" <<std::endl;
    return this;
  }
  
  auto search = roomMap.find(roomName);
  
  //Must take into account if the room was removed
  if(search == roomMap.end()) {
    std::cout << "Can't go that way" <<std::endl;
    return this;
  }
  
  Room* newRoom = dynamic_cast<Room*>(search->second);
  std::cout << newRoom->get_description() << std::endl;
  return newRoom;  
}

bool Room::exit_check() {
  bool exit = false;
  if(type == "exit") {
    std::cout << "Game Over" << std::endl;
    exit = true;
  }
  return exit;
}

bool Room::find_container(std::string container) {
  bool found = false;
  if(std::find(containers.begin(), containers.end(), container) != containers.end()) {
    found = true;
  }
  return found;
}

//For searching both the immediate items, as well as items in containers
bool Room::find_item(std::string item, const std::unordered_map<std::string, Object*>& containerMap) {
  bool found = false;
  
  //Check if the item is directly in the room
  if(std::find(std::begin(items), std::end(items), item) != std::end(items)) {
    found = true;
    return found;
  }
  //Check if the item is in a container in the room
  for(auto container : containers) {
    auto search = containerMap.find(container);
    auto containerObject = dynamic_cast<Container*>(search->second);
    found = containerObject->find_item(item);
    if(found == true) {
      return found;
    }
  }
  return found;
}

//For searching only the item vector in the room
bool Room::find_item(std::string item) {
  bool found = false;
  
  if(std::find(std::begin(items), std::end(items), item) != std::end(items)) {
    found = true;
  }
  return found;
}

void Room::remove_item(std::string item, std::unordered_map<std::string, Object*>& containerMap) {
  auto index = std::find(std::begin(items), std::end(items), item);
  
  //Check to see if the item is directly in the room
  if(index != std::end(items)) {
    items.erase(index);
  }
  
  //See if the item is in a container in the room
  else {
    for(auto container : containers) {
      auto search = containerMap.find(container);
      auto containerObject = dynamic_cast<Container*>(search->second);
      bool found = containerObject->find_item(item);
      if(found == true) {
        containerObject->remove_item(item);
        return;
      }
    }
  }
}

void Room::remove_item(std::string item) {
  auto index = std::find(std::begin(items), std::end(items), item);
  if(index != std::end(items)) {
    items.erase(index);
  }
}

void Room::remove_container(std::string container) {
  auto index = std::find(std::begin(containers), std::end(containers), container);
  if(index != std::end(containers)) {
    containers.erase(index);
  }
}

void Room::remove_creature(std::string creature) {
  auto index = std::find(std::begin(creatures), std::end(creatures), creature);
  if(index != std::end(creatures)) {
    creatures.erase(index);
  }
}

bool Room::find_creature(std::string creature) {
  if(std::find(std::begin(creatures), std::end(creatures), creature) != std::end(creatures)) {
    return true;
  }
  return false;
}

bool Room::find_object(std::string object) {
  bool found = false;
  
  //Check if the item is directly in the room
  if(std::find(std::begin(items), std::end(items), object) != std::end(items)) {
    found = true;
    return found;
  }
  //Check if the item is directly in the room
  if(std::find(std::begin(creatures), std::end(creatures), object) != std::end(items)) {
    found = true;
    return found;
  }
  //Check if the item is directly in the room
  if(std::find(std::begin(containers), std::end(containers), object) != std::end(items)) {
    found = true;
    return found;
  }
  return found;
}

void Room::find_triggers(std::string input, std::unordered_map<std::string, Object*>& objectMap, bool& fired) {
  for(auto trigger = std::begin(triggers); trigger < std::end(triggers); ++trigger) {
    bool needsDeletion = trigger->trigger_check(input, objectMap, fired);
    if(needsDeletion) {
      triggers.erase(trigger);
    }
  }
  for(auto containterName: containers) {
    auto container = objectMap[containterName];
    container->find_triggers(input, objectMap, fired);
  }
  for(auto itemName: items) {
    auto item = objectMap[itemName];
    item->find_triggers(input, objectMap, fired);
  }
  for(auto creatureName: creatures) {
    auto creature = objectMap[creatureName];
    creature->find_triggers(input, objectMap, fired);
  }
}



