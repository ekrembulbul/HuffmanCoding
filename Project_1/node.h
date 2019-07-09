#pragma once
#include <string>

struct Node
{
	std::string ch;
	int freq;
	std::string bitSeq;
	Node *left = NULL, *right = NULL;

	Node(std::string ch, int freq);
	bool operator<(const Node& rhs) const;
};

