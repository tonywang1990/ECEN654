#include "partition.h"
#include <set>
#include <assert.h>
#include <iostream>

// all counting number(node, net) start from zero
partition::partition(){
	G.init(20, 100);
	G.read_file("net");
	debug = false;
}

void partition::init(int mode){
	srand(time(NULL));
	if (mode == 1){
		way = 2;
		part_size = G.NodeSize()/2;
		part = new std::set<int>[2];
		for (int i = 0; i < part_size; i++){
			part[0].insert(i);
			part[1].insert(i+10);
		}
	}
	else if (mode == 2){
		way = 2;
		part_size = G.NodeSize()/2;
		part = new std::set<int>[2];
		// randomly pick 10 node in A
		for (int i = 0; i < part_size; i++){
			int k = rand()%20;
			while(part[0].count(k) == 1){
				k = rand()%20;
			}
			part[0].insert(k);
		}
		// whatever is left goes to B
		for (int i = 0; i < part_size*2; i++){
			if (part[0].count(i) == 0){
				part[1].insert(i);
			}
		}
		assert(part[0].size() == part_size && part[1].size() == part_size);
	}
	else if (mode == 3){
		std::set<int> allocated;
		way = 5;
		part_size = G.NodeSize()/5;
		part = new std::set<int>[5];
		for (int i = 0; i < 5; i++){
			for (int j = 0; j < part_size; j++){
				int k = rand()%20;
				while (allocated.count(k) == 1){
					k = rand()%20;
				}
				part[i].insert(k);
				allocated.insert(k);
			}
		}
	}
	else if (mode == 4){
		std::set<int> allocated;
		set_size[0] = 4;
		set_size[1] = 6;
		set_size[2] = 10;
		way = 3;
		part_size = 4;
		part = new std::set<int>[way];
		for (int i = 0; i < 3; i++){
			for (int j = 0; j < set_size[i]; j++){
				int k = rand()%20;
				while (allocated.count(k) == 1){
					k = rand()%20;
				}
				part[i].insert(k);
				allocated.insert(k);
			}
		}
	}
	std::cout<<"Initial Partition:"<<std::endl;
	show_partition();
	std::cout<<"Initial cost: "<<get_total_cost()<<std::endl;
}



void partition::two_way_partition(){
	std::cout<<"*two way ascend order:"<<std::endl;
	init(1);
	bi_partition(part[0], part[1]);
	clear();
	final_output();
	
	std::cout<<"*two way random:"<<std::endl;
	init(2);
	bi_partition(part[0], part[1]);
	final_output();

}

void partition::three_way_partition(){
	int iter = 0;
	int onepass = 0;
	init(4);
	while (onepass != 1 && iter < 10){
		onepass = 1;
		for (int i = 0; i < way; i++){
			for (int j = i+1; j < way; j++){
				part_size = set_size[i];
				onepass *= bi_partition(part[i], part[j]);
			}
		}
		iter++;
	}
	final_output(iter);

}
void partition::five_way_partition(){
	int iter = 0;
	int onepass = 0;
	init(3);
	while (onepass != 1){
		onepass = 1;
		for (int i = 0; i < 5; i++){
			for (int j = i+1; j < 5; j++){
				onepass *= bi_partition(part[i], part[j]);
			}
		}
		iter++;
	}
	final_output(iter);
}
int partition::bi_partition(std::set<int>& setA, std::set<int>& setB){
	int *sum = new int[part_size];
	int *gain = new int[part_size];
	int *swapA = new int[part_size];
	int *swapB = new int[part_size];
	int iter = 0;

	while(1){
		iter++;
		// initialize
		for (int i = 0; i < part_size; i++){
			sum[i] = 0;
			gain[i] = 0;
			swapA[i] = 0;
			swapB[i] = 0;
		}
		int max_sum, max_i;
		// list all swap and gain
		loop(setA, setB, gain, swapA, swapB);
		// find the swap of max gain
		// assume gain[0] is the max sum
		max_sum = sum[0] = gain[0];
		max_i = 0;
		for (int i = 1; i < part_size; i++){
			sum[i] = sum[i-1] + gain[i];
			if (sum[i] >= max_sum){
				max_sum = sum[i];
				max_i = i;
			}
		}

		if (debug){
			for (int i = 0; i < part_size; i++){
				std::cout<<sum[i]<<" "<<swapA[i]<<" "<<swapB[i]<<std::endl;
			}
			std::cout<<max_sum<<" "<< max_i<<std::endl; 
			std::cout<<sum_net_cost(setA, setB, 0, 0, true)<<std::endl;
			std::cout<<"Iteration "<<iter<<std::endl;
		}
		
		//check if best swap gain > 0
		if (max_sum > 0){
			for (int i = 0; i <= max_i; i++){
				// commit swap
				swap(setA, setB, swapA[i], swapB[i]);
			}
		}
		else{
			break;
		}
		if (debug) {
			show_partition();
			std::cout<<"*"<<get_total_cost()<<std::endl;
		}	
	}
	delete []sum;
	delete []gain;
	delete []swapA;
	delete []swapB;
	return iter;
}

// loop through all nodes and calculate gain
void partition::loop(std::set<int> A, std::set<int> B, int* gain, int* swap_A, int* swap_B){
	int x, y;
	std::set<int> locked;
	locked.clear();
	for (int i = 0; i < part_size; i++){
		gain[i] = 0;
	}
	for (int i = 0; i < part_size; i++){
		gain[i] = find_max_gxy(A, B, locked, x, y);
		swap_A[i] = x;
		swap_B[i] = y;
		swap(A, B, x, y);
		locked.insert(x);
		locked.insert(y);
	}

}


// find the max gxy in partition excluding locked nodes
int partition::find_max_gxy(std::set<int> set_A, std::set<int> set_B, std::set<int> locked, int& max_x, int& max_y){
	int max_gxy = -10000;
	std::set<int>::iterator ita, itb;
	for (ita = set_A.begin(); ita != set_A.end(); ++ita){
		// if ita is not a locked node
		if (locked.count(*ita) == 0){
			for (itb = set_B.begin(); itb != set_B.end(); ++itb){
				// if itb is not a locked node
				if (locked.count(*itb) == 0){
					int g_x_y = g_xy(set_A, set_B, *ita, *itb);
					if (debug)
						std::cout<<"x,y= "<<*ita<<" "<<*itb<<" "<<g_x_y<<" "<<max_gxy<<std::endl;
					if (max_gxy < g_x_y){
						max_gxy = g_x_y;
						max_x = *ita;
						max_y = *itb;
					}	
				}
			}
		}
	}
	return max_gxy;

}
// calculate gain by exchanging x and y
int partition::g_xy(std::set<int> set_A, std::set<int> set_B, int x, int y){
	int cost_xy = 0;
	int cost_yx = 0;

	// cost of all nets that contain x or y)
	cost_xy = sum_net_cost(set_A, set_B, x, y, false);

	// swap x to set_B and y to set_A
	swap(set_A, set_B, x, y);

	// cost after swap
	cost_yx = sum_net_cost(set_A, set_B, x, y, false);

	return (cost_xy - cost_yx);
}

// swap x and y 
void partition::swap(std::set<int>& A, std::set<int>& B, int x, int y){
	if (A.count(x) == 0 || B.count(y) == 0){
		std::cout<<"node number not found: "<<x<<" "<<y<<std::endl;
		assert(0);
	}
	A.erase(A.find(x));
	B.erase(B.find(y));
	A.insert(y);
	B.insert(x);

}

// total net cost that contains node x and y
int partition::sum_net_cost(std::set<int> A, std::set<int> B, int x, int y, bool skip_check){
	int sum_cost = 0;
	// for each net
	for (int i = 0; i < G.NetSize(); i++){
		// if net contains x or y
		if (G.check(i,x) == 1|| G.check(i,y) == 1 || skip_check){
			sum_cost += net_cost(i, A, B);
			if (debug) std::cout<< sum_cost<<std::endl;
		}
	}
	return sum_cost;
}

// cost of the i_th net in partition A/B
int partition::net_cost(int i, std::set<int> A, std::set<int> B){
	int cost_A = 0;
	int cost_B = 0;
	// count number of nodes in A/B
	for (int j = 0; j < G.NodeSize(); j++){
		if (G.check(i,j) == 1){
			if (A.count(j) != 0){
				cost_A++;
			}
			else if (B.count(j) != 0){
				cost_B++;
			}
			/*
			else{
				std::cout<<"invalid node number: "<<j<<std::endl;
				assert(0);
			}
			*/
		}
		if (debug){
			std::cout<<"A:"<<cost_A<<"B:"<<cost_B<<std::endl;
		}
		
	}
	// return the smaller one
	return (cost_A < cost_B ? cost_A : cost_B);
}

// show current partition of A and B
void partition::show_partition(std::set<int> A, std::set<int> B){
	std::set<int>::iterator it;
	std::cout<<"Partition A: ";
	for (it=A.begin(); it!=A.end(); ++it)
		std::cout << *it << " ";
	std::cout << '\n';
	std::cout<<"Partition B: ";
	for (it=B.begin(); it!=B.end(); ++it)
		std::cout << *it << " ";
	std::cout << '\n';
}

void partition::show_partition(){
	std::set<int>::iterator it;
	for (int i = 0; i < way; i++){
		std::cout<<"P"<<i<<": ";
		for (it=part[i].begin(); it!=part[i].end(); ++it){
			printf("%.2i  ", *it);
		}
		std::cout << '\n';
	}
}

int partition::get_total_cost(){
	int total_cost = 0;
	for (int i = 0; i < way; i++){
		for (int j = i+1; j < way; j++){
			total_cost += sum_net_cost(part[i], part[j], 0, 0, true);
		}
	}
	return total_cost; 
}

void partition::final_output(int iter){
	std::cout<<"Total iteration:"<<iter<<std::endl;
	std::cout<<"Final Partition:"<<std::endl;
	show_partition();
	std::cout<<"Final cost: "<<get_total_cost()<<std::endl; 
}

void partition::clear(){
	delete []part;
}
