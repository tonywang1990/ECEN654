#include <iostream>
#include <fstream>
#include <sstream>
#include <assert.h>     /* assert */
#include "graph.h"

void graph::init(int order, int size){
	net_size = size;
	node_size = order;
	matrix = new bool* [net_size];
	for (int i = 0; i < net_size; i++){
		matrix[i] = new bool [node_size];
		for (int j = 0; j < node_size; j++){
			matrix[i][j] = 0;
		}
	}
}

// Read net file and parse the file to matrix
void graph::read_file(std::string filename){
	int net, node, total_node, node_count;
	std::ifstream infile;
	std::string line;
	std::istringstream iss;
	std::string net_name;
	std::string total_node_string;
	std::string node_string;

	infile.open(filename.c_str());
	while (std::getline(infile, line))
	{
		// start a new net
		if (line.find("Net") != std::string::npos){
			//std::cout<<std::endl;
			net_name = line.substr(3,3);
			//std::cout<<net_name<<std::endl;
			iss.clear();
			iss.str(net_name); 
			iss >> net;
		    //std::cout<<net<<std::endl;

			iss.clear();
			total_node_string = line.substr(7,3);
			//	std::cout<<total_node_string<<std::endl;
			iss.str(total_node_string);
			iss >> total_node;
			//	std::cout<<total_node<<std::endl;

			iss.clear();
			node_string = line.substr(14);
			//	std::cout<<node_string<<std::endl;
			iss.str(node_string);
			node_count = 0;
			while (iss >> node){
				//std::cout<<node<<" ";
				node_count++;
				insert(net, node);
			}
		}
		// previous net continues
		else {
			iss.clear();
			iss.str(line);
			while (iss >> node){
				//std::cout<<node<<" ";
				node_count++;
				insert(net, node);
			}
		}
	}
}

void graph::plot_matrix(){
	printf("    ");
	for (int i = 0; i < node_size; i++){
		printf("%.2i   ",i);
	}
	std::cout<<std::endl;
	for (int i = 0; i < net_size; i++){
		printf("%.2i  ",i);
		for (int j = 0; j < node_size; j++){
			if (matrix[i][j] == 1)
				std::cout <<" *   ";
			else 
				std::cout<<"     ";
		}
		std::cout<<std::endl;
	}
}

void graph::plot_mat(){
	printf("    ");
	for (int i = 0; i < node_size; i++){
		printf("%.2i   ",i);
	}
	std::cout<<std::endl;
	for (int i = 0; i < node_size; i++){
		printf("%.2i ",i);
		for (int j = 0; j < node_size; j++){
			if (mat[i][j] != 0)
				std::cout <<" "<<mat[i][j]<<"  ";
			else 
				std::cout<<"     ";
		}
		std::cout<<std::endl;
	}
}

void graph::insert(int row, int col){
	assert(row > 0 && col > 0);
	matrix[row-1][col-1] = 1; 
}

bool graph::check(int row, int col){
	assert(row >= 0 && row <100 && col >= 0 && col<100);
	return matrix[row][col];
}

/*
void graph::convert(){
	int* link_set;
	for (int i = 0; i < net_size; i++){
		// count number of linked nodes
		int count = 0;
		for (int j = 0; j < node_size; j++){
			if (matrix[i][j] == 1)
				count++;
		}
		// put node number into linked set
		link_set = new int [count];
		int k = 0;
		for (int j = 0; j < node_size; j++){
			if (matrix[i][j] == 1) 
				link_set[k++] = j; 
		}
		
		for (int j = 0; j < count; j++){
			std::cout<<link_set[j]<<" ";
		}
		std::cout<<std::endl;
		assert(0);
	
		// mark edges in mat
		for (int i = 0; i < count; i++){
			int row = link_set[i];
			for (int j = 0; j < count; j++){ 
				int col = link_set[j];
				mat[row][col] += 1;
			}
		} 
	}
}
*/

int graph::NetSize(){
	return net_size;
}

int graph::NodeSize(){
	return node_size;
}
