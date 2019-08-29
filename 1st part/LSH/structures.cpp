#include "structures.h"

default_random_engine generator;
normal_distribution<double> distribution(0.0,1.0);

myvector::myvector(char* name,int* pts,int dim,bool t){
		type=t;
		d=dim;
		nameid= new char[strlen(name)+1];
		strcpy(nameid,name);
		points= new int[dim];
		for(int i=0;i<dim;i++){
				points[i]=pts[i];
		}
		/*cout<<"created myvector name "<<nameid<<" with dim "<<dim<<endl;
		cout<<"Points:"<<endl;
		for(int i=0;i<dim;i++){
			cout<<points[i]<<" ";
		}	
		cout<<endl;	*/

}


myvector::myvector(char* name,double* pts,int dim,bool t){
		type=t;
		d=dim;
		nameid= new char[strlen(name)+1];
		strcpy(nameid,name);
		pointsd= new double[dim];
		for(int i=0;i<dim;i++){
				pointsd[i]=pts[i];

		}
		/*cout<<"created myvector name "<<nameid<<" with dim "<<dim<<endl;
		cout<<"Points:"<<endl;
		for(int i=0;i<dim;i++){
			cout<<points[i]<<" ";
		}	
		cout<<endl;	*/

}

myvector::~myvector(){
	delete []nameid;
	if(type==0){
		delete []points;
	}
	else{
		delete []pointsd;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


listnode::listnode(){
	next=NULL;
	ptr=NULL;
	g=NULL;
}

listnode::~listnode(){
	if(g!=NULL)
	delete []g;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
List::List(){
	head=NULL;
	size=0;
}

List::~List(){
	listnode* current = head;
	while( current != NULL) {
    	listnode* next = current->next;
    	current->ptr=NULL;
    	delete current;
    	current = next;
	}
	head = NULL;
}

void List::insert(myvector* nodept,int* g){
	listnode* temp = new listnode();
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

void List::insertend(myvector* nodept){
	listnode* temp = new listnode();
	temp->ptr=nodept;
	if(head==NULL){
		head=temp;
	}
	else{
			listnode* current = head;
			while(current->next != NULL){
				current=current->next;
			}
			current -> next = temp;

	}
	size++;
		
}

bool List::is_empty(){
	if (size<=0){
		return true;
	}
	return false;
}

void List::display(){
	listnode* temp = head;
	while(temp!=NULL){
		cout<<temp->ptr->nameid<<" ";
		for(int i=0;i<temp->ptr->d;i++){
			cout<<temp->ptr->points[i]<<" ";
		}
		cout<<endl;
		temp=temp->next;
	}

}

int List::length(){
	return size;
}

myvector* List::search(myvector* pt,bool metric,int K,int* g){
	double min=100000;
	int flag=0;
	myvector *vec=NULL;
	listnode* temp = head;
	while(temp!=NULL){
		if(metric==0){
			for(int i=0;i<K;i++){
				if(g[i]!=(temp->g[i])){
					flag=1;
					break;
				}
			}
			if((euclidean_dist(pt->points,temp->ptr->points,temp->ptr->d)<min)&&(flag==0)){
				min=euclidean_dist(pt->points,temp->ptr->points,temp->ptr->d);
				vec=temp->ptr;
			}
			temp=temp->next;
		}
		else{
			if((cosine_similarity(pt->points,temp->ptr->points,temp->ptr->d)<min)&&(flag==0)){
				min=cosine_similarity(pt->points,temp->ptr->points,temp->ptr->d);
				vec=temp->ptr;
			}
			temp=temp->next;
		}
	}
	//cout<<min<<endl;
	if(vec!=NULL)vec->dis=min;
	return vec;

}
myvector* List::exhaustedsearch(myvector* pt,bool metric,int K){
	double min=100000;
	myvector *vec=NULL;
	listnode* temp = head;
	while(temp!=NULL){
		if(metric==0){
			if(euclidean_dist(pt->points,temp->ptr->points,temp->ptr->d)<min){
				min=euclidean_dist(pt->points,temp->ptr->points,temp->ptr->d);
				vec=temp->ptr;
			}
			temp=temp->next;
		}
		else{
			if(cosine_similarity(pt->points,temp->ptr->points,temp->ptr->d)<min){
				min=cosine_similarity(pt->points,temp->ptr->points,temp->ptr->d);
				vec=temp->ptr;
			}
			temp=temp->next;
		}
	}
	//cout<<min<<endl;
	if(vec!=NULL)vec->dis=min;
	return vec;

}

void List::Rangesearch(myvector* pt,bool metric,int R,ofstream& output,int K,int* g){
	myvector *vec=NULL;
	int flag=0;
	listnode* temp = head;
	while(temp!=NULL){
		if(metric==0){
			for(int i=0;i<K;i++){
				if(g[i]!=(temp->g[i])){
					flag=1;
					break;
				}
			}
			if((euclidean_dist(pt->points,temp->ptr->points,temp->ptr->d)<R)&&(flag==0)){
				vec=temp->ptr;
				output<<vec->nameid<<endl;
			}
			temp=temp->next;
		}
		else{
			if((cosine_similarity(pt->points,temp->ptr->points,temp->ptr->d)<R)&&(flag==0)){
				vec=temp->ptr;
				output<<vec->nameid<<endl;
			}
			temp=temp->next;
		}
	}

}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int* hashtable:: hashfunction(myvector* p,bool metric){
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
	list = new List*[s];
	for(int i=0;i<s;i++){
		list[i]=new List();
	}
	if(metric==0){
		v=new myvector*[k];
		t=new double[k];
		r=new int[k];
		for(int i=0;i<k;i++){
		  	double* map=new double[dim];
			for (int i=0; i<dim; i++) {
				double number = distribution(generator);
			    //cout<<number<<endl;
			   	map[i]=number;
			}
			char buff[4];
			strcpy(buff,"vvv");
			v[i] = new myvector(buff,map,dim,1);
			t[i] = w * (rand() / (RAND_MAX + 1.0));
			r[i] = rand()%800;
			delete []map;
			//r[i] = 800 * (rand() / (RAND_MAX + 1.0));
			//r[i] = -1000 + (rand() / (RAND_MAX + 1.0))*(2000+1);
		}
	}
	else if (metric==1){
		v=new myvector*[k];
		//cout<<"product"<<endl;
		for(int i=0;i<k;i++){
			//double* map=normal_distribute(dim);
	  		double* map=new double[dim];
			for (int i=0; i<dim; i++) {
				double number = distribution(generator);
			    //cout<<number<<endl;
			   	map[i]=number;
			}
			char buff[4];
			strcpy(buff,"vvv");
			v[i] = new myvector(buff,map,dim,1);
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

void hashtable::insert(myvector* nodept,bool metric) { 
		int* g=hashfunction(nodept,metric);
		///g[K] is the index
		list[g[K]]->insert(nodept,g);
		//list[hashfunction(nodept,metric)]->insert(nodept);
}


void hashtable::hashprint(){
	for(int i=0;i<size;i++){
		list[i]->display();
	}
}

myvector* hashtable::search(myvector* pt,bool metric){
		int* g=hashfunction(pt,metric);
		///g[K] is the index
		myvector* temp = list[g[K]]->search(pt,metric,K,g);
		delete []g;
		return temp;
		//return list[hashfunction(pt,metric)]->search(pt,metric,K);
}

void hashtable::Rangesearch(myvector* pt,bool metric,int R,ofstream& output){
		int* g=hashfunction(pt,metric);
		///g[K] is the index
		list[g[K]]->Rangesearch(pt,metric,R,output,K,g);
		delete []g;
		//list[hashfunction(pt,metric)]->Rangesearch(pt,metric,R,output,K);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////


double euclidean_dist(int* X,int* Y,int dim){
	int sum=0;
	for(int i=0;i<dim;i++){
		sum+=((X[i]-Y[i])*(X[i]-Y[i]));
	}
	return sqrt(sum);
}

double cosine_similarity(int* X,int* Y,int dim){
	int sum1=0,sum2=0,sum3=0;
	for(int i=0;i<dim;i++){
		sum1+=X[i]*Y[i];
		sum2+=X[i]*X[i];
		sum3+=Y[i]*Y[i];
	}
	double dis = sum1 / (sqrt(sum2)*sqrt(sum3));
	return 1 - dis;
}

double inner_product(myvector* X,myvector* Y){
	double sum1=0;
	for(int i=0;i<X->d;i++){
		sum1+=(X->points[i])*(Y->pointsd[i]);
	}
	return sum1;
}

double* normal_distribute(int times){

  default_random_engine generator;
  normal_distribution<double> distribution(0.0,1.0);

  double* p=new double[times];
  //cout<<times<<endl;
  for (int i=0; i<times; i++) {
    double number = distribution(generator);
    //cout<<number<<endl;
    p[i]=number;
  }

  return p;
}

