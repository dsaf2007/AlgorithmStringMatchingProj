#pragma once
#include <iostream>
#include <vector>

int toInt(char a)//���ĺ� �ε���
{

	if (a == 'A')
		return 0;
	else if (a == 'C')
		return 1;
	else if (a == 'T')
		return 2;
	else if (a == 'G')
		return 3;
	else
		return 4;
}

std::vector<int> makeBad_table(std::string pattern_str)//����ġ ���� ���̺�
{
	int pattern_size = pattern_str.size();
	std::vector<int> bad(5, pattern_size);

	for (int i = 0; i < pattern_size; i++)
	{
		bad[toInt(pattern_str[i])] = i;
	}

	return bad;
}



std::vector<int> makeGoddsuffix_table(std::string pattern_str)//��ġ ���̺� ���̺�
{
	int pattern_size = pattern_str.size();

	int pattern_pt = pattern_size; // pattern point
	int suffix_pt = pattern_pt + 1; // suffir point
	
	std::vector<int> suffix_table(pattern_size + 1, 0); //���̻�� ���ڿ� üũ
	suffix_table[pattern_pt] = suffix_pt;

	std::vector<int> skip_table(pattern_size + 1, 0);//skip table

	while (pattern_pt > 0)
	{
		
		while (suffix_pt <= pattern_size && //��ġ�ϴ� ���̻� ���� ����
			pattern_str[pattern_pt - 1] != pattern_str[suffix_pt - 1])
		{
			if (skip_table[suffix_pt] == 0)
			{
				skip_table[suffix_pt] == suffix_pt - pattern_pt;
			}
			suffix_pt = suffix_table[suffix_pt];
		}
		suffix_table[--pattern_pt] = --suffix_pt; //��ġ ���ڿ��� suffix�� index�� ����Ű����
	}

	suffix_pt = suffix_table[0];

	while (pattern_pt < pattern_size)
	{
		if (skip_table[pattern_pt] == 0)
			skip_table[pattern_pt] = suffix_pt;
		if (pattern_pt++ == suffix_pt)
			suffix_pt = suffix_table[suffix_pt];
	}
	return skip_table;
}

int search(std::vector<int> bad_table, std::vector<int> good_suffix_table, std::string plain_text, std::string pattern_str)//���ڿ� ��
{
	int plain_size = plain_text.size();
	int pattern_size = pattern_str.size();

	int begin = 0;

	if (plain_size < pattern_size)
		return -1;

	while (begin <= plain_size - pattern_size)
	{
		int match = pattern_size;
		int miss_match = 0;
		while(match >0 && miss_match <5)//miss match 4�� ���
		{
			if (match > 0 && pattern_str[match - 1] == plain_text[begin + (match - 1)])
				match--;
			else
				miss_match++;

		}
			if (match == 0) // ���ڿ� ��ġ�ϴ� ���
				return begin; //��ġ index ��ȯ
			if (bad_table[toInt(plain_text[begin + match])] != pattern_size)
			{
				//����ġ ���� ��å ���� ��ġ = ���� ���ڿ� �� ���� index(begin) + (���� ���ڿ� �� ������ġ - ���̺��� ���� �� ��)
				begin += std::max(match - bad_table[toInt(plain_text[begin + match])], good_suffix_table[match]);//��ġ ���̺ο� ����ġ ���� ��å �� �� ���� �̵��� �� �ִ� ������� �̵�
			}
			else // ����ġ ���ڰ� ���Ϻ��� ���ʿ� �ִ� ��� 
			{
				begin += std::max(good_suffix_table[match], match);
			}
	}
	return -1;
}