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
#include "Container.hpp"
#include "Creature.hpp"
#include <sstream>
#include <iterator>
#include "helper.hpp"


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


	//create an unordered map to store all of our  objects
	std::unordered_map<std::string, Object*> objectMap;

	//Map node is the first child of the XML overall tree
	rapidxml::xml_node<>* mapNode = doc.first_node("map");
  
  //Room creation
	rapidxml::xml_node<>* node = mapNode->first_node("room");

	if(node == nullptr) {
		std::cout << "Could not find a valid room with the XML parser" <<std::endl;
		return 0;
	}

	//keep creating rooms while we have new ones
	while(node) {
		//create Room
		Room* room = new Room(node);
		//Add room to unordered map
		objectMap.insert(std::make_pair(room->get_name(), room));
		//Get next room node
		node = node->next_sibling("room");
	}
  
  //End Room Creation
  
  //Container creation
  
  node = mapNode->first_node("container");
  
  while(node) {
    Container* container = new Container(node);
    objectMap.insert(std::make_pair(container->get_name(), container));
    node = node->next_sibling("container");
  }
  
  //End Container creation
  
  //Item creation
  
  node = mapNode->first_node("item");
  
  while(node) {
    Item* item = new Item(node);
    objectMap.insert(std::make_pair(item->get_name(), item));
    node = node->next_sibling("item");
  }
  
  //End item Creation
  
  //Creature Creation
  
  node = mapNode->first_node("creature");
  
  while(node) {
    Creature* creature = new Creature(node);
    objectMap.insert(std::make_pair(creature->get_name(), creature));
    node = node->next_sibling("creature");
  }
  
  //End creature creation
  
  //Create the player's inventory
  auto inventory = new Container();
  objectMap.insert(std::make_pair(inventory->get_name(), inventory));


	//END OF SETUP

	//START OF GAMEPLAY

	//Find the Entrance to start the game
	auto search = objectMap.find("Entrance");
	auto currentRoom = dynamic_cast<Room*>(search->second);
  
  //Add the Current Room to the map, this is used to look up room in other functions
  objectMap.insert(std::make_pair("Current Room", currentRoom));
  std::cout << currentRoom->get_description() << std::endl;
  
  //Start game
  bool exit_condition = false;
	while(exit_condition == false) {
		//Get user input
		std::string input;
		std::getline(std::cin, input);    
    parse_user_commands(input, objectMap);
	}


	return 0;
}

