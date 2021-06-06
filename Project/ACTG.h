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
	ACTG(int l_,int n_);
	~ACTG();

	void initMyDNA(int x);
	void exec_initMyDNA();
	void makeShortread();
	char random();
	void restore(int x);
	void exec_restore();
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
	int L;//length of short read
	int M;//number of reads
	int N;//length of DNA
	std::vector<std::string> short_read;
	std::vector<std::thread> threads;	

};