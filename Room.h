/*
 * Room.h
 *
 *  Created on: Oct 13, 2017
 *      Author: austinhoffmann
 */

#ifndef ROOM_H_
#define ROOM_H_

#include "Object.h"
#include <iostream>
#include <unordered_map>
#include "./rapidXML/rapidxml.hpp"


class Room : public Object {
	std::string type = "regular";
	std::array<std::string, 4> border;

public:
	Room(rapidxml::xml_node<>* node);
	bool exit_check();
  Room movement(std::string direction, const std::unordered_map<std::string, Room>& roomMap);

};


#endif /* ROOM_H_ */
