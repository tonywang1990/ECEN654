// graph class reads, stores and graph data in a matrix
#include <iostream>
class graph{
	protected:
		int net_size;
		int node_size;
		bool** matrix;
		int** mat;
	public:
		void init(int, int);
		void read_file(std::string);
		void plot_matrix();
		void plot_mat();
		void insert(int, int);
		bool check(int, int);
		void convert();
		int NetSize();
		int NodeSize();
};
