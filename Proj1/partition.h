// partition class stores the current partition of the graph
#include <set>
#include "graph.h"
class partition{
	protected:
		std::set <int> *part;
		graph G; 
		int way;
		int part_size;
		int set_size[3];
		bool debug;
	public:
		partition();
		void five_way_partition();
		void three_way_partition();
		void two_way_partition();
		int bi_partition(std::set<int>&,std::set<int>&);
		void loop(std::set<int>, std::set<int>, int*, int*, int*);
		int find_max_gxy(std::set<int>, std::set<int>, std::set<int>, int&, int&);
		int g_xy(std::set<int>, std::set<int>, int, int);
		void swap(std::set<int>&, std::set<int>&, int, int);
		int sum_net_cost(std::set<int>, std::set<int>, int, int, bool);
		int net_cost(int, std::set<int>, std::set<int>);
		void show_partition(std::set<int>, std::set<int>);
		void show_partition();
		int get_total_cost();
		void final_output(int iter = 1);
		void init(int);
		void clear();
};
