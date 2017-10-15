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
}

Room Room::movement(std::string direction, const std::unordered_map<std::string, Room>& roomMap) {
  int index = direction_to_array(direction);
  std::string roomName = border[index];
  if(roomName == "") {
    std::cout << "Can't go that way" <<std::endl;
    return *this;
  }
  
  auto search = roomMap.find(roomName);
  
  //Must take into account if the room was removed
  if(search == roomMap.end()) {
    std::cout << "Can't go that way" <<std::endl;
    return *this;
  }
  
  Room newRoom = search->second;
  std::cout << newRoom.get_description() << std::endl;
  return newRoom;  
}



