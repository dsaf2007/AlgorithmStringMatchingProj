//2016112154 ������
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

	std::cout << "�ɸ��ð� :" << test.elapse_time << std::endl;
	test.exec_compare();
	endx = time(NULL);
	std::cout << "�� �ɸ� �ð� : " << (double)(endx - startx) << "\n";
	test.makeText();


	
}