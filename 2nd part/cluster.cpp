#include "structures.h"

using namespace std;


int main(int argc,char *argv[]){
	FILE *stream;
    char *linee = NULL,*name_i,*name_c,*name_o,*nmetric;
    size_t len = 0;
	int k=4,L=5,numoflines=0,i=0,counter,K,probes=3,probedim=3;
	int flag1=0,flag2=0,flag3=0,dim,init=0,assign=0,update=0,complete=0;
	bool metric;
	List** clusters; 
	srand(time(NULL));
	myvector** v,**centroids,**ucentroids,**acentroids;
	if(argc<9){
		cout<<"Incorrect num of arguments.Try again!"<<endl;
		return -1;
	}
	for(int i=1;i<argc;i+=2){
		if(strcmp(argv[i],"-i")==0){
			flag1=1;
			name_i=new char[strlen(argv[i+1])+1];
			strcpy(name_i,argv[i+1]);
		}
		else if(strcmp(argv[i],"-d")==0){
			nmetric=new char[strlen(argv[i+1])+1];
			strcpy(nmetric,argv[i+1]);
			if(strcmp("euclidean",nmetric)==0){
				metric = 0;
			}
			else if(strcmp("cosine",nmetric)==0){
				metric = 1;
			}
		}
		else if(strcmp(argv[i],"-c")==0){
			flag2=1;
			name_c=new char[strlen(argv[i+1])+1];
			strcpy(name_c,argv[i+1]);
		}
		else if(strcmp(argv[i],"-o")==0){
			flag3=1;
			name_o=new char[strlen(argv[i+1])+1];
			strcpy(name_o,argv[i+1]);
		}
		if(strcmp(argv[i],"-complete")==0){
			complete=1;
		}
	}
	if((flag1==1)&&(flag2==1)&&(flag3==1)){
		///elegxos parametrwn ap to configuration file
	    stream = fopen(name_c, "r");
	    if (stream == NULL) {
	        perror("fopen");
	        exit(EXIT_FAILURE);
	    }
	    while ((getline(&linee, &len, stream)) != -1) {
	    	char* pc;
			pc = strtok(linee," \t");
	        if(strcmp(pc,"number_of_clusters:")==0){
	        	pc = strtok(NULL,"\n");
	        	K = atoi(pc);
	        }
	        else if(strcmp(pc,"number_of_hash_functions:")==0){
	        	pc = strtok(NULL,"\n");
	        	k = atoi(pc);
	        }
	        else if(strcmp(pc,"number_of_hash_tables:")==0){
	        	pc = strtok(NULL,"\n");
	        	L = atoi(pc);
	        }
	        else if(strcmp(pc,"number_of_probes:")==0){
	        	pc = strtok(NULL,"\n");
	        	probes = atoi(pc);
	        }
	        else if(strcmp(pc,"probe_to_dim:")==0){
	        	pc = strtok(NULL,"\n");
	        	probedim = atoi(pc);
	        }
	        else if(strcmp(pc,"type_of_init:")==0){
	        	pc = strtok(NULL,"\n");
	        	init = atoi(pc);
	        }
	        else if(strcmp(pc,"type_of_assignment:")==0){
	        	pc = strtok(NULL,"\n");
	        	assign = atoi(pc);
	        }
	        else if(strcmp(pc,"type_of_update:")==0){
	        	pc = strtok(NULL,"\n");
	        	update = atoi(pc);
	        }
	    }
		fclose(stream);

		//metrima grammwn+dimension
	    stream = fopen(name_i, "r");
	    if (stream == NULL) {
	        perror("fopen");
	        exit(EXIT_FAILURE);
	    }
	    while ((getline(&linee, &len, stream)) != -1) {
	    	if(numoflines==2){
	    		char* pc;
				pc = strtok(linee," \t,");
				int i=0;
				while(pc!=NULL){
					pc=strtok(NULL,"  \t,");
					i++;
				}
				dim=i-1;

	    	}
		    numoflines++;
	    }
		fclose(stream);
		cout<<numoflines<<endl;

    	///save the points 
        stream = fopen(name_i, "r");
        if (stream == NULL) {
        	perror("fopen");
        	exit(EXIT_FAILURE);
        }
		v=new myvector*[numoflines];
    	counter=0;
        while ((getline(&linee, &len, stream)) != -1) {
	       	counter++;
	       	i=0;
			//cout<<linee<<endl;
			long double* map=new long double[dim];
			char* pch;
			pch = strtok(linee," \t,");
			int id = atoi(linee);
			pch=strtok(NULL,"  \t,");
			while(pch!=NULL){
				long double num=stold(pch);
 				cout.precision(25);
 				map[i]=num;
				pch=strtok(NULL,"  \t,");
				i++;
			}
			if(i!=dim){
				cout<<"error in input file"<<endl;
				return -1;
			}
			v[counter-1] = new myvector(id,map,dim,1);
			delete []map;
		}

		free(linee);
		fclose(stream);

		clock_t time1,time2;
		time1=clock();

		////INITIALIZE K centroids
		if(init==1){
        	centroids=RandomInit(K,v,numoflines);
        }
        else if(init==2){
        	centroids=Kmeansplusplus(K,v,numoflines,metric);
        }
        else{
	        cout<<"error initialization type given"<<endl;
	        return -1;
        }

        ///while -until no changes
        acentroids=centroids;
        counter=0;
        while(1){
	        ////ASSIGNMENT
	        if(assign==1){
	        	clusters=LloydsAssign(K,acentroids,v,metric,numoflines);
	        }
	        else if(assign==2){
	        	clusters=LSHrangeassign(K,acentroids,v,metric,numoflines,k,L,dim);
	        }
	        else if(assign==3){
	        	clusters=CUBErangeassign(K,acentroids,v,metric,numoflines,probes,probedim,dim);
	        }
	        else{
	        	cout<<"error assignment type given"<<endl;
	        	return -1;
	        }

	        /////UPDATE
	       	if(update==1){
	        	ucentroids=Kmeansupdate(K,acentroids,numoflines,clusters);
	        }
	        else if(update==2){
	        	ucentroids=PAMupdate(K,acentroids,numoflines,clusters,metric);

	        }
	        else{
	        	cout<<"error update type given"<<endl;
	        	return -1;
	        }
	        long double sum=0;
	        for(int i=0;i<K;i++){
	        	if(metric==0){
	        		sum+=euclidean_dist(ucentroids[i]->pointsd,acentroids[i]->pointsd,dim);
	        	}
	        	else if(metric==1){
	        		sum+=cosine_similarity(ucentroids[i]->pointsd,acentroids[i]->pointsd,dim);	        		
	        	}
	        }
	        cout<<sum<<endl;

	        acentroids=ucentroids;
	       	counter++;
	        if((sum==0)||(counter==30)){
	       		break;
	       	}
	       	for(int i=0;i<K;i++){
		  		delete clusters[i];
		  	}
		  	delete []clusters;
		}
		time2=clock();
		long double tcluster=(long double)(time2-time1)/CLOCKS_PER_SEC;

	    ofstream output;
	    output.open(name_o);

	    output<<"Algorithm: I"<<init<<"A"<<assign<<"U"<<update<<endl;
	    output<<"Metric: "<<nmetric<<endl;
	    for(int i=0;i<K;i++){
	    	output<<"CLUSTER-"<<i<<" {size: "<<clusters[i]->length()<<" ,centroid: "<<acentroids[i]->id<<" }"<<endl;
		}
		output<<"clustering_time: "<<tcluster<<endl;
		long double* a=new long double[numoflines];
		long double* b=new long double[numoflines];
		long double* S=new long double[numoflines];
		a=aSilhouette(numoflines,v,clusters,metric);
		b=bSilhouette(numoflines,v,clusters,metric);
		long double* s=cSilhouette(clusters,K);
		long double Stotal=0;
		for (int i = 0; i < numoflines; i++)
		{
			if(a[i]>b[i]){
				S[i]=(b[i]-a[i])/a[i];
			}
			else{
				S[i]=(b[i]-a[i])/b[i];				
			}
			Stotal+=S[i];
		}
		Stotal=Stotal/numoflines;
		cout<<Stotal<<endl;
		output<<"Silhouette: [ ";
		for (int i = 0; i < K; i++)
		{
			output<<s[i]<<" , ";
		}
		output<<Stotal;
		output<<" ]"<<endl;
		if(complete==1){
		    for(int i=0;i<K;i++){
		    	output<<"CLUSTER-"<<i<<" {";
		    	clusters[i]->display(output);
		    	output<<" }"<<endl;
			}			
		}
	    output.close();

	    delete []a;
	    delete []b;
	    delete []S;
	    delete []s;

	   	for(int i=0;i<K;i++){
		  		delete clusters[i];
		}
		delete []clusters;


	}
	//cout<<dim<<endl;

  	delete []centroids;
  	for(int i=0;i<K;i++){
  		delete ucentroids[i];
  	}
  	delete []ucentroids;

	delete []name_i;
	delete []name_c;
	delete []name_o;
	delete []nmetric;

	return 0;

}