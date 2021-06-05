//2016112154 정동구
#include <iostream>
#include <vector>
#include <cstdlib>
#include "ACTG.h"
#include <thread>

int main()
{
	

	time_t startx, endx;
	startx = time(NULL);
	ACTG test(30, 40000);
	test.printSizeInfo();
	test.exec_initMyDNA();
	test.makeShortread();

	test.execute();

	std::cout << "걸린시간 :" << test.elapse_time << std::endl;
	test.exec_compare();
	endx = time(NULL);
	std::cout << "총 걸린 시간 : " << (double)(endx - startx) << "\n";
	test.makeText();


	
}