//2016112154 ������
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
	std::cout << "�ɸ��ð� :" << (double)(end - start) << std::endl;
	test.compare();
	test.makeText();


	
}