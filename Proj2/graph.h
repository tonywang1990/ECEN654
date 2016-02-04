#include <iostream>
#include <set>

class graph 
{
	private:
		int **module;
		std::set<int>* net;
		std::set<int> pio;
		int module_size;
		int net_size;
		int pio_size;

	public:
		graph();
		void clear();
		void import_data(std::string, std::string, std::string);
		int get_module(int, int);
		std::set<int>* get_net();
		std::set<int> get_pio();
};
