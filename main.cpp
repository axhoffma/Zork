/*
 * main.cpp
 *
 *  Created on: Oct 13, 2017
 *      Author: austinhoffmann
 */

#include <iostream>
#include <unordered_map>
#include "./rapidXML/rapidxml_utils.hpp"
#include "./rapidXML/rapidxml.hpp"
#include "Object.h"
#include "Room.h"


int main(int argc, char* argv[]) {
	if(argc != 2) {
		std::cout << "Zork usage: ./Zork [filename]" << std::endl;
		return 0;
	}
	//create a c-string that will be parsed by rapidxml using the input file
	rapidxml::file<>ifile(argv[1]);
	//create a DOM model of the input file that is parsed
	rapidxml::xml_document<> doc;
	doc.parse<0>(ifile.data());


	//create an unordered map to store all of our Room objects
	std::unordered_map<std::string, Room> roomMap;

	//Map node is the first child of the XML overall tree
	rapidxml::xml_node<>* mapNode = doc.first_node("map");
	rapidxml::xml_node<>* node = mapNode->first_node("room");

	if(node == nullptr) {
		std::cout << "Could not find a valid room with the XML parser" <<std::endl;
		return 0;
	}

	//keep creating rooms while we have new ones
	while(node) {
		//create Room
		Room room(node);
		//Add room to unordered map
		roomMap.insert(std::make_pair(room.get_name(), room));
		//Get next room node
		node = node->next_sibling("room");
	}


	//END OF SETUP

	//START OF GAMEPLAY

	//Find the Entrance to start the game
	auto search = roomMap.find("Entrance");
	auto currentRoom = search->second;
  std::cout << currentRoom.get_description() << std::endl;
  bool exit_condition = false;
	while(exit_condition == false) {
		//Get user input
		std::string input;
		std::getline(std::cin, input);
    currentRoom = currentRoom.movement(input, roomMap);
	}


	return 0;
}

