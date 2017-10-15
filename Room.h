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
  std::vector<std::string> containers;

public:
	Room(rapidxml::xml_node<>* node);
	bool exit_check();
  Room movement(std::string direction, const std::unordered_map<std::string, Room>& roomMap);
  bool find_container(std::string container);

};


#endif /* ROOM_H_ */
