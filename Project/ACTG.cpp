#pragma once
#include "ACTG.h"
#include "BoyerMoor.h"
#include <time.h>

ACTG::ACTG(int k_,int m_)//������
{
	M = m_;
	k = k_;
	N = 1000000;
	miss = 0;
	for (int i = 0; i < N; i++)
	{
		ref_DNA_seq += random();
	}
	my_DNA_seq = ref_DNA_seq;
	restore_seq = ref_DNA_seq;
}

ACTG::~ACTG()
{
	
}

void ACTG::initMyDNA(int x)//short read�� �����Ѵ�.
{
	for (int i = (x)*(my_DNA_seq.length()/20); i < ((x+1) * (my_DNA_seq.length() / 20))-k; i += k)
	{
		my_DNA_seq[i + rand() % k] = random();
		my_DNA_seq[i + rand() % k] = random();
	}
	
}

void ACTG::exec_initMyDNA()//��Ƽ�������� �����ϱ� ���� �Լ�
{
	std::vector<std::thread> My;

	for (int i = 0; i < 20; i++)
	{
		My.emplace_back(std::thread(&ACTG::initMyDNA, this, i));
	}
	for (auto& init : My)
		init.join();
}

void ACTG::makeShortread()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dis(0, my_DNA_seq.length() - k - 1);
	std::mutex g_lock;

	for (int i = dis(gen); i < my_DNA_seq.length(); i = dis(gen))
	{
		std::string read_str = "";
		for (int j = 0; j < k; j++)
		{
			read_str += my_DNA_seq[i + j];
		}
		short_read.push_back(read_str);
		if (short_read.size() == M)
			break;
	}
}


char ACTG::random()//random characters
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dis(0, 3);

	return actg[dis(gen)];
}


void ACTG::restore()//brute force
{
	restore_seq = ref_DNA_seq;
	int mismatch;
	for (int i = 0; i < short_read.size(); i++)
	{
		for (int j = 0; j < N; j++)
		{
			mismatch = 0;
			for (int x = 0; x < k; x++)
			{
				if (short_read[i][x] != ref_DNA_seq[j + x])
					mismatch++;
				if (mismatch >= 4)
					break;
				if ((x == k - 1) && mismatch < 2)
				{
					for (int y = 0; y < k; y++)
					{
						restore_seq[j + y] = short_read[i][y];
					}
				}
			}
		}
	}
}

void ACTG::BMRestore(int x)//Boyer-Moor �˰����� �̿��� ���ڿ� ����
{
	for(int i = (x) * (M / 20); i < (x+1)*(M/20) ; i++)
	{
		std::vector<int> bad_table = makeBad_table(short_read[i]);
		std::vector<int> suffix_table = makeGoddsuffix_table(short_read[i]);
		int index = search(bad_table, suffix_table, restore_seq, short_read[i]);
		if (index > -1)
		{
		
			for (int j = 0; j < short_read[i].length(); j++)
			{
				restore_seq[index + j] = short_read[i][j];
			}
		}
	}
}

void ACTG::execute()//BMRestore�� multi threading �ϱ� ���� �Լ�
{
	std::cout << short_read.size() << std::endl;
	start = time(NULL);
	for (int i = 0; i < 20; i++)
	{
		threads.emplace_back(std::thread(&ACTG::BMRestore, this, i));
	}
	for (auto& thread : threads)
		thread.join();
	end = time(NULL);

	elapse_time = (double)(end - start);


}

void ACTG::compare(int x)//My_DNA�� ������ DNA ��
{

	for (int i = (x)*(N/20); i < (x+1)*(N/20); i++)
	{
		if (restore_seq[i] != my_DNA_seq[i])
		{
			miss++;
		}
		
	}

}

void ACTG::exec_compare()//compare ����ó��
{
	std::vector<std::thread> comp;

	for (int i = 0; i < 20; i++)
	{
		comp.emplace_back(std::thread(&ACTG::compare, this, i));
	}
	for (auto& compare : comp)
		compare.join();
}


void ACTG::makeText()//��� ���
{

	std::ofstream writeResult("result.txt");
	writeResult << "�ҿ� �ð� : " << elapse_time<<"\n";
	writeResult << "��ġ�� : " << ((double)((N - miss) / (double)N)) * 100 << "%\n";
	writeResult << "����ġ ���� ���� : " << miss << std::endl;
	std::ofstream writeShortRead("short_read.txt");
	std::ofstream myDNA("my_DNA.txt");
	std::ofstream restore("restore_seq.txt");
	for (int i = 0; i < short_read.size(); i++)
	{
		writeShortRead << short_read[i]<<"\n";
	}
	myDNA << my_DNA_seq;
	restore << restore_seq;
}

void ACTG::printSizeInfo()
{
	std::cout << "number of short read : " << M << std::endl << "length of short read : " << k << std::endl;
}