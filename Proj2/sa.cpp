#include "sa.h"
#include <queue>
#include <vector>
#include <stack>
#include <iomanip>
#include "assert.h"

sa::sa(){
	pe.clear();
	leaves.clear();
	root = new node(-1, 0, 0, 0, 0, false, 0);
}

// update (width, length, x, y, dis) by one dfs and one bfs
void sa::update(node* R){
	update_dfs(R);
	update_bfs(R);
}

// update each node(width, length) using dfs
void sa::update_dfs(node* N){
	node* lc = N->left;
	node* rc = N->right;
	node* fat;
	node* slim;
	int width, length, width1, length1;
	// if it's a H or V
	if (N -> item < 0){

		// 1. update left node first
		update_dfs(lc);
		update_dfs(rc);

		// 2. calculate width and length
		if (N->item == -1){ // it's a H
			width = lc->width + rc->width;
			length = lc->length > rc->length ? lc->length : rc->length;
			if (lc->length > rc->length){
				fat = lc;
				slim = rc; 
			}
			else{
				fat = rc;
				slim = lc;
			}
			// flip the fat one
			width1 = fat->length + slim->width;
			length1 = fat->width > slim->length ? fat->width : slim->length;
		}
		if (N->item == -2){ // its a V
			width = lc->width > rc->width ? lc->width : rc->width;
			length = lc->length + rc->length;
			if (lc->length > rc->length){
				fat = lc;
				slim = rc; 
			}
			else{
				fat = rc;
				slim = lc;
			}
			// flip the slim one
			width1 = fat->width > slim->length ? fat->width : slim->length;
			length1 = fat->length + slim->width;
		}
		if (width*length > width1*length1){
			N->width = width1;
			N->length = length1;
		}
		else {
			N->width = width;
			N->length = length;
		}
	}
}

// update each node(x, y, dis) using bfs
// left node --->> bottom - left
// right node -->> top - right
void sa::update_bfs(node* R){
	leaves.clear();
	node* N;
	std::queue <node*> Q;
	Q.push(R);
	while (Q.empty() != 1){
		N = Q.front();
		Q.pop();
		if (N->item < 0){ // it's a H or V
			node* nl = N->left;
			node* nr = N->right;
			if (N->item == -1){ //it's a H
				nl->x = N->x;
				nl->y = N->y;
				nr->x = N->x;
				nr->y = N->y + nl->width;
			}
			else { // it's a V
				nl->x = N->x;
				nl->y = N->y;
				nr->x = N->x + nl->length;
				nr->y = N->y;
			}
			nl->dis = N->dis+1;
			nr->dis = N->dis+1;
			Q.push(nl);
			Q.push(nr);
		}
		else{ // it's a leaf node
			leaves.push_back(N);	
		}
	}
	assert(leaves.size() == 100);
}

// Use BFS to construct a psuedo random tree
// the pointer to the root node is changed after new statement, thereby pass by reference is needed
void sa::random_tree(node* &R){ 	
	int count=0;
	node* N;
	std::queue <node*> Q;
	Q.push(R);
	while (Q.empty() != 1){
		N = Q.front();
		// break on level 6
		if (N->dis == 6) break;
		Q.pop();
		int type = N->item == -1 ? -2 : -1;
		N -> left = new node(type, 0, 0, 0, 0, false, N->dis + 1);
		N -> right = new node(type, 0, 0, 0, 0, false, N->dis +1);
		Q.push(N->left);
		Q.push(N->right);
		count++;	
	}

	// assign leaf nodes	
	std::vector<int> node_array(100);
	for (int i = 0; i < node_array.size(); i++){
		node_array[i] = i;
	}
	//first 36*2 leaf node of distance 6
	for (int i = 0; i < 36; i++){
		N = Q.front();
		Q.pop();
		N -> left = new node(0, 0, 0, 0, 0, false, N->dis + 1);
		N -> right = new node(0, 0, 0, 0, 0, false, N->dis + 1);
		// assign left node
		int n = rand()%node_array.size();
		assign(N->left, node_array[n]);
		node_array.erase(node_array.begin()+n);
		// assign right node
		n = rand()%node_array.size();
		assign(N->right, node_array[n]);
		node_array.erase(node_array.begin()+n);
	}
	//rest 28 leaf nodes are of distance 5
	//std::cout<<Q.size()<<node_array.size()<<std::endl;
	assert(Q.size() == node_array.size());
	while (Q.empty() != 1){
		N = Q.front();
		Q.pop();
		int n = rand()%node_array.size();
		assign(N,node_array[n]);
		node_array.erase(node_array.begin()+n);
	}
}
// assign width length and pio to a leaf node
void sa::assign(node* N, int n){
	N->item = n;
	int a = G.get_module(n,2);
	int b = G.get_module(n,3);
	// swap
	if (a < b){
		int c = a;
		a = b;
		b = c;
	}
	N->length = a;
	N->width = b;
	if (G.get_pio().count(n+1) == 1){
		N->pio = true;
	}
}

void sa::tree2pe(node* R){
	if (R->item < 0){
		tree2pe(R->left);
		tree2pe(R->right);
		pe.push_back(R->item);
	}
	else{
		pe.push_back(R->item);
	}
}

sa::node* sa::pe2tree(){
	node* N;
	std::stack<node*> S;
	assert(S.empty());
	for (int i = 0; i < pe.size(); i++){
		// leaf node
		if (pe[i] >= 0){
			N = new node(pe[i], 0, 0, 0, 0, false, 0);
			assign(N,pe[i]);
			S.push(N);
		}
		// internal node H/V
		else{
			N = new node(pe[i], 0, 0, 0, 0, false, 0);
			N->right = S.top();
			S.pop();
			N->left = S.top();
			S.pop();
			S.push(N);
		}
	}
	return N;
}

double sa::get_cost(node* R){
	int w = R->width;
	int l = R->length;
	std::set<int>::iterator it;
	std::set<int>* net = G.get_net();
	//** calculate Manhattan perimeter A
	double A = 2*(w + l);
	//** calculate Aspect ratio S
	int max = l > w ? l : w;
	int abs = l > w ? l-w : w-l;
	double S = (double)abs / (double)max; 
	//** calculate Connectivity C
	double C = 0.0;
	// for each net
	for (int i = 0; i < 30; i++){
		int max_x = 0;
		int max_y = 0;
		int min_x = 0;
		int min_y = 0;
		// for each node in net
		for (it = net[i].begin(); it != net[i].end(); it++){
			int nname = *it - 1; // counting from 0
			// locate node using leaves
			node* N = leaves.at(nname);
			max_x = (max_x > N->x + N->length) ? max_x : N->x + N->length;
			max_y = (max_y > N->y + N->width) ? max_y : N->y + N->width;
			min_x = (min_x < N->x) ? min_x : N->x;
			min_y = (min_y < N->y) ? min_y : N->y;
		}
		// sum up semiperimeter of enclosing rectangle
		C += (max_x - min_x) + (max_y - min_y);
	} 	
	//** calculate I/O connection PIO
	double PIO = 0.0;
	// for each primary I/O pin
	int a = G.get_pio().size();
	for (it = G.get_pio().begin(); it != G.get_pio().end(); it++){
		
		// locate leaf node
		int nname = *it - 1;
		node* N = leaves.at(nname);
		PIO += (N->x > N->y ? N->x : N->y);
	}
	std::cout<<"A: "<<A<<" S:"<<S<<" C:"<<C<<" PIO:"<<PIO<<" ";

	//** sum up total cost
	double cost = 1 * A + 10 * S + 3 * C + 6 * PIO;  
	return cost;
}

int sa::move1(){
	int loc = 0;
	int M = -1;
	int N = -1;
	// pick a leaf node
	while (M < 0){
		loc = rand()%pe.size();
		M = pe[loc];
	}
	// pick the leaf after N
	int loc1 = loc;
	while (N < 0){
		loc1++;
		N = pe[loc1];
		if (loc1 > pe.size()){
			return 1;
		}
	}
	//swap
	pe[loc] = N;
	pe[loc1] = M; 
	return 0;
}

int sa::move2(){
	int M = 1;
	int loc;
	while(M > 0){
		loc = rand()%pe.size();
		M = pe[loc];
	}
	int loc_end = loc;
	while (M < 0){
		loc_end++;
		M = pe[loc_end];
		if (loc_end > pe.size())
			return 1;
	}
	for (int i = loc; i <loc_end; i++){
		if (pe[i] == -1) pe[i] = -2;
		else if (pe[i] == -2) pe[i] = -1;
		else assert(0);
	}
}

int sa::move3(){
	bool done = false;
	while (!done){
		int loc;
		int M = 1;
		int N = 1;
		// find a pair of operator and operand
		while (M*N > 0){
			loc = rand()%pe.size();
			M = pe[loc];
			N = pe[loc+1];
		}
		// check if skewed
		if (pe[loc-1] == N || pe[loc+2] == M)
		{
			done = false;
		}
		// check balloting property
		else{
			int count = 0;
			for (int i = 0; i <= loc+1; i++){
				if(pe[i] < 0){
					count++;
				} 
			}
			if (2*count < loc){
				// swap
				pe[loc] = N;
				pe[loc+1] = M;
				done = true;
			}
			else{
				done = false;
			}
		} 
	}
	return 0;
}


//logistic
void sa::BFS(node* R){
	node* N;
	std::queue <node*> Q;
	Q.push(R);
	while (Q.empty() != 1){
		N = Q.front();
		Q.pop();
		if (N->left != NULL && N->right!= NULL){
			Q.push(N->left);
			Q.push(N->right);
		}
		check(N);
		print(N);
	}
}

void sa::delete_tree(node* N){
	//std::cout<<N->dis<<" "<<std::endl;
	if (N->left != NULL) delete_tree(N->left);
	if (N->right!= NULL) delete_tree(N->right);
	delete N;
}

void sa::check(node* N){
	if(N->item >= 0){ // it is a leaf
		assert(N->left == NULL && N->right == NULL);
	}
	else {
		assert(N->left != NULL && N->right != NULL);
	}
}

void sa::print(node* N){
	char type;
	if (N->item == -1){
		type = 'H';
	}
	else if (N->item == -2){
		type = 'V';
	}
	else{
		type = 'l';
	}
	std::cout<<type<<"  "<<"w,l = "<<N->width<<", "<<N->length<<"  cdn: ("<<N->x<<","<<N->y<<")"<<"  pio: "<<N->pio<<"  dis: "<<N->dis<<std::endl;
}

void sa::print_pe(){
	for(int i = 0; i < pe.size(); i++){
		if (pe[i] == -1) std::cout<<"H ";
		else if (pe[i] == -2) std::cout<<"V ";
		else
			std::cout<<pe[i]<<" ";
	}
	std::cout<<std::endl;
}

void sa::test(){
	G.import_data("module", "net", "pio");	
	random_tree(root);
	update(root);
//	BFS(root);
	std::cout<<get_cost(root)<<std::endl;
	tree2pe(root);
//	move3();
	delete_tree(root);
	root = pe2tree();
	update(root);
//	BFS(root);
	std::cout<<get_cost(root)<<std::endl;

}

int sa::move(int i){
	if (i == 1){
		return move1();
	}
	else if (i == 2){
		return move2();
	}
	else {
		return move3();
	}
}
void sa::loop(){
	std::vector<int> E, NE, BE;
	double  T = 100;
	double an = 0.9;
	int uphill = 0;
	int MT = 0;
	int M = 0;
	int N = 10;
	int reject = 0;
	int cost, new_cost, best_cost;
	
	// initial solution
	G.import_data("module", "net", "pio");	
	random_tree(root);
	update(root);
	tree2pe(root);
	E = pe;
	best_cost = get_cost(root);
	cost = get_cost(root);
	std::cout<<"init cost: "<<cost<<std::endl;
	srand(time(NULL));
	
	while(1) {
		MT = 0;
		uphill = 0;
		reject = 0;
		while(1) {
			int suc = 1;
			int sel;
			while (suc == 1){
				sel = rand()%3 + 1; 
				suc = move(sel);
			}
			std::cout<<"move "<< sel<<": MT:"<<MT<<" T:"<<T<<" ";
			MT = MT + 1;
			delete_tree(root);
			root = pe2tree();
			update(root);
			NE = pe;
			new_cost = get_cost(root);
			int d_cost = new_cost - cost;
			// accept
			double random = (double) rand()/RAND_MAX;
			double limit = exp(-d_cost/T);
			std::cout<<random<<" "<<limit<<" ";
			if (d_cost < 0 || random < limit){
				std::cout<<"accept ";
				E = NE;
				cost = new_cost;
				// uphill
				if (d_cost > 0){
					uphill++;
				}
				// downhill and best
				else if (cost < best_cost){
					BE = E;
					best_cost = cost;
				}
			}
			else {
				reject++;
				std::cout<<"rej ";
			}
			std::cout<<"NE: "<<new_cost<<" BE: "<<best_cost<<std::endl;
			if (uphill > N || MT > 2*N) break;
		}
		T = an*T;
		if (double(reject)/MT > 0.95 || T < 1e-5) break;
	}
	std::cout<<"Best solution: "<<best_cost<<std::endl;

}
