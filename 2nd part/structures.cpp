#include "structures.h"
#include "lsh.h"
#include "cube.h"

int ind;

myvector::myvector(int num,int* pts,int dim,bool t){
		type=t;
		d=dim;
		id=num;
		points= new int[dim];
		for(int i=0;i<dim;i++){
				points[i]=pts[i];
		}

}


myvector::myvector(int num,long double* pts,int dim,bool t){
		type=t;
		d=dim;
		id=num;
		pointsd= new long double[dim];
		for(int i=0;i<dim;i++){
				pointsd[i]=pts[i];

		}

}

myvector::~myvector(){
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

void List::insert(myvector* nodept,int i,int j){
	listnode* temp = new listnode();
	temp->ptr=nodept;
	temp->ptr->cluster=i;
	temp->ptr->ncluster=j;
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

void List::display(ofstream& output){
	listnode* temp = head;
	while(temp!=NULL){
		output<<temp->ptr->id<<", ";
		temp=temp->next;
	}

}

int List::length(){
	return size;
}

myvector* List::pointsum(){
	long double* p = new long double[head->ptr->d];

	for(int i = 0; i < head->ptr->d; i++){
		long double sum=0;
		listnode* temp = head;
		while(temp!=NULL){
			sum+=temp->ptr->pointsd[i];
			temp=temp->next;
		}
		p[i]=sum/(long double)head->ptr->d;
	}

	ind++;
	myvector* result = new myvector(ind,p,head->ptr->d,1);
	delete []p;
	return result;

}

myvector* List::medoid(int metric){

		listnode* temp2 = head;
		long double min = numeric_limits<long double>::max();
		long double** dis=new long double*[size];
		for (int i = 0; i < size; i++)
		{
			dis[i]=new long double[size];
		}
		myvector* result;
		int i=0;
		while(temp2!=NULL){
			long double sum;
			listnode* temp = head;
			int j=0;
			sum=0;
			while(temp!=NULL){
				if(metric==0){
					/*if(j<i){
						sum+=dis[j][i];
						temp=temp->next;
						continue;
					}*/
					dis[i][j]=euclidean_dist(temp->ptr->pointsd,temp2->ptr->pointsd,temp->ptr->d);
					sum+=dis[i][j];
					//sum+=euclidean_dist(temp->ptr->pointsd,temp2->ptr->pointsd,temp->ptr->d);
				}
				else if(metric==1){
					/*if(j<i){
						sum+=dis[j][i];
						temp=temp->next;
						continue;
					}*/
					dis[i][j]=cosine_similarity(temp->ptr->pointsd,temp2->ptr->pointsd,temp->ptr->d);
					sum+=dis[i][j];
					//sum+=cosine_similarity(temp->ptr->pointsd,temp2->ptr->pointsd,temp->ptr->d);
				}
				temp=temp->next;
				j++;
			}

			if(sum<min){
				min=sum;
				result=temp2->ptr;
			}
			temp2=temp2->next;
			i++;
		}

		return result;
}

long double List::Silhouette(myvector* pt,int metric){
	long double dis=0;
	listnode* temp = head;
	while(temp!=NULL){
			if(metric==0){
				dis+=euclidean_dist(temp->ptr->pointsd,pt->pointsd,pt->d);
			}
			else{
				dis+=cosine_similarity(temp->ptr->pointsd,pt->pointsd,pt->d);
			}
			temp=temp->next;
	}
	if(size>0){
		dis=dis/size;
	}
	else{
		return 0;
	}

	return dis;

}

long double List::averageSilhouette(){
	long double dis=0;
	listnode* temp = head;
	while(temp!=NULL){
		long double a=temp->ptr->Sa;
		long double b=temp->ptr->Sb;
		if(a>b){
			dis+=(b-a)/a;
		}
		else{
			dis+=(b-a)/b;

		}
		temp=temp->next;
	}
	if(size>0){
		dis=dis/size;
	}
	else{
		return 0;
	}

	return dis;
}



//////////////////////////////////////////////////////////////////////////////////////////////////////////


long double euclidean_dist(long double* X,long double* Y,int dim){
	long double sum=0;
	for(int i=0;i<dim;i++){
		sum+=((X[i]-Y[i])*(X[i]-Y[i]));
	}
	return sqrtl(sum);
}

long double cosine_similarity(long double* X,long double* Y,int dim){
	long double sum1=0,sum2=0,sum3=0;
	int flag=0;
	for(int i=0;i<dim;i++){
		if(X[i]!=Y[i]){
			flag=1;
			break;
		}
	}
	if(flag==0){
		return 0;
	}
	for(int i=0;i<dim;i++){
		sum1+=X[i]*Y[i];
		sum2+=X[i]*X[i];
		sum3+=Y[i]*Y[i];
	}
	long double dis = sum1 / (sqrt(sum2)*sqrt(sum3));
	return 1 - dis;
}

long double inner_product(myvector* X,myvector* Y){
	long double sum1=0;
	for(int i=0;i<X->d;i++){
		sum1+=(X->points[i])*(Y->pointsd[i]);
	}
	return sum1;
}

myvector** RandomInit(int numC,myvector** vec,int num){
	int* r=new int[numC];
	ind=num;
	myvector** result  = new myvector*[numC];
	for(int i=0;i<numC;i++){
		while(1){
			r[i] = rand()%num;
			for(int j=0;j<i;j++){
				if(r[j]==r[i]){
					continue;
				}
			}
			break;
		}
	}

	for (int i = 0; i < numC; i++)
	{
		result[i] = vec[r[i]];		
	}

	delete []r;

	return result;
}

myvector** Kmeansplusplus(int numC,myvector** vec,int num,int metric){

	ind=num;
	myvector** result  = new myvector*[numC];
	int* r=new int[numC];
	r[0] = rand()%num;
	for(int i = 1 ; i <= numC ;i++){
		for(int j=0;j<i;j++){
				if(r[j]==i){
					continue;
				}
		}
		long double *D=new long double[num-i];
		long double max=-1000000;
		for(int k=0;k<num-i;k++){
			long double min;
			if(metric==0){
				min=euclidean_dist(vec[k]->pointsd,vec[r[0]]->pointsd,vec[k]->d);
				for(int j=0;j<i-1;j++){
					//cout<<r[j]<<" "<<j<<endl;
					long double dis=euclidean_dist(vec[k]->pointsd,vec[r[j]]->pointsd,vec[k]->d);
					if(dis<min){
						min=dis;
					}
				}
			}
			else if(metric==1){
				min=cosine_similarity(vec[k]->pointsd,vec[r[0]]->pointsd,vec[k]->d);
				for(int j=0;j<i-1;j++){
					long double dis=cosine_similarity(vec[k]->pointsd,vec[r[j]]->pointsd,vec[k]->d);
					if(dis<min){
						min=dis;
					}
				}
			}
			D[k]=min;
			if(min>max){
				max=min;
			}
		}
		long double *P=new long double[num-i];
		P[0]=pow((D[0]/max),2);
		for(int k=1;k<num-i;k++){
			P[k]=P[k-1]+pow((D[k]/max),2);
		}
		int s;
		long double x=P[num-i-1] * (rand() / (RAND_MAX + 1.0));
		for(int k=1;k<num-i;k++){
			if((x<=P[k])&&(x>P[k-1])){
				s=k;
			}
		}
		r[i-1]=s;
		delete []D;
		delete []P;
	}

	for (int i = 0; i < numC; i++)
	{
		result[i] = vec[r[i]];		
	}

	delete []r;

	return result;


}

List** LloydsAssign(int numC,myvector** centroids,myvector** v,int metric,int num){

	List** result = new List*[numC];
	for(int i=0;i<numC;i++){
		result[i]=new List();
	}
	for(int i=0;i<num;i++){
		long double min=numeric_limits<long double>::max(),min2=numeric_limits<long double>::max();
		int minv,minv2;
		if(metric==0){
			for(int j=0;j<numC;j++){
				long double dis=euclidean_dist(v[i]->pointsd,centroids[j]->pointsd,v[i]->d);
				//cout<<j<<" "<<dis<<endl;
				if(dis<min){
					min=dis;
					minv=j;
				}
			}
			for(int j=0;j<numC;j++){
				if(j==minv)continue;
				long double dis=euclidean_dist(v[i]->pointsd,centroids[j]->pointsd,v[i]->d);
				//cout<<j<<" "<<dis<<endl;
				if(dis<min2){
					min2=dis;
					minv2=j;
				}
			}
		}
		else if(metric==1){
			for(int j=0;j<numC;j++){
				long double dis=cosine_similarity(v[i]->pointsd,centroids[j]->pointsd,v[i]->d);
				//cout<<j<<" "<<dis<<endl;
				if(dis<min){
					min=dis;
					minv=j;
				}
			}
			for(int j=0;j<numC;j++){
				if(j==minv)continue;
				long double dis=cosine_similarity(v[i]->pointsd,centroids[j]->pointsd,v[i]->d);
				//cout<<j<<" "<<dis<<endl;
				if(dis<min2){
					min2=dis;
					minv2=j;
				}
			}
		}
		result[minv]->insert(v[i],minv,minv2);
	}

	return result;

}

List** LSHrangeassign(int numC,myvector** centroids,myvector** v,int metric,int num,int k,int L,int dim){
	
	List** result;
	Listlsh** result2 = new Listlsh*[numC];
	for(int i=0;i<numC;i++){
		result2[i]=new Listlsh();
	}

    ///construct L hashtables
	hashtable** hmap = new hashtable*[L];
	for(int i=0;i<L;i++){
		hmap[i]=new hashtable(num/8,k,dim,metric);
	}	

	myvectorlsh** vlsh=new myvectorlsh*[num];
	for (int i = 0; i < num; i++)
	{
		vlsh[i]= new myvectorlsh(v[i]->id,v[i]->pointsd,dim,1);
	}

	myvectorlsh** centroidslsh=new myvectorlsh*[numC];
	for (int i = 0; i < numC; i++)
	{
		centroidslsh[i]= new myvectorlsh(centroids[i]->id,centroids[i]->pointsd,dim,1);
	}
	//insert the points to them
	for(int j=0;j<num;j++){
		for(int i=0;i<L;i++){
			hmap[i]->insert(vlsh[j],metric);
		}
	}
	long double min = numeric_limits<long double>::max();		

	for(int i=0;i<numC;i++){
		for(int j=0;j<numC;j++){
			long double dist;
			if (metric==0)
			{
				if(i!=j){
					dist=euclidean_dist(centroids[i]->pointsd,centroids[j]->pointsd,dim);
					if (dist<min)
					{
						min=dist;
					}	
				}
			}
			else if (metric==1)
			{
				if(i!=j){
					dist=cosine_similarity(centroids[i]->pointsd,centroids[j]->pointsd,dim);
					if (dist<min)
					{
						min=dist;
					}	
				}
			}	
		}
	}
	//cout<<min<<endl;

	long double R=min/2;
	int counter=0,sum2=0,help=0,num2,flag12;
	while(1){
		//cout<<R<<endl;
		int sum=0;
		for (int j = 0; j < numC; j++){
			for(int i=0;i<L;i++){
				hmap[i]->Rangesearch(centroidslsh[j],metric,R,j,centroidslsh);
			}
		}
		int flag3=0,flag2=0;
		for (int i = 0; i < num; i++)
		{
			if(vlsh[i]->flag==-1){
				sum++;
				flag3=1;
			}
			else{
				int flag=0;
				ListlshNode* temp=result2[vlsh[i]->flag]->head;
				while(temp!=NULL){
					if(temp->ptr->id==vlsh[i]->id){
						flag=1;
						break;
					}
					temp=temp->next;
				}
				if(flag==0){
					result2[vlsh[i]->flag]->insert(vlsh[i],NULL);
				}				
			}

		}
		//cout<<sum<<endl;
		if(help==sum){
			sum2++;
		}
		help=sum;
		if(sum2==10){
			flag2=1;
		}
		if((flag2==1)||(flag3==0)){
			flag12=flag2;
			num2=sum;
			break;
		}
		counter++;
		R*=2;
	}
	if(flag12==1){
		myvector** restvecs=new myvector*[num2];
		counter=0;
		for (int i = 0; i < num; i++)
		{
			if(vlsh[i]->flag==-1){
				restvecs[counter]=new myvector(vlsh[i]->id,vlsh[i]->pointsd,dim,1);
				counter++;
			}
		}

		result=LloydsAssign(numC,centroids,restvecs,metric,num2);
	}
	else{
		result=new List*[numC];
		for(int i=0;i<numC;i++){
			result[i]=new List();
		}
	}

	for (int i = 0; i < numC; i++)		
	{
		ListlshNode* temp=result2[i]->head;
		while(temp!=NULL){
			for (int j = 0; j < num; j++)
			{
				if(v[j]->id==temp->ptr->id){

					int minv2;
					long double min2=numeric_limits<long double>::max();
					if(metric==0){
						for(int k=0;k<numC;k++){
							if(k==i)continue;
							long double dis=euclidean_dist(v[j]->pointsd,centroids[k]->pointsd,v[i]->d);
							if(dis<min2){
								min2=dis;
								minv2=k;
							}
						}
					}
					else if(metric==1){
						for(int k=0;k<numC;k++){
							if(k==i)continue;
							long double dis=cosine_similarity(v[j]->pointsd,centroids[k]->pointsd,v[i]->d);
							if(dis<min2){
								min2=dis;
								minv2=k;
							}
						}
					}
					result[i]->insert(v[j],i,minv2);
					break;
				}
			}
			temp=temp->next;
		}
	}

	return result;

}

List** CUBErangeassign(int numC,myvector** centroids,myvector** v,int metric,int num,int probes,int probedim,int dim){
	
	List** result;
	listcube** result2 = new listcube*[numC];
	for(int i=0;i<numC;i++){
		result2[i]=new listcube();
	}

    ///construct cube
	cube* hmap = new cube(num/8,probedim,dim,metric,probes);

	myvectorcube** vlsh=new myvectorcube*[num];
	for (int i = 0; i < num; i++)
	{
		vlsh[i]= new myvectorcube(v[i]->id,v[i]->pointsd,dim,1);
	}

	myvectorcube** centroidslsh=new myvectorcube*[numC];
	for (int i = 0; i < numC; i++)
	{
		centroidslsh[i]= new myvectorcube(centroids[i]->id,centroids[i]->pointsd,dim,1);
	}
	//insert the points to them
	for(int j=0;j<num;j++){
			hmap->insert(vlsh[j],metric);
	}
	long double min = numeric_limits<long double>::max();		

	for(int i=0;i<numC;i++){
		for(int j=0;j<numC;j++){
			long double dist;
			if (metric==0)
			{
				if(i!=j){
					dist=euclidean_dist(centroids[i]->pointsd,centroids[j]->pointsd,dim);
					if (dist<min)
					{
						min=dist;
					}	
				}
			}
			else if (metric==1)
			{
				if(i!=j){
					dist=cosine_similarity(centroids[i]->pointsd,centroids[j]->pointsd,dim);
					if (dist<min)
					{
						min=dist;
					}	
				}
			}	
		}
	}
	//cout<<min<<endl;

	long double R=min/2;
	int counter=0,sum2=0,help=0,num2,flag12;
	while(1){
		//cout<<R<<endl;
		int sum=0;
		for (int j = 0; j < numC; j++){
			hmap->Rangesearch(centroidslsh[j],metric,R,j,centroidslsh);
		}
		int flag3=0,flag2=0;
		for (int i = 0; i < num; i++)
		{
			if(vlsh[i]->flag==-1){
				sum++;
				flag3=1;
			}
			else{
				int flag=0;
				listcubenode* temp=result2[vlsh[i]->flag]->head;
				while(temp!=NULL){
					if(temp->ptr->id==vlsh[i]->id){
						flag=1;
						break;
					}
					temp=temp->next;
				}
				if(flag==0){
					result2[vlsh[i]->flag]->insert(vlsh[i],NULL);
				}				
			}

		}
		//cout<<sum<<endl;
		if(help==sum){
			sum2++;
		}
		help=sum;
		if(sum2==10){
			flag2=1;
		}
		if((flag2==1)||(flag3==0)){
			flag12=flag2;
			num2=sum;
			break;
		}
		counter++;
		R*=2;
	}
	if(flag12==1){
		myvector** restvecs=new myvector*[num2];
		counter=0;
		for (int i = 0; i < num; i++)
		{
			if(vlsh[i]->flag==-1){
				restvecs[counter]=new myvector(vlsh[i]->id,vlsh[i]->pointsd,dim,1);
				counter++;
			}
		}
		result=LloydsAssign(numC,centroids,restvecs,metric,num2);
	}
	else{
		result=new List*[numC];
		for(int i=0;i<numC;i++){
			result[i]=new List();
		}
	}

	for (int i = 0; i < numC; i++)		
	{
		listcubenode* temp=result2[i]->head;
		while(temp!=NULL){
			for (int j = 0; j < num; j++)
			{
				if(v[j]->id==temp->ptr->id){

					int minv2;
					long double min2=numeric_limits<long double>::max();
					if(metric==0){
						for(int k=0;k<numC;k++){
							if(k==i)continue;
							long double dis=euclidean_dist(v[j]->pointsd,centroids[k]->pointsd,v[i]->d);
							if(dis<min2){
								min2=dis;
								minv2=k;
							}
						}
					}
					else if(metric==1){
						for(int k=0;k<numC;k++){
							if(k==i)continue;
							long double dis=cosine_similarity(v[j]->pointsd,centroids[k]->pointsd,v[i]->d);
							if(dis<min2){
								min2=dis;
								minv2=k;
							}
						}
					}
					result[i]->insert(v[j],i,minv2);
					break;
				}
			}
			temp=temp->next;
		}
	}

	return result;

}

myvector** Kmeansupdate(int numC,myvector** v,int num,List** clusters){

	myvector** result  = new myvector*[numC];
	for (int i = 0; i < numC; i++)
	{
		//cout<<clusters[i]->length()<<endl;
		if(clusters[i]->length()>0){
			result[i]=clusters[i]->pointsum();
		}
		else{
			result[i]=v[i];
		}
	}

	return result;
}

myvector** PAMupdate(int numC,myvector** v,int num,List** clusters,int metric){

	myvector** result  = new myvector*[numC];
	for (int i = 0; i < numC; i++){
		//cout<<clusters[i]->length()<<endl;
		if(clusters[i]->length()>0){
			result[i]=clusters[i]->medoid(metric);
		}
		else{
			result[i]=v[i];
		}


	}


	return result;


}

long double* aSilhouette(int num,myvector** v,List** c,int metric){
	long double* a=new long double[num];
	for (int i = 0; i < num; i++)
	{
		a[i]=c[v[i]->cluster]->Silhouette(v[i],metric);
		v[i]->Sa=a[i];
	}	

	return a;

}

long double* bSilhouette(int num,myvector** v,List** c,int metric){
	long double* b=new long double[num];
	for (int i = 0; i < num; i++)
	{
		b[i]=c[v[i]->ncluster]->Silhouette(v[i],metric);
		v[i]->Sb=b[i];

	}	

	return b;

}

long double* cSilhouette(List** c,int K){
	long double* result=new long double[K];
	for (int i = 0; i < K; i++)
	{
		result[i]=c[i]->averageSilhouette();
	}

	return result;
}



