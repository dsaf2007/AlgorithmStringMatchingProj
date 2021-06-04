//2016112154 정동구
#include <iostream>
#include <vector>
#include <cstdlib>
#include "ACTG.h"


int main()
{
	

	time_t start, end;

	ACTG test(30, 40000);
	test.printSizeInfo();
	test.init();

	start = time(NULL);
	test.BMRestore();
	end = time(NULL);
	std::cout << "걸린시간 :" << (double)(end - start) << std::endl;
	test.compare();
	test.makeText();


	
}