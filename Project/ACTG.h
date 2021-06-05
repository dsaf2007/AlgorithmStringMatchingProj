#pragma once
#include <iostream>
#include <random>
#include <ctime>
#include <fstream>
#include <mutex>
#include <thread>
#include <future>
#include <cstdlib>

class ACTG
{
public:
	ACTG(int k_,int n_);
	~ACTG();

	void initMyDNA(int x);
	void exec_initMyDNA();
	void makeShortread();
	void exec_makeShortread();
	char random();
	void restore();
	void compare(int x);
	void makeText();
	void printSizeInfo();
	void BMRestore(int x);
	void execute();
	void exec_compare();
	time_t start, end;
	double elapse_time;
	int miss;
private:
	std::string ref_DNA_seq;
	std::string my_DNA_seq;
	std::string restore_seq;
	std::string actg = "ACTG";
	int k;
	int M;
	int N;
	std::vector<std::string> short_read;
	std::vector<std::thread> threads;	

};