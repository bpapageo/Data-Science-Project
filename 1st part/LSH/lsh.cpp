#include "structures.h"

using namespace std;


int main(int argc,char *argv[]){
	FILE *stream;
    char *linee = NULL,*name_d,*name_q,*name_o,*nameid,*namin,str[100],*nameidS;
    size_t len = 0;
	int k=4,L=5,numoflines=0,i=0,counter,numq;
	int flag1=0,flag2=0,flag3=0,dim;
	bool metric;
	double max_approximation=0,sum=0,sum2=0,sum3=0;
	nameid= new char[strlen("item_id")+1];
	strcpy(nameid,"item_id");
	nameidS= new char[strlen("item_idS")+1];
	strcpy(nameidS,"item_idS");
	List *list=new List();
	hashtable **hmap;
	srand(time(NULL));
	myvector** v;
	if(argc<7){
		cout<<"Incorrect num of arguments.Try again!"<<endl;
		return -1;
	}
	for(int i=1;i<argc;i+=2){
		if(strcmp(argv[i],"-d")==0){
			flag1=1;
			name_d=new char[strlen(argv[i+1])+1];
			strcpy(name_d,argv[i+1]);
		}
		else if(strcmp(argv[i],"-k")==0){
			k = atoi(argv[i+1]);
		}
		else if(strcmp(argv[i],"-L")==0){
			L = atoi(argv[i+1]);
		}
		else if(strcmp(argv[i],"-q")==0){
			flag2=1;
			name_q=new char[strlen(argv[i+1])+1];
			strcpy(name_q,argv[i+1]);
		}
		else if(strcmp(argv[i],"-o")==0){
			flag3=1;
			name_o=new char[strlen(argv[i+1])+1];
			strcpy(name_o,argv[i+1]);
		}
	}
	if((flag1==1)&&(flag2==1)&&(flag3==1)){
		//metrima grammwn+dimension
	    stream = fopen(name_d, "r");
	    if (stream == NULL) {
	        perror("fopen");
	        exit(EXIT_FAILURE);
	    }
	    while ((getline(&linee, &len, stream)) != -1) {
	    	if(numoflines==2){
	    		char* pc;
				pc = strtok(linee," \t");
				int i=0;
				while(pc!=NULL){
					pc=strtok(NULL,"  \t");
					i++;
				}
				dim=i;

	    	}
		    numoflines++;
	    }
		fclose(stream);
		cout<<numoflines<<endl;

		////insert input
        stream = fopen(name_d, "r");
        if (stream == NULL) {
        	perror("fopen");
        	exit(EXIT_FAILURE);
        }
        ///First line
        if((getline(&linee, &len, stream)) != -1){
        	char* pc;
			pc = strtok(linee," ");
	        if(strcmp(pc,"@euclidean")==0){
	        	metric=0;
	        }
	        else if(strcmp(pc,"@cosine")==0){
	        	metric=1;
	        }

    	}

    	///construct L hashtables
		hmap = new hashtable*[L];
		for(int i=0;i<L;i++){
			hmap[i]=new hashtable(((numoflines-1)/4),k,dim,metric);
		}
		v=new myvector*[numoflines-1];
    	///whole lines
    	counter=0;
        while ((getline(&linee, &len, stream)) != -1) {
	       	counter++;
	       	i=0;
			//cout<<linee<<endl;
			int* map=new int[dim];
			char* pch;
			sprintf(str,"%d",counter);
	        char* nam1=new char[strlen(nameid)+2+strlen(str)];
	        strcpy(nam1,nameid);
			strcat(nam1,str);
			namin=new char[strlen(nam1)+1];
			strcpy(namin,nam1);
			delete []nam1;
			pch = strtok(linee," \t");
			while(pch!=NULL){
				int num=atoi(pch);
				map[i]=num;
				pch=strtok(NULL,"  \t");
				i++;
			}
			if(i!=dim){
				cout<<"error in input file"<<endl;
				return -1;
			}
			v[counter-1] = new myvector(namin,map,dim,0);
			//myvector* v= new myvector(namin,map,dim,0);
			for(int i=0;i<L;i++){
					hmap[i]->insert(v[counter-1],metric);
			}
			list->insert(v[counter-1],NULL);
			delete []map;
			delete []namin;
		}
		fclose(stream);
		/////query file search
	    stream = fopen(name_q, "r");
	    if (stream == NULL) {
	        perror("fopen");
	        exit(EXIT_FAILURE);
	    }

	    ofstream output;
	    output.open(name_o);

        ///First line
        double R;
        if((getline(&linee, &len, stream)) != -1){
        	char* pc;
			pc = strtok(linee," ");
	        if(strcmp(pc,"Radius:")==0){
	        	pc = strtok(NULL,"\n");
	        	R = atof(pc);
	        }

    	}
    	cout<<R<<endl;

	    numq=0;
	    while ((getline(&linee, &len, stream)) != -1) {
	    	numq++;
	       	i=0;
			//cout<<linee<<endl;
			int* map=new int[dim];
			char* pch;
			sprintf(str,"%d",numq);
	        char* nam1=new char[strlen(nameidS)+2+strlen(str)];
	        strcpy(nam1,nameidS);
			strcat(nam1,str);
			namin=new char[strlen(nam1)+1];
			strcpy(namin,nam1);
			delete []nam1;
			pch = strtok(linee," \t");
			while(pch!=NULL){
				int num=atoi(pch);
				map[i]=num;
				pch=strtok(NULL,"  \t");
				i++;
			}
			if(i!=dim){
				cout<<"error in input file"<<endl;
				return -1;
			}

			myvector* v= new myvector(namin,map,dim,0);

			output<<"Query: "<<namin<<endl;

			///LSH Range search
			if(R!=0){
				for(int i=0;i<L;i++){
					hmap[i]->Rangesearch(v,metric,R,output);
				}

			}

			///exhausted NN search
			clock_t time1,time2;
			time1=clock();
			myvector* ress=list->exhaustedsearch(v,metric,k);
			//cout<<ress->nameid<<" "<<ress->dis<<endl;
			time2=clock();
			double tReal=(double)(time2-time1)/CLOCKS_PER_SEC;
			sum2+=tReal;

			///LSH NN search
			time1=clock();
			myvector* res=hmap[0]->search(v,metric);
			double min;
			if(res==NULL){
				min=1000000;
			}
			else{
				min=res->dis;
			}
			for(int i=1;i<L;i++){
				myvector* temp=hmap[i]->search(v,metric);
				if(temp!=NULL){
					if((temp->dis)<min){
						res=temp;
						min=res->dis;
					}
				}
			}
			time2=clock();

			//statistics 
			double tLSH = (double)(time2-time1)/CLOCKS_PER_SEC;
			sum+=tLSH;
			output<<"Nearest neighbor: "<<res->nameid<<endl<<"distanceLSH: "<<res->dis<<endl<<"distanceTrue: "<<ress->dis<<endl;
			output<<"tLSH: "<<tLSH<<endl<<"tReal: "<<tReal<<endl;
			//cout<<(res->dis/ress->dis)<<endl;
			if((res->dis/ress->dis)>max_approximation){
				max_approximation=res->dis/ress->dis;
			}
			sum3+=res->dis/ress->dis;


			delete v;
			delete []map;
			delete []namin;

	    }
	    output.close();
		free(linee);
		fclose(stream);


	}
	//cout<<dim<<endl;
	//cout<<list->length()<<endl;
	//list->display();

	cout<<"Max fraction: "<<max_approximation<<endl;
	cout<<"Mean fraction: "<<sum3/numq<<endl;
	cout<<"Mean tLSH: "<<sum/numq<<endl;
	cout<<"Mean tReal: "<<sum2/numq<<endl;


  	for(int i=0;i<L;i++){
  		delete hmap[i];
  	}
  	delete []hmap;

  	for(int i=0;i<numoflines-1;i++){
  		delete v[i];
  	}
  	delete []v;

	delete list;
	delete []name_d;
	delete []name_q;
	delete []name_o;
	delete []nameid;
	delete []nameidS;

	return 0;

}