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


Room::Room(rapidxml::xml_node<>* roomNode) : Object(roomNode) {
  
  //Get the type of the room
  rapidxml::xml_node<>* roomProperty = roomNode->first_node("type");
  if(roomProperty != nullptr) {
    type = roomProperty->value();
  }
  
  
  //Get the borders in the room
	roomProperty = roomNode->first_node("border");
	while(roomProperty) {
		rapidxml::xml_node<>* borderNode = roomProperty->first_node("direction");
		int borderIndex = direction_to_array(borderNode->value());
		borderNode = roomProperty->first_node("name");
		border[borderIndex] = std::string(borderNode->value());
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

Room* Room::movement(std::string direction, GameInformation& gameInfo) {
  int index = direction_to_array(direction);
  std::string roomName = border[index];
  if(roomName == "") {
    std::cout << "Can't go that way" <<std::endl;
    return this;;
  }
  
  auto search = gameInfo.objectMap.find(roomName);
  
  //Must take into account if the room was removed
  if(search == gameInfo.objectMap.end()) {
    std::cout << "Can't go that way" <<std::endl;
    return this;;
  }
  
  auto newRoom = dynamic_cast<Room*>(search->second);
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
    if(containerObject->check_take()) {
      found = containerObject->find_item(item);
      if(found == true) {
        return found;
      }
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

void Room::find_triggers(std::string input, GameInformation& gameInfo, bool& fired) {
  
  //Search the list of triggers
  for(auto trigger = std::begin(triggers); trigger < std::end(triggers); ++trigger) {
    //Check if the trigger's conditions are met, and if it is of type single
    bool needsDeletion = trigger->trigger_check(input, gameInfo, fired);
    if(needsDeletion) {
      triggers.erase(trigger);
    }
  }
  for(auto containterName: containers) {
    auto container = gameInfo.objectMap[containterName];
    container->find_triggers(input, gameInfo, fired);
  }
  for(auto itemName: items) {
    auto item = gameInfo.objectMap[itemName];
    item->find_triggers(input, gameInfo, fired);
  }
  for(auto creatureName: creatures) {
    auto creature = gameInfo.objectMap[creatureName];
    creature->find_triggers(input, gameInfo, fired);
  }
}



