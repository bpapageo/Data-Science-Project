#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <fstream>
#include <cstring>
#include <cmath>
#include <random>
#include <ctime>


#define w 400




using namespace std;

class myvectorcube{
public:
	char* nameid;
	int id;
	int* points;
	long double* pointsd;
	int d;
	int flag;
	bool type;
	long double dis;
	myvectorcube(char*,int*,int,bool);
	myvectorcube(int,long double*,int,bool);
	~myvectorcube();
};

struct listcubenode{
	myvectorcube* ptr;
	listcubenode* next;
	int *g;
	listcubenode();
	~listcubenode();
};

class listcube{
private:
	int size;
public:
	listcubenode* head;
	listcube();
	~listcube();
    void insert(myvectorcube*,int*);
    void insertend(myvectorcube*);
    bool is_empty();
    void display();
    int length();
	myvectorcube* search(myvectorcube*,bool,int,int*);
	myvectorcube* exhaustedsearch(myvectorcube*,bool,int);
	void Rangesearch(myvectorcube*,int,long double,int,int*,int,myvectorcube**);

};

class cube{
	int size;
	int K;
	int dim;
	int probes;
	listcube** list;
	myvectorcube** v;
	long double* t;
	int* r;
public:
	cube(int,int,int,bool,int);
	~cube();
	int* hashfunction(myvectorcube*,bool);
	void insert(myvectorcube*,bool);
	myvectorcube* search(myvectorcube*,bool);
	void Rangesearch(myvectorcube*,int,long double,int,myvectorcube**);
	int* Nextprobes(int);
	void hashprint();
};

long double euc(long double*,long double*,int);

long double cos(long double*,long double*,int);

long double inner_product(myvectorcube*,myvectorcube*);

int HammingDist(int,int);


