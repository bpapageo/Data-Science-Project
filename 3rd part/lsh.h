#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <fstream>
#include <cstring>
#include <cmath>
#include <random>
#include <ctime>
#include <unordered_map>
#include <algorithm>



#define M 4294967291 
#define w 400


using namespace std;

class myvectorlsh{
public:
	char* nameid;
	int id;
	int* points;
	long double* pointsd;
	long double* Cpointsd;
	int d;
	int flag;
	bool type;
	long double dis;
	long double dis2;
	long double mid;
	myvectorlsh(char*,int*,int,bool);
	myvectorlsh(int,long double*,int,bool);
	~myvectorlsh();
};

struct ListlshNode{
	myvectorlsh* ptr;
	ListlshNode* next;
	int *g;
	ListlshNode();
	~ListlshNode();
};

class Listlsh{
private:
	int size;
public:
	ListlshNode* head;
	Listlsh();
	~Listlsh();
    void insert(myvectorlsh*,int*);
    void insertend(myvectorlsh*);
    bool is_empty();
    void display();
    int length();
	myvectorlsh* search(myvectorlsh*,bool,int,int*);
	myvectorlsh* exhaustedsearch(myvectorlsh*,bool,int);
	void Rangesearch(myvectorlsh*,int,long double,int,int*,int,myvectorlsh**);
	void LSHRangesearch(myvectorlsh*,bool,int R,Listlsh*,int,int*);

};

class hashtable{
	int size;
	int K;
	int dim;
	Listlsh** list;
	myvectorlsh** v;
	long double* t;
	int* r;
public:
	hashtable(int,int,int,bool);
	~hashtable();
	int* hashfunction(myvectorlsh*,bool);
	void insert(myvectorlsh*,bool);
	myvectorlsh* search(myvectorlsh*,bool);
	void Rangesearch(myvectorlsh*,int,long double,int,myvectorlsh**);
    void LSHRangesearch(myvectorlsh*,int,long double,Listlsh*);
	void hashprint();
};

long double euclidean(long double*,long double*,int);

long double cosine(long double*,long double*,int);

long double inner_product(myvectorlsh*,myvectorlsh*);

long double* normal_distribute(int);

void LSHRecommendation(hashtable**,myvectorlsh**,int,string[],ofstream&,int);

void Validation(myvectorlsh**,int);

void Qsort( long double*,int*,string[], int, int);

int partition( long double*,int*,string[], int, int);




