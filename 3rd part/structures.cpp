#include "structures.h"
#include "lsh.h"
#include "cluster.h"

tweet::tweet(int uid,int tid,double sum,int f[COINSIZE]){
	user_id=uid;
	tweet_id=tid;
	score=sum;
	//cout<<user_id<<" "<<tweet_id<<" "<<score<<endl;
	for(int i=0;i<COINSIZE;i++){
		flags[i]=f[i];
		//cout<<flags[i]<<" ";
	}
	//cout<<endl;
}
