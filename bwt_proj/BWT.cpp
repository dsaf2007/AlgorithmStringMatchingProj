#include <iostream>
#include <algorithm>
#include <vector>
#include <cstdlib>
#include <string>
#include <fstream>
#include <time.h>

using namespace std;

string readFile();
int *BWT(char **, int *, int[][2], int *);
void sort(int, vector<int> &, vector<int> &);
void suffix(string, int *, char *, char *);
string reconstruct(string reference, char **bwt_t, int *index, int suffixtable[][2], int c, int *suffixIndex);
void result(string);

string s;
string t;
string refPath = "reference.txt";
string srPath = "shortRead.txt";
string myPath = "MyDNA.txt";
string resultPath = "result.txt";

int N;     // reference 길이
int D = 4; // missmatch 허용 개수

int main(void)
{
    string reference;
    int *index;
    int c = 0;

    // 걸리는 시간 측정
    time_t start, end;
    double total_time;
    start = time(NULL);

    // read reference
    reference = readFile();
    N = reference.size();
    index = new int[N];
    char *first_c = new char[N];
    char *last_c = new char[N];
    int suffixtable[5][2];
    int *suffixIndex = new int[N];
    char **bwt_t = new char *[2];
    for (int i = 0; i < 2; i++)
    {
        bwt_t[i] = new char[N];
    }

    // BWT
    suffix(reference, index, first_c, last_c);

    // reconstruct
    index = BWT(bwt_t, index, suffixtable, suffixIndex);
    reference = reconstruct(reference, bwt_t, index, suffixtable, c, suffixIndex);

    // time
    end = time(NULL);
    total_time = (double)(end - start) / CLOCKS_PER_SEC;
    cout << "총 걸린 시간 : " << total_time << "sec" << endl;

    return 0;
}

// read reference file
string readFile(void)
{
    string reference;
    ifstream refFile(refPath.data());

    // reference 문자열에 저장, 마지막에 $붙이기
    if (refFile.is_open())
    {
        while (getline(refFile, reference))
            ;
        reference.append(1, '$');
    }
    refFile.close();
    cout << "file read" << endl;
    return reference;
}

// suffix 만들기
void suffix(string reference, int *index, char *first_c, char *last_c)
{
    vector<int> mmap(N);
    vector<int> pre_mmap(N + 1);

    // 한 글자씩 밀어가면서 저장 - last-first mapping
    for (int i = 0; i < N; i++)
    {
        first_c[i] = reference[i];
        if (i)
            last_c[i] = reference[i - 1];
        else
            last_c[i] = reference[N - 1];
    }

    // 문자열 suffix array(접미사 배열) 만들기
    for (int i = 0; i < N; i++)
        mmap[i] = i;
    for (int i = 0; i < N; i++)
        pre_mmap[i] = reference[i];

    for (int n = 1; n < N; n *= 2)
    {
        // 정렬
        sort(n, mmap, pre_mmap);
        sort(0, mmap, pre_mmap);

        vector<int> new_mmap(N + 1);
        new_mmap[mmap[0]] = 0;
        for (int i = 1; i < N; i++)
        {
            if (pre_mmap[mmap[i]] == pre_mmap[mmap[i - 1]] && pre_mmap[mmap[i] + n] == pre_mmap[mmap[i - 1] + n])
                new_mmap[mmap[i]] = new_mmap[mmap[i - 1]];
            else
                new_mmap[mmap[i]] = new_mmap[mmap[i - 1]] + 1;
        }

        swap(pre_mmap, new_mmap);
        if (pre_mmap[mmap[N - 1]] == N - 1)
            break;
    }
    for (int i = N - 1; i >= 0; i--)
    {
        index[i] = mmap[N - i - 1];
        t.append(1, first_c[index[i]]);
        s.append(1, last_c[index[i]]);
    }
    delete[] first_c;
    delete[] last_c;
    cout << "make suffix" << endl;
}

void sort(int n, vector<int> &mmap, vector<int> &pre_mmap)
{
    // 계수 정렬
    int x = mmap.size();
    int r = max(x, (int)numeric_limits<char>::max());

    vector<int> vec(r + 1, 0);
    for (int i = 0; i < x; i++)
    {
        if (i + n < x)
        {
            vec[pre_mmap[i + n]]++;
        }
        else
        {
            vec[0]++;
        }
    }
    for (int i = 1; i <= r; i++)
    {
        vec[i] += vec[i - 1];
    }

    vector<int> new_map(x);
    for (int i = x; i >= 0; i--)
    {
        if (mmap[i] + n < x)
        {
            new_map[--vec[pre_mmap[mmap[i] + n]]] = mmap[i];
        }
        else
        {
            new_map[--vec[0]] = mmap[i];
        }
    }
    swap(mmap, new_map);
}

int *BWT(char **bwt_t, int *index, int suffixtable[][2], int *suffixIndex)
{
    // BWT 만들기

    int length = s.size();
    int *count = new int[length];
    int i = 0;
    int temp = 0;
    int X;
    int A;
    int C;
    int G;
    int T;

    // index 정렬
    for (int j = length - 1; j >= 0; j--)
        count[j] = index[length - (j + 1)];

    for (i = 0; i < length; i++)
    {
        bwt_t[0][i] = t[i];

        if (i)
        {
            if (bwt_t[0][i - 1] != bwt_t[0][i])
            {
                suffixtable[temp][0] = i;
                if (temp < 5)
                {
                    suffixtable[temp - 1][1] = i - 1;
                    temp++;
                }
            }
        }
        else
        {
            suffixtable[temp][0] = 0;
            temp++;
        }
    }
    suffixtable[temp - 1][1] = i - 1;

    cout << "start bwt" << endl;

    // suffix index 만들기
    X = suffixtable[0][0];
    A = suffixtable[1][0];
    C = suffixtable[2][0];
    G = suffixtable[3][0];
    T = suffixtable[4][0];

    for (int j = 0; j < length; j++)
    {
        bwt_t[1][j] = s[j];
        if (bwt_t[1][j] == '$')
        {
            suffixIndex[j] = X;
            X++;
        }
        else if (bwt_t[1][j] == 'A')
        {
            suffixIndex[j] = A;
            A++;
        }
        else if (bwt_t[1][j] == 'C')
        {
            suffixIndex[j] = C;
            C++;
        }
        else if (bwt_t[1][j] == 'G')
        {
            suffixIndex[j] = G;
            G++;
        }
        else
        {
            suffixIndex[j] = T;
            T++;
        }
    }
    delete[] index;
    cout << "end bwt" << endl;

    return count;
}

// 복원
string reconstruct(string reference, char **bwt_t, int *index, int suffixtable[][2], int c, int *suffixIndex)
{
    int flag;
    ifstream srFile(srPath.data());
    string shortread;
    if (srFile.is_open())
    {
        cout << "start reconstruct" << endl;

        while (getline(srFile, shortread))
        {
            c = shortread.size();

            int p = 1;
            int q = 1;
            int r = -1;
            int front = 0;
            int end = 0;
            int frontIn = -1;
            int endIn = -1;
            int miss[2] = {0, 0};

            if (shortread[c - 1] == '$')
                flag = 0;
            else if (shortread[c - 1] == 'A')
                flag = 1;
            else if (shortread[c - 1] == 'C')
                flag = 2;
            else if (shortread[c - 1] == 'G')
                flag = 3;
            else // 'T'
                flag = 4;

            front = suffixtable[flag][0];
            end = suffixtable[flag][1];

            while (1)
            {
                if (end - front < 0)
                    if (frontIn == -1 && endIn == -1)
                        break;

                if (front <= end || frontIn != -1)
                {
                    if (c - p >= 0)
                    {
                        if (frontIn == -1)
                        {
                            frontIn = suffixIndex[front];
                        }
                        else
                        {
                            if (bwt_t[0][frontIn] == '$')
                            {
                                p = 0;
                                front++;
                                frontIn = -1;
                                miss[0] = 0;
                            }
                            else
                            {
                                if (bwt_t[0][frontIn] == shortread[c - p])
                                {
                                    if (c - p == 0)
                                    {
                                        // 찾음
                                        r = frontIn;
                                        break;
                                    }

                                    frontIn = suffixIndex[frontIn];
                                }
                                else
                                {
                                    miss[0]++;
                                    // missMatch 허용 횟수 초과
                                    if (miss[0] > D)
                                    {
                                        p = 0;
                                        front++;
                                        frontIn = -1;
                                        miss[0] = 0;
                                    }
                                    else
                                    {
                                        if (c - p == 0)
                                        {
                                            // 찾음
                                            r = frontIn;
                                            break;
                                        }

                                        frontIn = suffixIndex[frontIn];
                                    }
                                }
                            }
                        }
                    }
                    else
                    {
                        p = 0;
                        front++;
                        frontIn = -1;
                        miss[0] = 0;
                    }

                    p++;
                }

                if (end >= front || endIn != -1)
                {
                    if (c - q >= 0)
                    {
                        if (endIn == -1)
                        {
                            endIn = suffixIndex[end];
                        }
                        else
                        {
                            if (bwt_t[0][endIn] == '$')
                            {
                                q = 0;
                                end--;
                                endIn = -1;
                                miss[1] = 0;
                            }
                            else
                            {
                                if (bwt_t[0][endIn] == shortread[c - q])
                                {
                                    // 찾음
                                    if (c - q == 0)
                                    {
                                        r = endIn;
                                        break;
                                    }

                                    endIn = suffixIndex[endIn];
                                }
                                else
                                {
                                    miss[1]++;
                                    // missMatch 허용 횟수 초과
                                    if (miss[1] > D)
                                    {
                                        q = 0;
                                        end--;
                                        endIn = -1;
                                        miss[1] = 0;
                                    }
                                    else
                                    {
                                        if (c - q == 0)
                                        {
                                            // 찾음
                                            r = endIn;
                                            break;
                                        }

                                        endIn = suffixIndex[endIn];
                                    }
                                }
                            }
                        }
                    }
                    else
                    {
                        q = 0;
                        end--;
                        endIn = -1;
                        miss[1] = 0;
                    }

                    q++;
                }
            }

            // reference에 추가
            int here = index[r];
            if (r != -1)
            {
                for (int i = 0; i < c; i++)
                {
                    if (here + i >= N)
                        break;
                    reference[here + i] = shortread[i];
                }
            }
        }
        srFile.close();
        cout << "reconstruct reference" << endl;
    }
    return reference;
}

void result(string reference)
{
    string myGenome;

    int x = 0;
    reference = reference.substr(0, reference.size() - 1); // $ 삭제

    ifstream myFile(myPath.data());
    if (myFile.is_open())
    {
        while (getline(myFile, myGenome))
            ;
        myFile.close();
    }

    for (int i = 0; i < N; i++)
    {
        if (reference[i] == myGenome[i])
            x++;
    }

    cout << "일치율 : " << double(x) / N * 100 << "%" << endl;

    ofstream resultFile(resultPath.data());
    if (resultFile.is_open())
    {
        resultFile << reference;
    }
    resultFile.close();
}