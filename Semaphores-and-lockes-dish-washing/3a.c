#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<semaphore.h>
sem_t mutex;
int m,n;
int *data;
int *lock;
void *editor(void *param);
int *countt;
int main(int argc,char *argv[]){
	int i;
	//char *u=(char *)malloc(2);
	n=atoi(argv[1]);
	m=atoi(argv[2]);
	data=malloc(sizeof(int)*(m+1));
	lock=malloc(sizeof(int)*(m+1));
	countt=malloc(sizeof(int)*(n+1));
	sem_init(&mutex,0,1);
	for(i=0;i<=m;i++){
		data[i]=0;
	}
	for(i=0;i<=m;i++){
		lock[i]=0;
	}
	pthread_t tid[n+1];
	pthread_attr_t attr;
	for(i=1;i<=n;i++){
		countt[i]=0;
		char *u=(char *)malloc(2);
		sprintf(u,"%d",i);
		//printf("given=%d\n",atoi(u));
		pthread_attr_init(&attr);
		pthread_create(&tid[i],&attr,editor,u);
	}
	for(i=1;i<=n;i++){
		pthread_join(tid[i],NULL);
	}
	int j;
	for(i=1;i<=n;i++){
		//printf("editor %d : count %d : ",i,countt[i]);
		printf("%d : ",countt[i]);
		for(j=1;j<=m;j++){
			if(data[j]==i){
				printf("%d ",j);
			}
		}
		printf("\n");
	}
	return 0;
}
void *editor(void *param){
	int ed=atoi(param);
	//printf("in editor=%d\n",ed);
	int i,j=1;
	int rej[m+1];
	for(i=0;i<=m;i++){
		rej[i]=0;
	}
	while(1){
		j=1;
		for(i=1;i<=m;i++){
			if(data[i]==0 && rej[i]==0 && lock[i]==0){
				j++;
			}
		}
		if(j==1)break;
		sem_wait(&mutex);
		int r=(rand()%m)+1;
		if(data[r]==0 && rej[r]==0 && lock[r]==0){
			lock[r]=1;
			sem_post(&mutex);
			sleep(1);
			if(rand()%2==0){
				countt[ed]++;
				data[r]=ed;
			}
			else{
				lock[r]=0;
				rej[r]=1;
				continue;
			}
		}
		else{
			sem_post(&mutex);
		}
	}
	pthread_exit(0);
}
