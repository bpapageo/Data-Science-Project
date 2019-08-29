#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <fstream>
#include <cstring>
#include <cmath>
#include <random>
#include <ctime>


#define M 4294967291 
#define w 400


using namespace std;

class myvectorlsh{
public:
	char* nameid;
	int id;
	int* points;
	long double* pointsd;
	int d;
	int flag;
	bool type;
	long double dis;
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
	void hashprint();
};

long double euclidean(long double*,long double*,int);

long double cosine(long double*,long double*,int);

long double inner_product(myvectorlsh*,myvectorlsh*);

long double* normal_distribute(int);


