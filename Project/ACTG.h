#include <iostream>
#include <random>
#include <ctime>
#include <fstream>

class ACTG
{
public:
	ACTG(int k_,int n_);
	~ACTG();

	void init();
	char random();
	void restore();
	void compare();
	void makeText();
	void printSizeInfo();
	void BMRestore();

private:
	std::string ref_DNA_seq;
	std::string my_DNA_seq;
	std::string restore_seq;
	std::string actg = "ACTG";
	int k;
	int n;
	std::vector<std::string> short_read;
};