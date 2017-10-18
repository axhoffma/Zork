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
  auto inventory = Container();


	//END OF SETUP

	//START OF GAMEPLAY

	//Find the Entrance to start the game
	auto search = objectMap.find("Entrance");
	auto currentRoom = dynamic_cast<Room*>(search->second);
  std::cout << currentRoom->get_description() << std::endl;
  
  //Start game
  bool exit_condition = false;
	while(exit_condition == false) {
		//Get user input
		std::string input;
		std::getline(std::cin, input);
    
    //parse input into separate tokens
    std::istringstream iss{input};
    std::vector<std::string> tokens{std::istream_iterator<std::string>{iss}, std::istream_iterator<std::string>{}};
    
    //n, s, e, w
    if(tokens[0] == "n" || tokens[0] == "s" || tokens[0] == "e" || tokens[0] == "w") {
       currentRoom = currentRoom->movement(input, objectMap);
    }
    
    //open exit
    else if(input == "open exit") {
      exit_condition = currentRoom->exit_check();
    }
    
    //i
    else if(tokens[0] == "i") {
      inventory.open_container();
    }
    
    //open (container)
    else if(tokens[0] == "open" && tokens.size() == 2) {
      std::string containerName = tokens[1];
      bool found = currentRoom->find_container(containerName);
      if(found) {
        auto containerSearch = objectMap.find(containerName);
        auto container = dynamic_cast<Container*>(containerSearch->second);
        auto openable = container->check_open();
        if(openable) {
          container->open_container();
        }
        else {
          std::cout << "You can't open it" << std::endl;
        }
      }
      else {
        std::cout << "Error: that container is not in this room" << std::endl;
      }
    }
    
    //put (item) in (container)
    else if(tokens[0] == "put" && tokens.size() == 4) {
      bool found = inventory.find_item(tokens[1]);
      if(found) {
        bool containerFound = currentRoom->find_container(tokens[3]);
        if(containerFound) {
          auto containerSearch = objectMap.find(tokens[3]);
          auto container = dynamic_cast<Container*>(containerSearch->second);
          bool allowedInsert = container->check_accept(tokens[1]);
          if(allowedInsert) {
            container->add_item(tokens[1]);
            inventory.remove_item(tokens[1]);
            std::cout << "Item " << tokens[1] << " added to " << tokens[3] << std::endl;
          }
          else {
            std::cout << "You cannot put that item there" << std::endl;
          }
  
        }
        else {
          std::cout << "Error: that container is not in this room" << std::endl;
        }
          
      }
      else {
        std::cout << "Error: you dont have " << tokens[1] << " in your inventory" << std::endl;
      }
    }
    
    //take (item)
    else if(tokens[0] == "take" && tokens.size() == 2) {
      std::string itemName = tokens[1];
      bool found = currentRoom->find_item(itemName, objectMap);
      if(found) {
        inventory.add_item(itemName);
        currentRoom->remove_item(itemName, objectMap);
        
        std::cout << "Item " << itemName << " added to inventory" << std::endl;
      }
      else {
        std::cout << "Error: that item is not in the current room" << std::endl;
      }
    }
    
    //drop (item)
    else if(tokens[0]== "drop" && tokens.size() == 2) {
      std::string itemName = tokens[1];
      bool found = inventory.find_item(itemName);
      if(found) {
        inventory.remove_item(itemName);
        currentRoom->add_item(itemName);
        std::cout << itemName << " dropped" << std::endl;
      }
      else {
        std::cout << "Error: you do not have " << itemName << " in your inventory" << std::endl;
      }
    }
    
    //read (item)
    else if(tokens[0] == "read" && tokens.size() == 2) {
      std::string itemName = tokens[1];
      bool found = inventory.find_item(itemName);
      if(found) {
        auto search = objectMap.find(itemName);
        auto item = dynamic_cast<Item*>(search->second);
        item->read_writing();
      }
      else {
        std::cout << "Error: you do not have " << itemName << " in your inventory" << std::endl;
      }
    }
    
    //turn on (item)
    else if(tokens[0] == "turn" && tokens[1] == "on" && tokens.size() == 3) {
      std::string itemName = tokens[2];
      bool found = inventory.find_item(itemName);
      if(found) {
        Item* item = dynamic_cast<Item*>(objectMap[itemName]);
        //Get the vector of actions to perform
        auto actions = item->turn_on();
        for(auto action : actions) {
          parse_commands(action, objectMap);
        }
      }
      else {
        std::cout << "Error: " << tokens[2] << " is not in your inventory" << std::endl;
      }
    }
    
    //attack (creature) with (item)
    else if(tokens[0] == "attack" && tokens[2] == "with" && tokens.size() == 4) {
      std::string itemName = tokens[3];
      std::string creatureName = tokens[1];
      bool found = currentRoom->find_creature(creatureName);
      if(found) {
        found = inventory.find_item(itemName);
        if(found) {
          Creature* creature = dynamic_cast<Creature*>(objectMap[creatureName]);
          bool effective = creature->check_attack(itemName);
          if(effective) {
            //Check the attack conditions
            std::cout << "You attack " << creatureName << " with " << itemName << "!" << std::endl;
            creature->execute_attack(objectMap);
          }
          else {
            std::cout << "The item has no effect!" << std::endl;
          }
        }
        else {
          std::cout << "That item is not in your inventory" << std::endl;
        }
      }
      else {
        std::cout << "That creature is not in this room" << std::endl;
      }
    }
    
    else {
      std::cout << "Invalid Command" << std::endl;
    }
    
	}


	return 0;
}

