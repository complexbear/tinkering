#pragma once

/*
 * Implementation of a Trie data structure for holding strings
 * 
 */

// TODO - add gtest 

#include <functional>
#include <string>
#include <vector>

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

