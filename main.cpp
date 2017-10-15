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
#include <sstream>
#include <iterator>


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
  
  //Room creation
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
  
  //End Room Creation
  
  //Container creation
  
  node = mapNode->first_node("container");
  
  std::unordered_map<std::string, Container> containerMap;
  while(node) {
    Container container(node);
    containerMap.insert(std::make_pair(container.get_name(), container));
    node = node->next_sibling("container");
  }
  
  //End Container creation
  
  //Create the player's inventory
  auto inventory = Container();


	//END OF SETUP

	//START OF GAMEPLAY

	//Find the Entrance to start the game
	auto search = roomMap.find("Entrance");
	auto currentRoom = search->second;
  std::cout << currentRoom.get_description() << std::endl;
  
  //Start game
  bool exit_condition = false;
	while(exit_condition == false) {
		//Get user input
		std::string input;
		std::getline(std::cin, input);
    
    //n, s, e, w
    if(input == "n" || input == "s" || input == "e" || input == "w") {
       currentRoom = currentRoom.movement(input, roomMap);
    }
    
    //open exit
    if(input == "open exit") {
      exit_condition = currentRoom.exit_check();
    }
    
    //i
    if(input == "i") {
      inventory.open_container();
    }
    
    //open (container)
    if(input.substr(0,4) == "open") {
      std::string containerName = input.substr(5);
      bool found = currentRoom.find_container(containerName);
      if(found) {
        auto containerSearch = containerMap.find(containerName);
        auto container = containerSearch->second;
        container.open_container();
      }
      else {
        std::cout << "Error: that container is not in this room" << std::endl;
      }
    }
    
    //put (item) in (container)
    if(input.substr(0,3) == "put") {
      std::istringstream iss{input};
      std::vector<std::string> tokens{std::istream_iterator<std::string>{iss}, std::istream_iterator<std::string>{}};
      bool found = inventory.find_item(tokens[1]);
      if(found) {
        bool containerFound = currentRoom.find_container(tokens[3]);
        if(containerFound) {
          auto containerSearch = containerMap.find(tokens[3]);
          auto container = containerSearch->second;
          container.add_item(tokens[1]);
          
          inventory.remove_item(tokens[1]);
        }
        else {
          std::cout << "Error: that container is not in this room" << std::endl;
        }
        
      }
      else {
        std::cout << "Error: you dont have " << tokens[1] << " in your inventory" << std::endl;
      }
    }
    
	}


	return 0;
}

