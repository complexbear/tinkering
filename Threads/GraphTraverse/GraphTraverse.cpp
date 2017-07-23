//============================================================================
// Demonstration of multithreaded concurrent graph traversal
//============================================================================

#include <iostream>
#include <thread>
#include <atomic>
#include <mutex>
#include <vector>
#include <string>
#include <sstream>

using namespace std;


void log(const string& s) {
	static mutex l;
	lock_guard<mutex> lock(l);
	
	cout << s << endl;
}

#define LOG(msg) \
	stringstream os; \
	os << msg; \
	log(os.str());

struct Node;

struct Node
{
	Node() : val(0), level(-1) {}

	Node(const string& v, const vector<Node*>& ch)
	: val(v), children(ch), level(-1)
	{}

	string val;
	vector<Node*> children;

	atomic_int level;
	mutex inUse;
};

struct Config
{
	int level;
};

void visitNode(int level, Node* n)
{
	if(n == nullptr) return;

	// This node has already been visited
	if(n->level >= level) return;

	// Wait for our turn on the node in level order
	int l = n->level;
	while(! n->level.compare_exchange_weak(l, level))
	{}

	// Now wait for excusive access to the node from other traversing threads at this level
	{
		lock_guard<mutex> lock(n->inUse);
		LOG("ID:" << this_thread::get_id() << ", Level " << level << ", val = " << n->val);

		n->level = level;
		n->val += string("x");
	}

	// Visit connected nodes
	for(auto child : n->children)
	{
		visitNode(level, child);
	}

}


int main() {

	/*
	 * Graph to test
	 *       A
	 *       |
	 *   B - C - D
	 *   \   |   |
	 *    ---E   F
	 *       |   |
	 *       D   A
	 */
	Node* E = new Node("E", {nullptr});
	Node* F = new Node("F", {});
	Node* D = new Node("D", {F});
	Node* C = new Node("C", {E, D});

	Node* B = new Node("B", {E});
	Node* A = new Node("A", {B,C,D});

	F->children.push_back(A);
	E->children.push_back(D);
	
	std::thread t1(visitNode, 1, A);
	std::thread t2(visitNode, 2, A);
	
	t1.join(); 
	t2.join();

	return 0;
}
