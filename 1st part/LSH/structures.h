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

class myvector{
public:
	char* nameid;
	int* points;
	double* pointsd;
	int d;
	bool type;
	double dis;
	myvector(char*,int*,int,bool);
	myvector(char*,double*,int,bool);
	~myvector();
};

struct listnode{
	myvector* ptr;
	listnode* next;
	int *g;
	listnode();
	~listnode();
};

class List{
private:
	listnode* head;
	int size;
public:
	List();
	~List();
    void insert(myvector*,int*);
    void insertend(myvector*);
    bool is_empty();
    void display();
    int length();
	myvector* search(myvector*,bool,int,int*);
	myvector* exhaustedsearch(myvector*,bool,int);
	void Rangesearch(myvector*,bool,int,ofstream&,int,int*);

};

class hashtable{
	int size;
	int K;
	int dim;
	List** list;
	myvector** v;
	double* t;
	int* r;
public:
	hashtable(int,int,int,bool);
	~hashtable();
	int* hashfunction(myvector*,bool);
	void insert(myvector*,bool);
	myvector* search(myvector*,bool);
	void Rangesearch(myvector*,bool,int,ofstream&);
	void hashprint();
};

double euclidean_dist(int*,int*,int);

double cosine_similarity(int*,int*,int);

double inner_product(myvector*,myvector*);

double* normal_distribute(int);


