#include "graph.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include "assert.h" 

graph::graph( ):
module_size(100),
net_size(30),
pio_size(30)
{
	
	module = new int* [100];
	for (int i = 0; i < module_size; i++){
		module[i] = new int [4];
		for (int j = 0; j < 4; j++){
			module[i][j] = 0;	
		}
	}
	
	net = new std::set<int> [net_size];

}
void graph::clear(){
	for (int i = 0; i < module_size; i++){
		delete []module[i];
	}
	delete []module;
	delete []net;
	pio.clear();
}

void graph::import_data(std::string Module, std::string Net, std::string Pio){
	std::ifstream input;
	std::string line;
	std::istringstream iss;
	int count = 0;

	// read Module
	input.open(Module.c_str());
	while (std::getline(input, line)){
		iss.clear();
		iss.str(line);
		int dummy;
		iss >> dummy;
		for (int j = 0; j < 4; j++){
			iss >> module[count][j]; 
		}
		count++;
	}
	assert(count == module_size);
	input.close();
	/*
	for (int i = 0; i < 100; i++){
		for (int j = 0; j < 4; j++){
			std::cout<<module[i][j]<<" ";
		}
		std::cout<<std::endl;
	}
	*/

	// read net 
	input.open(Net.c_str());
	count = 0;
	while (std::getline(input, line)){
		iss.clear();
		iss.str(line);
		int snet;
		// discard the first number
		iss >> snet;
		while (iss >> snet){
			net[count].insert(snet);
		}
		count++;
	}
	assert(count == net_size);
	input.close();
	/*
	std::set<int>::iterator it;
	for (int i = 0; i < net_size; i++){
		for (it = net[i].begin(); it != net[i].end(); it++){
			std::cout<<" "<<*it;
		}
		std::cout<<std::endl;
	}
	*/
	
	// read pio
	input.open(Net.c_str());
	count = 0;
	while (std::getline(input, line)){
		iss.clear();
		iss.str(line);
		int n;
		iss >> n;
		pio.insert(n);
		count++;
	}
	assert(count == pio_size);
	input.close();
}

int graph::get_module(int n, int m){
	assert(n < 100 && m < 4);
	return module[n][m];
}

std::set<int>* graph::get_net(){
	return net;
}

std::set<int> graph::get_pio(){
	return pio;
}
