#include "pch.h"
#include "decoder.h"
#include <iostream>
#include <string>
#include <fstream>

decoder::decoder(std::string filePath) :
	bitcount(8)
{
	if (decode(filePath))
		puts("Decode successful.");
	else
		puts("Decode failed.");
}

bool decoder::decode(std::string filePath)
{
	std::ifstream inf;
	inf.open(filePath, std::ios::binary);
	inf.read(&acc, sizeof(char));
	int i = 0;
	while (!inf.eof())
	{
		int bit = readbit(inf);
		char ch = '0' + bit;
		hufS.push_back(ch);
		i++;
	}
	inf.close();

	for (size_t i = 0; i < 4; i++)
		filePath.pop_back();
	
	filePath.append(".txt");
	inf.open(filePath);
	int lastBit;
	inf >> lastBit;
	//std::cout << lastBit << std::endl;
	for (int i = 0; i < 8 - lastBit + 1; i++)
	{
		hufS.pop_back();
	}
	std::string s;
	int dictSize;
	inf >> dictSize;
	//std::cout << dictSize << std::endl;
	std::getline(inf, s);
	for (int i = 0; i < dictSize; i++)
	{
		std::getline(inf, s);
		if (s.empty())
		{
			i--;
			continue;
		}
		else {
			if (s[0] == ':' && s[1] != ':')
				dict.insert(std::pair<char, std::string>(10, s.substr(1, s.size())));
			else
				dict.insert(std::pair<char, std::string>(s[0], s.substr(2, s.size())));
		}
	}
	//for (auto i : dict)
	//	std::cout << i.first << ":" << i.second << std::endl;
	inf.close();

	std::string str;

	//std::cout << hufS << std::endl;

	for (auto i : hufS)
	{
		str.append(sizeof(char), i);
		char ch = checkDict(str);
		if (ch)
		{
			textS.append(sizeof(char), ch);
			str.clear();
		}
	}

	//std::cout << textS << std::endl;

	std::ofstream outf;
	outf.open("text_decode.txt");
	outf << textS;

	std::fstream f;
	f.open("huf_text.txt");
	if (f.fail())
		return false;
	else
		return true;
}

char decoder::checkDict(std::string str)
{
	bool flag = false;
	std::map<char, std::string>::iterator it;
	it = dict.begin();

	/*auto it = std::find(dict.begin(), dict.end(), [&str](auto e)
	{
		return c.second == str;
	});
	if (it != dict.end())
	{

	}*/


	while (!flag && it != dict.end())
	{
		if (str == it->second)
			flag = true;
		it++;
	}

	if (flag)
		return (--it)->first;
	else
		return NULL;
}

int decoder::readbit(std::ifstream &inf)
{
	if (bitcount == 0)
	{
		bitcount = 8;
		inf.read(&acc, sizeof(char));
	}
	--bitcount;
	return (acc >> (7 - bitcount)) & 1;
}