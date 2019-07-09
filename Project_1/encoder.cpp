#include "pch.h"
#include "encoder.h"
#include <iostream>
#include <array>
#include <map>
#include <fstream>

encoder::encoder(std::string filePath)
{
	if (encode(filePath))
		puts("Encode successful.");
	else
		puts("Encode failed.");
}

bool encoder::encode(std::string fileName)
{
	std::string text = readText(fileName);

	std::array<std::string, 256> ch;
	std::array<int, 256> freq;

	int size = findFreq(text, ch, freq);

	//std::cout << text << std::endl;
	//std::cout << size << std::endl;
	//for (int i = 0; i < size; i++)
	//{
	//	std::cout << ch[i] << ": " << freq[i] << std::endl;
	//}

	std::set<Node> nodes;

	for (int i = 0; i < size; i++)
	{
		Node node(ch[i], freq[i]);
		nodes.insert(node);
	}

	//std::cout << std::endl;
	//for (auto &i : nodes)
	//{
	//	std::cout << i.ch << ": " << i.freq << " -> " << &i << std::endl;
	//}

	Node *head = createTree(nodes);
	std::string bitSeqTmp;
	findBitSeq(head, nodes, bitSeqTmp);

	//std::cout << std::endl;
	//for (auto &i : nodes)
	//{
	//	std::cout << i.ch << ": " << i.freq << " -> " << &i << "\t\t\t" << i.bitSeq << std::endl;
	//}

	std::map<char, std::string> dict;

	for (auto &i : nodes)
	{
		if (!i.bitSeq.empty())
			dict.insert(std::pair<char, std::string>(i.ch[0], i.bitSeq));
	}

	//for (auto &i : dict)
	//{
	//	std::cout << i.first << ": " << i.second << std::endl;
	//}

	std::string huffText;
	for (auto i : text)
	{
		huffText.append(dict.find(i)->second);
	}

	//std::cout << huffText.size() << std::endl;

	unsigned char acc = 0;
	int bitcount = 0;
	std::string writeText;

	int lastBitCount = huffText.size() % 8;
	//std::cout << lastBitCount << ", " << huffText.size() / 8 << std::endl;

	for (auto i : huffText)
	{
		//std::cout << i;
		//std::cout << i - '0' << ", ";
		writebit(i - '0', acc, bitcount, writeText, 8);
	}

	

	if (lastBitCount != 0)
	{
		std::string sub = huffText.substr(huffText.size() - lastBitCount, huffText.size());
		acc = 0;
		bitcount = 0;
		for (auto i : sub)
			writebit(i - '0', acc, bitcount, writeText, lastBitCount);
	}

	//std::cout << writeText.size() << std::endl;

	std::ofstream outf;
	outf.open("huf_text.huf", std::ios::out | std::ios::binary);
	outf.write(writeText.c_str(), writeText.size());
	outf.close();

	outf.open("huf_text.txt");
	//printf("LastBitCount: %d", lastBitCount);

	outf << lastBitCount << std::endl;
	outf << dict.size() << std::endl;

	for (auto i : dict)
	{
		outf << i.first << ":" << i.second << std::endl;
	}

	outf.close();
	//for (auto i : dict)
	//	std::cout << i.first << ": " << i.second << std::endl;

	std::fstream f;
	f.open("huf_text.huf");
	if (f.fail())
		return false;
	else
		return true;
}

void encoder::writebit(int bit, unsigned char &acc, int &bitcount, std::string &writeText, int lastBit)
{
	//std::cout << bitcount;
	//std::cout << bit;
	acc |= (bit << bitcount);
	if (++bitcount == lastBit)
	{
		writebyte(acc, writeText);
		//std::cout << acc << std::endl;
		acc = 0;
		bitcount = 0;
	}
}

void encoder::writebyte(unsigned char &acc, std::string &writeText)
{
	//printf("%d ", acc);
	writeText.append(1, acc);
}

std::string encoder::readText(std::string fileName)
{
	std::ifstream myFile;
	myFile.open(fileName);
	std::string text;
	std::string buf;
	//std::cout << "Reading from the file..." << std::endl;

	while (!myFile.eof())
	{
		std::getline(myFile, buf);
		text += buf;
		text += 10;
	}

	text.pop_back();


	//myFile >> buf;
	//text.append(buf);
	//while (myFile >> buf)
	//{
	//	text.append(" ");
	//	text.append(buf);
	//}

	myFile.close();
	return text;
}

int encoder::findFreq(std::string text, std::array<std::string, 256> &ch, std::array<int, 256> &freq)
{
	int size = 0;
	int i;
	bool flag;
	for (auto s : text)
	{
		flag = false;
		i = 0;
		while (!flag && i < size)
		{
			if (ch[i][0] == s)
				flag = true;
			else
				i++;
		}
		if (flag)
			freq[i]++;
		else
		{
			ch[size] = s;
			freq[size] = 1;
			size++;
		}
	}
	return size;
}

Node *encoder::createTree(std::set<Node> &nodes)
{
	std::set<Node>::iterator it;
	it = nodes.begin();

	Node *n1 = const_cast<Node*>(&(*it));
	it++;
	Node *n2 = const_cast<Node*>(&(*it));
	it++;

	Node n(n1->ch + n2->ch, n1->freq + n2->freq);
	n.left = n1;
	n.right = n2;
	nodes.insert(n);

	while (it != nodes.end())
	{
		n1 = const_cast<Node*>(&(*it));
		it++;
		n2 = const_cast<Node*>(&(*it));
		it++;
		n.ch = n1->ch + n2->ch;
		n.freq = n1->freq + n2->freq;
		n.left = n1;
		n.right = n2;
		nodes.insert(n);
	}

	it = nodes.end();
	it--;
	return const_cast<Node*>(&(*it));
}

void encoder::findBitSeq(Node *head, std::set<Node> &nodes, std::string &bitSeqTmp)
{
	if (head->left != NULL)
	{
		bitSeqTmp += '0';
		findBitSeq(head->left, nodes, bitSeqTmp);
	}

	if (head->right != NULL)
	{
		bitSeqTmp += '1';
		findBitSeq(head->right, nodes, bitSeqTmp);
	}

	if (head->left == NULL && head->right == NULL)
	{
		head->bitSeq = bitSeqTmp;
	}
	bitSeqTmp = bitSeqTmp.substr(0, bitSeqTmp.size() - 1);

	return;
}