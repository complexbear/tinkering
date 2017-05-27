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

mutex go;

struct Node;

struct Node
{
	Node() : val(0), level(-1), inUse(-1) {}

	Node(const string& v, const vector<Node*>& ch)
	: val(v), children(ch), level(-1), inUse(-1)
	{}

	string val;
	vector<Node*> children;

	atomic_int level;
	atomic_int inUse;
};

struct Config
{
	int level;
	stringstream os;
};

void visitNode(Config& cfg, Node* n)
{
	{
		lock_guard<mutex> lk(go);
	}

	if(n == nullptr) return;

	// This node has already been visited
	if(n->level >= cfg.level) return;

	// Wait for our turn on the node in level order
	int l = n->level;
	while(! n->level.compare_exchange_weak(l, cfg.level))
	{}

	cfg.os << "ID:" << this_thread::get_id() << ", Level " << cfg.level << ", val = " << n->val << endl;
	n->level = cfg.level;
	n->val += string("x");

	for(auto child : n->children)
	{
		visitNode(cfg, child);
	}

}


int main() {

	/*
	 * Graph to test
	 *       A
	 *       |
	 *   B - C - D
	 *   |   |   |
	 *   E   E   F
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

	Config c1 = { 1 };
	Config c2 = { 2 };

	go.lock();
	std::thread t1(visitNode, ref(c1), A);
	std::thread t2(visitNode, ref(c2), A);
	go.unlock();

	t1.join();
	t2.join();

	cout << c1.os.str() << endl;
	cout << c2.os.str() << endl;
	return 0;
}
