#pragma once
#include <iostream>
#include <vector>

int toInt(char a)//알파벳 인덱싱
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

std::vector<int> makeBad_table(std::string pattern_str)//불일치 문자 테이블
{
	int pattern_size = pattern_str.size();
	std::vector<int> bad(5, pattern_size);

	for (int i = 0; i < pattern_size; i++)
	{
		bad[toInt(pattern_str[i])] = i;
	}

	return bad;
}



std::vector<int> makeGoddsuffix_table(std::string pattern_str)//일치 접미부 테이블
{
	int pattern_size = pattern_str.size();

	int pattern_pt = pattern_size; // pattern point
	int suffix_pt = pattern_pt + 1; // suffir point
	
	std::vector<int> suffix_table(pattern_size + 1, 0); //접미사와 문자열 체크
	suffix_table[pattern_pt] = suffix_pt;

	std::vector<int> skip_table(pattern_size + 1, 0);//skip table

	while (pattern_pt > 0)
	{
		
		while (suffix_pt <= pattern_size && //일치하는 접미사 개수 측정
			pattern_str[pattern_pt - 1] != pattern_str[suffix_pt - 1])
		{
			if (skip_table[suffix_pt] == 0)
			{
				skip_table[suffix_pt] == suffix_pt - pattern_pt;
			}
			suffix_pt = suffix_table[suffix_pt];
		}
		suffix_table[--pattern_pt] = --suffix_pt; //일치 문자열이 suffix의 index를 가리키도록
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

int search(std::vector<int> bad_table, std::vector<int> good_suffix_table, std::string plain_text, std::string pattern_str)//문자열 비교
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
		while(match >0 && miss_match <5)//miss match 4개 허용
		{
			if (match > 0 && pattern_str[match - 1] == plain_text[begin + (match - 1)])
				match--;
			else
				miss_match++;

		}
			if (match == 0) // 문자열 일치하는 경우
				return begin; //일치 index 반환
			if (bad_table[toInt(plain_text[begin + match])] != pattern_size)
			{
				//불일치 문자 방책 다음 위치 = 본문 문자열 내 현재 index(begin) + (패턴 문자열 내 현재위치 - 테이블에서 참조 한 값)
				begin += std::max(match - bad_table[toInt(plain_text[begin + match])], good_suffix_table[match]);//일치 접미부와 불일치 문자 방책 중 더 많이 이동할 수 있는 방식으로 이동
			}
			else // 불일치 문자가 패턴보다 뒤쪽에 있는 경우 
			{
				begin += std::max(good_suffix_table[match], match);
			}
	}
	return -1;
}