// Boggle.cpp
// Given a Boggle board and a dictionary of words, find all possible words on the board
//

// TODO - add gtest 
#include <iostream>
#include <functional>
#include <set>
#include <vector>

// Convet the dictionary into a Trie structure that we can search
struct Node
{
	Node() : val(' '), marker(false) {}
	Node* add(char c) 
	{ 
		Node* n = new Node;
		n->val = c;
		children.push_back(n); 
		return n;
	}
	Node* get(char c)
	{
		if (val == c) 
			return this;
		for (const auto& n : children)
			if (n && n->val == c) return n;
		return nullptr;
	}

	char val;
	bool marker;
	std::vector<Node*> children;
};

struct Trie
{
	Trie() : root(new Node) {}

	~Trie() 
	{
		// Delete nodes recursively
		std::function<void(Node*)> deleteNode = [&deleteNode](Node* n) {
			for (Node* c : n->children)
				deleteNode(c);
			delete n;
		};

		deleteNode(root);
	}

	void addString(const std::string& s)
	{
		Node* n = root;
		for (const char c : s)
		{
			Node* next = n->get(c);
			if (next == nullptr)
				next = n->add(c);
			n = next;
		}
		n->marker = true;
	}

	bool search(const std::string& s)
	{
		Node* n = root;
		for (const char c : s)
		{
			n = n->get(c);
			if (n == nullptr) break;
		}
		return (n && n->marker);
	}

	Node* root;
};

typedef std::set<std::string> Dictionary;
typedef std::vector<std::vector<char>> BBoard;




std::vector<std::string> findWords(const BBoard& board, const Dictionary& dict)
{
	std::vector<std::string> results;

	// Walk through the board testing each cell as if it is the first letter of a word.

	// When moving to a neighbour cell, append it to the parent cell to build words
	
	


	return results;
}

int main()
{
	Dictionary d{
		"GIT",
		"GOOGLE",
		"GOO"
	};

	BBoard board{
		{'G', 'I', 'P'},
		{'O', 'T', 'X'},
		{'O', 'L', 'E'}
	};

	// Trie test
	Trie t;
	for (const auto& w : d) t.addString(w);

	std::cout << "Search GOO: " << t.search("GOO") << std::endl;
	std::cout << "Search GOT: " << t.search("GOT") << std::endl;
	std::cout << "Search GOOGLE: " << t.search("GOOGLE") << std::endl;

    return 0;
}

