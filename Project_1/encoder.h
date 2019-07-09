#pragma once
#include <set>
#include "Node.h"

class encoder
{
public:
	encoder(std::string filePath);

private:
	bool encode(std::string fileName);
	std::string readText(std::string fileName);
	int findFreq(std::string text, std::array<std::string, 256> &ch, std::array<int, 256> &freq);
	Node *createTree(std::set<Node> &nodes);
	void findBitSeq(Node *head, std::set<Node> &nodes, std::string &bitSeqTmp);
	void writebit(int bit, unsigned char &acc, int &bitcount, std::string &writeText, int lastBit);
	void writebyte(unsigned char &acc, std::string &writeText);
};

