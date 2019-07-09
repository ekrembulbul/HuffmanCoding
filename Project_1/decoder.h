#pragma once
#include <string>
#include <map>

class decoder
{
private:
	char acc;
	int bitcount;
	std::string hufS;
	std::string textS;
	std::map<char, std::string> dict;

	bool decode(std::string filePath);
	int readbit(std::ifstream &inf);
	char checkDict(std::string str);

public:
	decoder(std::string filePath);
	~decoder() = default;
};

