#include "graph.h"
#include "math.h"
#include <vector>

class sa{
	private: 
		struct node{
			// leaf node name/slicing type
			int item;
			// left and right child
			node* left;
			node* right;
			// length >= width 
			int width;
			int length;
			// coordinates of bottom-left corner
			int x;
			int y;
			// has a pio?
			bool pio;
			// distance to root
			int dis;
			node (int _item, int _w, int _l, int _x, int _y, bool _pio, int _d): 
				item(_item),
				left(NULL),
				right(NULL),
				width(_w),
				length(_l),
				x(_x),
				y(_y),
				pio(_pio),
				dis(_d)
			{}
		};
		graph G;
		node *root;
		int io_length;
		std::vector<int> pe;
		std::vector<node*> leaves;


	public:
		sa();
		int move1();
		int move2();
		int move3();
		void update(node*);
		void update_dfs(node*);
		void update_bfs(node*);
		void test();
		void assign(node*, int);
		void random_tree(node*& ); 
		void BFS(node*);
		void delete_tree(node* );
		void check(node*);
		void print(node* );
		void tree2pe(node*);
		node* pe2tree();
		void print_pe();
		double get_cost(node*);
		int move(int);
		void loop();
};
