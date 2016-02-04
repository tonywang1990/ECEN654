#include <iostream>
#include "partition.h"
int main(){
	partition Part;
	
	std::cout<<"+++++++++++Two way partition+++++++++++"<<std::endl;
	Part.two_way_partition();
	std::cout<<"+++++++++++Five way partition+++++++++++"<<std::endl;
	Part.five_way_partition();
	std::cout<<"+++++++++++Three way partition+++++++++++"<<std::endl;
	Part.three_way_partition();
	return 0;
}
