#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <fstream>
#include <cstring>
#include <cmath>
#include <random>
#include <ctime>
#include <climits>
#include <string>
#include <unordered_map>

using namespace std;

#define COINSIZE 100

class tweet{
public:
	int user_id;
	int tweet_id;
	double score;
	int	flags[COINSIZE];
	tweet(int,int,double,int[]);
};



