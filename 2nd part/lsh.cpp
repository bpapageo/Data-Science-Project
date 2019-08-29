#include "lsh.h"

default_random_engine generator;
normal_distribution<long double> distribution(0.0,1.0);

myvectorlsh::myvectorlsh(char* name,int* pts,int dim,bool t){
		type=t;
		d=dim;
		nameid= new char[strlen(name)+1];
		strcpy(nameid,name);
		points= new int[dim];
		for(int i=0;i<dim;i++){
				points[i]=pts[i];
		}
		/*cout<<"created myvectorlsh name "<<nameid<<" with dim "<<dim<<endl;
		cout<<"Points:"<<endl;
		for(int i=0;i<dim;i++){
			cout<<points[i]<<" ";
		}	
		cout<<endl;	*/

}


myvectorlsh::myvectorlsh(int num,long double* pts,int dim,bool t){
		type=t;
		d=dim;
		id=num;
		flag=-1;
		pointsd= new long double[dim];
		for(int i=0;i<dim;i++){
				pointsd[i]=pts[i];

		}
		/*cout<<"created myvectorlsh name "<<nameid<<" with dim "<<dim<<endl;
		cout<<"Points:"<<endl;
		for(int i=0;i<dim;i++){
			cout<<points[i]<<" ";
		}	
		cout<<endl;	*/

}

myvectorlsh::~myvectorlsh(){
	delete []nameid;
	if(type==0){
		delete []points;
	}
	else{
		delete []pointsd;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


ListlshNode::ListlshNode(){
	next=NULL;
	ptr=NULL;
	g=NULL;
}

ListlshNode::~ListlshNode(){
	if(g!=NULL)
	delete []g;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Listlsh::Listlsh(){
	head=NULL;
	size=0;
}

Listlsh::~Listlsh(){
	ListlshNode* current = head;
	while( current != NULL) {
    	ListlshNode* next = current->next;
    	current->ptr=NULL;
    	delete current;
    	current = next;
	}
	head = NULL;
}

void Listlsh::insert(myvectorlsh* nodept,int* g){
	ListlshNode* temp = new ListlshNode();
	temp->ptr=nodept;
	temp->g=g;
	if(head==NULL){
		head=temp;
	}
	else{
		temp->next=head;
		head=temp;
	}
	size++;
		
}

void Listlsh::insertend(myvectorlsh* nodept){
	ListlshNode* temp = new ListlshNode();
	temp->ptr=nodept;
	if(head==NULL){
		head=temp;
	}
	else{
			ListlshNode* current = head;
			while(current->next != NULL){
				current=current->next;
			}
			current -> next = temp;

	}
	size++;
		
}

bool Listlsh::is_empty(){
	if (size<=0){
		return true;
	}
	return false;
}

void Listlsh::display(){
	ListlshNode* temp = head;
	while(temp!=NULL){
		cout<<temp->ptr->nameid<<" ";
		for(int i=0;i<temp->ptr->d;i++){
			cout<<temp->ptr->points[i]<<" ";
		}
		cout<<endl;
		temp=temp->next;
	}

}

int Listlsh::length(){
	return size;
}

myvectorlsh* Listlsh::search(myvectorlsh* pt,bool metric,int K,int* g){
	long double min=100000;
	int flag=0;
	myvectorlsh *vec=NULL;
	ListlshNode* temp = head;
	while(temp!=NULL){
		if(metric==0){
			for(int i=0;i<K;i++){
				if(g[i]!=(temp->g[i])){
					flag=1;
					break;
				}
			}
			if((euclidean(pt->pointsd,temp->ptr->pointsd,temp->ptr->d)<min)&&(flag==0)){
				min=euclidean(pt->pointsd,temp->ptr->pointsd,temp->ptr->d);
				vec=temp->ptr;
			}
			temp=temp->next;
		}
		else{
			if((cosine(pt->pointsd,temp->ptr->pointsd,temp->ptr->d)<min)&&(flag==0)){
				min=cosine(pt->pointsd,temp->ptr->pointsd,temp->ptr->d);
				vec=temp->ptr;
			}
			temp=temp->next;
		}
	}
	//cout<<min<<endl;
	if(vec!=NULL)vec->dis=min;
	return vec;

}
myvectorlsh* Listlsh::exhaustedsearch(myvectorlsh* pt,bool metric,int K){
	long double min=100000;
	myvectorlsh *vec=NULL;
	ListlshNode* temp = head;
	while(temp!=NULL){
		if(metric==0){
			if(euclidean(pt->pointsd,temp->ptr->pointsd,temp->ptr->d)<min){
				min=euclidean(pt->pointsd,temp->ptr->pointsd,temp->ptr->d);
				vec=temp->ptr;
			}
			temp=temp->next;
		}
		else{
			if(cosine(pt->pointsd,temp->ptr->pointsd,temp->ptr->d)<min){
				min=cosine(pt->pointsd,temp->ptr->pointsd,temp->ptr->d);
				vec=temp->ptr;
			}
			temp=temp->next;
		}
	}
	//cout<<min<<endl;
	if(vec!=NULL)vec->dis=min;
	return vec;

}

void Listlsh::Rangesearch(myvectorlsh* pt,int metric,long double R,int K,int* g,int j,myvectorlsh** centroids){
	//cout<<R<<endl;
	myvectorlsh *vec=NULL;
	int flag=0;
	//Listlsh* result=new Listlsh();
	ListlshNode* temp = head;
	while(temp!=NULL){
		if(metric==0){
			for(int i=0;i<K;i++){
				if(g[i]!=(temp->g[i])){
					flag=1;
					break;
				}
			}
			long double d=euclidean(pt->pointsd,temp->ptr->pointsd,temp->ptr->d);
			if((d<R)&&(flag==0)&&(temp->ptr->flag==-1)){
				vec=temp->ptr;
				vec->flag=j;
				vec->dis=d;
				//result->insert(vec,temp->g);
			}
			else if((d<R)&&(flag==0)&&(temp->ptr->flag!=-1)){
				vec=temp->ptr;
				if(d<vec->dis){
					vec->flag=j;
					vec->dis=d;
					//result->insert(vec,temp->g);
				}

			}
			temp=temp->next;
		}
		else{
			long double d=cosine(pt->pointsd,temp->ptr->pointsd,temp->ptr->d);
			if((d<R)&&(flag==0)&&(temp->ptr->flag==-1)){
				vec=temp->ptr;
				vec->flag=j;
				vec->dis=d;
				//result->insert(vec,temp->g);
			}
			else if((d<R)&&(flag==0)&&(temp->ptr->flag!=-1)){
				vec=temp->ptr;
				if(d<vec->dis){
					vec->flag=j;
					vec->dis=d;
					//result->insert(vec,temp->g);
				}

			}
			temp=temp->next;
		}
	}

	//return result;

}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int* hashtable:: hashfunction(myvectorlsh* p,bool metric){
	if(metric==0){
		int* g=new int[K+1];
		long long f=0;
		for(int i=0;i<K;i++){
			g[i]=floor((inner_product(p,v[i])+t[i])/w);
			f+=(r[i]*g[i]);
		}
		//delete []g;
		//cout<<"k"<<((f%M+M)%M)%size<<endl;
		g[K]=((f%M+M)%M)%size;
		return g;
		//return ((f%M+M)%M)%size;///ΦΦΦΦ
	}
	else if(metric==1){
		int* g=new int[K+1];
		for(int i=0;i<K;i++){
			//cout<<inner_product(p,v[i])<<endl;
			if(inner_product(p,v[i])<0){
				g[i]=0;
			}
			else{
				g[i]=1;
			}
		}

		long sum =0;
		for(int i=K-1;i>=0;i--){
			if(g[i]==1){
				sum+=pow(2.0,K-i-1);
			}
		}

		//delete []g;
		//cout<<sum<<endl;
		g[K]=sum;
		return g;
		//return sum;

	}
}



hashtable::hashtable(int s,int k,int d,bool metric):size(s),K(k),dim(d){
	list = new Listlsh*[s];
	for(int i=0;i<s;i++){
		list[i]=new Listlsh();
	}
	if(metric==0){
		v=new myvectorlsh*[k];
		t=new long double[k];
		r=new int[k];
		for(int i=0;i<k;i++){
		  	long double* map=new long double[dim];
			for (int i=0; i<dim; i++) {
				long double number = distribution(generator);
			    //cout<<number<<endl;
			   	map[i]=number;
			}
			char buff[4];
			strcpy(buff,"vvv");
			v[i] = new myvectorlsh(-1,map,dim,1);
			t[i] = w * (rand() / (RAND_MAX + 1.0));
			r[i] = rand()%800;
			delete []map;
			//r[i] = 800 * (rand() / (RAND_MAX + 1.0));
			//r[i] = -1000 + (rand() / (RAND_MAX + 1.0))*(2000+1);
		}
	}
	else if (metric==1){
		v=new myvectorlsh*[k];
		//cout<<"product"<<endl;
		for(int i=0;i<k;i++){
			//long double* map=normal_distribute(dim);
	  		long double* map=new long double[dim];
			for (int i=0; i<dim; i++) {
				long double number = distribution(generator);
			    //cout<<number<<endl;
			   	map[i]=number;
			}
			char buff[4];
			strcpy(buff,"vvv");
			v[i] = new myvectorlsh(-1,map,dim,1);
			delete []map;
		}
		t=NULL;
		r=NULL;


	}
}

hashtable::~hashtable(){
	for(int i=0;i<size;i++){
		delete list[i];
	}
	delete []list;
	for(int i=0;i<K;i++){
		delete v[i];
	}
	delete []v;

	if(t!=NULL)
	delete []t;

	if(r!=NULL)
	delete []r;

}

void hashtable::insert(myvectorlsh* nodept,bool metric) { 
		int* g=hashfunction(nodept,metric);
		///g[K] is the index
		list[g[K]]->insert(nodept,g);
		//Listlsh[hashfunction(nodept,metric)]->insert(nodept);
}


void hashtable::hashprint(){
	for(int i=0;i<size;i++){
		list[i]->display();
	}
}

myvectorlsh* hashtable::search(myvectorlsh* pt,bool metric){
		int* g=hashfunction(pt,metric);
		///g[K] is the index
		myvectorlsh* temp = list[g[K]]->search(pt,metric,K,g);
		delete []g;
		return temp;
		//return Listlsh[hashfunction(pt,metric)]->search(pt,metric,K);
}

void hashtable::Rangesearch(myvectorlsh* pt,int metric,long double R,int j,myvectorlsh** centroids){
		int* g=hashfunction(pt,metric);
		///g[K] is the index
		//Listlsh* result=
		list[g[K]]->Rangesearch(pt,metric,R,K,g,j,centroids);
		delete []g;
		//return result;
		//Listlsh[hashfunction(pt,metric)]->Rangesearch(pt,metric,R,output,K);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////


long double euclidean(long double* X,long double * Y,int dim){
	long double sum=0;
	for(int i=0;i<dim;i++){
		sum+=((X[i]-Y[i])*(X[i]-Y[i]));
	}
	return sqrt(sum);
}

long double cosine(long double* X,long double* Y,int dim){
	long double sum1=0,sum2=0,sum3=0;
	for(int i=0;i<dim;i++){
		sum1+=X[i]*Y[i];
		sum2+=X[i]*X[i];
		sum3+=Y[i]*Y[i];
	}
	long double dis = sum1 / (sqrt(sum2)*sqrt(sum3));
	return 1 - dis;
}

long double inner_product(myvectorlsh* X,myvectorlsh* Y){
	long double sum1=0;
	for(int i=0;i<X->d;i++){
		sum1+=(X->pointsd[i])*(Y->pointsd[i]);
	}
	return sum1;
}

long double* normal_distribute(int times){

  default_random_engine generator;
  normal_distribution<long double> distribution(0.0,1.0);

  long double* p=new long double[times];
  //cout<<times<<endl;
  for (int i=0; i<times; i++) {
    long double number = distribution(generator);
    //cout<<number<<endl;
    p[i]=number;
  }

  return p;
}

