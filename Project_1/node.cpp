#include "pch.h"
#include "Node.h"


Node::Node(std::string ch, int freq)
{
	left, right = NULL;
	this->ch = ch;
	this->freq = freq;
}

bool Node::operator<(const Node& rhs) const
{
	if (freq != rhs.freq)
		return freq < rhs.freq;
	else
		return ch < rhs.ch;
}
