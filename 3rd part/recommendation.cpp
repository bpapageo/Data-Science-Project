#include "cluster.h"
#include "structures.h"
#include "lsh.h"

using namespace std;

#define SIZE 5000
#define USERSTYPE 1

int main(int argc,char *argv[]){
	FILE *stream;
    char *linee = NULL,*name_d,*name_l,*name_cl,*name_o,*name_o2,*name_o3;
    size_t len = 0;
	int numoflines=0,i=0,counter,numofusr;
	int flag1=0,flag2=0,flag3=0,flag4=0,dim,type,validate=0;
	bool metric;
	unordered_map<string,double> lexicon;
	unordered_map<string,int> coinslexicon;
	unordered_map<int,int> userslexicon;
	unordered_map<int,int> tweetslexicon;
	string coinames[COINSIZE];
	srand(time(NULL));
	tweet** map;
	if(argc<9){
		cout<<"Incorrect num of arguments.Try again!"<<endl;
		return -1;
	}
	for(int i=1;i<argc;i+=2){
		if(strcmp(argv[i],"-d")==0){
			flag1=1;
			name_d=new char[strlen(argv[i+1])+1];
			strcpy(name_d,argv[i+1]);
		}
		else if(strcmp(argv[i],"-l")==0){
			flag2=1;
			name_l=new char[strlen(argv[i+1])+1];
			strcpy(name_l,argv[i+1]);
		}
		else if(strcmp(argv[i],"-cl")==0){
			flag3=1;
			name_cl=new char[strlen(argv[i+1])+1];
			strcpy(name_cl,argv[i+1]);
		}
		else if(strcmp(argv[i],"-o")==0){
			flag4=1;
			name_o=new char[strlen(argv[i+1])+1];
			strcpy(name_o,argv[i+1]);
		}
		else if(strcmp(argv[i],"-o2")==0){
			name_o2=new char[strlen(argv[i+1])+1];
			strcpy(name_o2,argv[i+1]);
		}
		else if(strcmp(argv[i],"-o3")==0){
			name_o3=new char[strlen(argv[i+1])+1];
			strcpy(name_o3,argv[i+1]);
		}
		else if(strcmp(argv[i],"-validate")==0){
			validate=1;
		}
	}
	if((flag1==1)&&(flag2==1)&&(flag3==1)&&(flag4==1)){
		///hasharisma sentiments ap to lexicon
	    stream = fopen(name_l, "r");
	    if (stream == NULL) {
	        perror("fopen");
	        exit(EXIT_FAILURE);
	    }
	    while ((getline(&linee, &len, stream)) != -1) {
	    	//cout<<linee<<endl;
	    	char* pc;
			pc = strtok(linee," \t");
			char* key = new char[strlen(pc)+1];
			strcpy(key,pc);
			pc=strtok(NULL," \n");
			double value=atof(pc);
			lexicon[(string)key]=value;
	    }
		fclose(stream);

		///hasharisma coins lexicon
	    stream = fopen(name_cl, "r");
	    if (stream == NULL) {
	        perror("fopen");
	        exit(EXIT_FAILURE);
	    }
	    while ((getline(&linee, &len, stream)) != -1) {
	    	char* pc;
			pc = strtok(linee,"\t");
			coinames[numoflines] = (string)pc;
			int num=0;
			while(pc!=NULL){
				char* key = new char[strlen(pc)+1];
				strcpy(key,pc);
				if(strlen(key)!=0 && key[strlen(key)-1] == '\n'){
					key[strlen(pc)-2] = '\0';
				}
				coinslexicon[(string)key]=numoflines+1;
				if(num==4){
					coinames[numoflines]=(string)key;
				}
				pc=strtok(NULL,"\t");
				num++;
			}
		    numoflines++;
	    }

    	/*for (auto x : coinslexicon){ 
      		cout<< x.first << " " << x.second << endl; 
      	}*/
		fclose(stream);

    	///save the scores
        stream = fopen(name_d, "r");
        if (stream == NULL) {
        	perror("fopen");
        	exit(EXIT_FAILURE);
        }
    	counter=0;
    	map=new tweet*[SIZE];
        while ((getline(&linee, &len, stream)) != -1) {
        	int flags[COINSIZE];
			for(int i=0;i<COINSIZE;i++){
				flags[i]=0;
			}
			char* pch;
			pch = strtok(linee," \t");
			int userid = atoi(pch);
			pch=strtok(NULL,"  \t");
			int tweetid = atoi(pch);
			pch=strtok(NULL,"  \t");
			double sum=0;
			while(pch!=NULL){
				if(strlen(pch)!=0 && pch[strlen(pch)-1] == '\n'){
					pch[strlen(pch)-2] = '\0';
				}
				if(coinslexicon[pch]!=0){
					flags[coinslexicon[pch]-1]+=1;
					pch=strtok(NULL,"  \t");
					continue;
				}
				sum+=lexicon[pch];
				pch=strtok(NULL,"  \t");
			}
			sum=sum/sqrt(pow(sum,2.0)+15.0);
			tweetslexicon[tweetid]=counter;

			map[counter]=new tweet(userid,tweetid,sum,flags);
	       	counter++;
		}
		fclose(stream);
		myvector** users;
		if(USERSTYPE==1){
			type=1;
			///Construct the users and their vectors
			int temp=map[0]->user_id;
			counter=1;
			for (int i = 0; i < SIZE; i++)
			{
				if((map[i]->user_id)!=temp){
					counter++;
					temp=map[i]->user_id;
				}
			}
			cout<<counter<<endl;
			numofusr=counter;
			int* f=new int[COINSIZE];
			users=new myvector*[counter];
			int fullusers=counter;
			long double *vec=new long double[COINSIZE];
			for (int i = 0; i < COINSIZE; i++)
			{
				vec[i]=numeric_limits<long double>::infinity();
				f[i]=0;
			}
			temp=map[0]->user_id;
			counter=0;
			for (int i = 0; i < SIZE; i++)
			{
				if((map[i]->user_id)!=temp){
					users[counter]=new myvector(temp,vec,COINSIZE,1);
					userslexicon[temp]=counter;			
					counter++;
					temp=map[i]->user_id;
					for (int k = 0; k < COINSIZE; k++)
					{
						vec[k]=numeric_limits<long double>::infinity();
						users[counter-1]->points[k]=f[k];
						f[k]=0;
					}
				}
				for (int j = 0; j < COINSIZE; j++)
				{
					if (map[i]->flags[j]!=0)
					{
						f[j]=1;
						if(vec[j]==numeric_limits<long double>::infinity()){
							vec[j]=(map[i]->flags[j])*(map[i]->score);
						}
						else{
							vec[j]+=(map[i]->flags[j])*(map[i]->score);
						}
					}
				}
			}
		}	
		else if(USERSTYPE==2){
			type=2;
			///Construct the virtual users and their vectors
			counter=100;
			int* f=new int[COINSIZE];
			users=new myvector*[counter];
			numofusr=counter;
			int fullusers=counter;
			long double *vec=new long double[COINSIZE];
			for (int i = 0; i < COINSIZE; i++)
			{
				vec[i]=numeric_limits<long double>::infinity();
				f[i]=0;
			}
	        stream = fopen("outputof2nd_inputof3rd", "r");
	        if (stream == NULL) {
	        	perror("fopen");
	        	exit(EXIT_FAILURE);
	        }
	        counter=0;
	        while ((getline(&linee, &len, stream)) != -1) {
				char* pch;
				pch = strtok(linee,"{");
				int userid = atoi(pch);
				pch=strtok(NULL,",");
				while(pch!=NULL){
					int tweetid=atoi(pch);
					for (int j = 0; j < COINSIZE; j++)
					{
						if (map[tweetslexicon[tweetid]]->flags[j]!=0)
						{
							f[j]=1;
							if(vec[j]==numeric_limits<long double>::infinity()){
								vec[j]=(map[tweetslexicon[tweetid]]->flags[j])*(map[tweetslexicon[tweetid]]->score);
							}
							else{
								vec[j]+=(map[tweetslexicon[tweetid]]->flags[j])*(map[tweetslexicon[tweetid]]->score);
							}
						}
					}
					pch=strtok(NULL,",");
				}
				users[counter]=new myvector(counter,vec,COINSIZE,1);
				userslexicon[counter]=counter;			
				for (int k = 0; k < COINSIZE; k++)
				{
					vec[k]=numeric_limits<long double>::infinity();
					users[counter]->points[k]=f[k];
					f[k]=0;
				}
				counter++;
	        }

		}	
	    ofstream output,output2;
	    output.open(name_o2);
	    output2.open(name_o3);
	    int numofusers=0;
	    for (int i = 0; i < counter; i++){
		    output<<users[i]->id<<" ";
		    long double sum=0;
		    int count=0;
		    int flag=0;
			for(int j=0;j<users[i]->d;j++){
					output<<users[i]->pointsd[j]<<" ";
					if(users[i]->pointsd[j]!=numeric_limits<long double>::infinity()){
						if(users[i]->pointsd[j]!=0)flag=1;
						count++;
						sum+=users[i]->pointsd[j];
					}
			}
			output<<endl;
			sum=sum/count;
			users[i]->mid=sum;
			for(int j=0;j<users[i]->d;j++){
					if(users[i]->pointsd[j]==numeric_limits<long double>::infinity()){
						users[i]->Cpointsd[j]=sum;
					}
					else{
						users[i]->Cpointsd[j]=users[i]->pointsd[j];	
					}

			}
			if(flag==1){
				numofusers++;
		    	output2<<users[i]->id<<" ";
				for(int j=0;j<users[i]->d;j++){
					output2<<users[i]->Cpointsd[j]<<" ";
				}
				output2<<endl;
			}
		}
	    output.close();
	    output2.close();
	    cout<<numofusers<<endl;
	    myvector** clusterusers=new myvector*[numofusers];
	    myvectorlsh** lshusers=new myvectorlsh*[numofusers];
        stream = fopen(name_o3, "r");
        if (stream == NULL) {
        	perror("fopen");
        	exit(EXIT_FAILURE);
        }
        counter=0;
        while ((getline(&linee, &len, stream)) != -1) {
			char* pch;
			long double* map=new long double[COINSIZE];
			pch = strtok(linee," \t");
			int userid = atoi(pch);
			pch=strtok(NULL,"  \t");
			int sum=0;
			while(pch!=NULL){
				if(sum<COINSIZE){
					map[sum]=stold(pch);
				}
				pch=strtok(NULL,"  \t");
				sum++;
			}
			clusterusers[counter]=new myvector(userid,map,COINSIZE,1);
			lshusers[counter]=new myvectorlsh(userid,map,COINSIZE,1);
			counter++;
        }
        for (int i = 0; i < counter; i++)
        {
	       	for(int j=0;j<COINSIZE;j++){
	        	clusterusers[i]->points[j]=users[userslexicon[clusterusers[i]->id]]->points[j];
	        	lshusers[i]->points[j]=users[userslexicon[clusterusers[i]->id]]->points[j];
	        	clusterusers[i]->mid=users[userslexicon[clusterusers[i]->id]]->mid;
	        	lshusers[i]->mid=users[userslexicon[clusterusers[i]->id]]->mid;
	        }
        }
	    hashtable** hmap = new hashtable*[5];
		for(int i=0;i<5;i++){
			hmap[i]=new hashtable(counter/4,4,COINSIZE,1);
		}	
		for(int j=0;j<counter;j++){
			for(int i=0;i<5;i++){
				hmap[i]->insert(lshusers[j],1);
			}
		}

		output.open(name_o);
		LSHRecommendation(hmap,lshusers,counter,coinames,output,type);
		if(USERSTYPE==1)ClusteringRecommendation(clusterusers,counter,coinames,output,150,type);
		else if(USERSTYPE==2)ClusteringRecommendation(clusterusers,counter,coinames,output,20,type);
		output.close();

       	free(linee);
		fclose(stream);

		if((validate==1)&&(type==1)){
			Validation(lshusers,counter);
		}



	}
	delete []name_d;
	delete []name_cl;
	delete []name_l;
	delete []name_o;
	delete []name_o2;
	delete []name_o3;

	return 0;

}