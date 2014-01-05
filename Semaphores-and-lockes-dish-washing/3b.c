#include <unistd.h>     /* Symbolic Constants */
#include <sys/types.h>  /* Primitive System Data Types */ 
#include <errno.h>      /* Errors */
#include <stdio.h>      /* Input/Output */
#include <stdlib.h>     /* General Utilities */
#include <pthread.h>    /* POSIX Threads */
#include <string.h>     /* String handling */
#include <semaphore.h>  /* Semaphore */
#include<time.h>
void *wash(void *param);
int critical(int index,int sno,int s);
int m,n;
time_t st,e;
int *end;
int *tap;
int *sponge;
sem_t mutexT;
sem_t *mutexS;
int main(int argc,char *argv[]){
	int i;
	n=atoi(argv[1]);
	m=atoi(argv[2]);
	end=malloc(sizeof(int)*(m+1));
	tap=malloc(sizeof(int)*(n+1));
	sponge=malloc(sizeof(int)*(n));
	mutexS=malloc(sizeof(sem_t)*(n));
	pthread_t tid[m+1];
	pthread_attr_t attr;
	sem_init(&mutexT,0,1);
	for(i=0;i<n;i++){
		sem_init(&mutexS[i],0,1);
	}
	time(&st);
	for(i=0;i<n;i++){
		tap[i]=0;
		sponge[i]=0;
	}
	tap[n]=0;
	for(i=1;i<=m;i++){
		char *u=(char *)malloc(2);
		sprintf(u,"%d",i);
		pthread_attr_init(&attr);
		pthread_create(&tid[i],&attr,wash,u);
	}
	for(i=1;i<=m;i++){
		pthread_join(tid[i],NULL);
	}
	for(i=1;i<=m;i++){
		printf("%d : %d\n",i,end[i]);
	}
	sem_destroy(&mutexT);
	for(i=0;i<n;i++){
		sem_destroy(&mutexS[i]);
	}
	return 0;
}
void *wash(void *param){
	int s=atoi(param);
	int index=1,sno;
	sem_wait(&mutexT);
	while(1){
		if(tap[index]==0){
			tap[index]=1;
			sem_post(&mutexT);
			break;
		}
		else{
			index=(index)%n+1;
		}
	}
//	printf("student %d occupied %d tap\n",s,index);
	sno=critical(index,sno,s);
	sleep(4);
//	printf("released sponge = %d\n",sno);
	sponge[sno]=0;
	sleep(5);
	sno=critical(index,sno,s);
	sleep(3);
//	printf("released sponge = %d\n",sno);
	sponge[sno]=0;
	sleep(3);
	sno=critical(index,sno,s);
	sleep(2);
//	printf("released sponge = %d\n",sno);
	sponge[sno]=0;
	sleep(1);
//	printf("tap released = %d\n",index);
	tap[index]=0;
	time(&e);
	end[s]=difftime(e,st);
	pthread_exit(0);
}
int critical(int index,int sno,int s){
	if(index==1){
		while(1){
			sem_wait(&mutexS[1]);
			if(sponge[1]==0){
				sponge[1]=1;
				sem_post(&mutexS[1]);
				sno=1;
//				printf("%d took sponge %d\n",s,sno);
				break;
			}
			else{
				sem_post(&mutexS[1]);
			}
		}
	}
	else if(index==n){
		while(1){
			//printf("enter index=%d\n",index);
			sem_wait(&mutexS[n-1]);
			if(sponge[n-1]==0){
				sponge[n-1]=1;
				sem_post(&mutexS[n-1]);
				sno=n-1;
//				printf("%d took sponge %d\n",s,sno);
				break;
			}
			else{
				sem_post(&mutexS[n-1]);
			}
		}
	}
	else{
		while(1){
			sem_wait(&mutexS[index-1]);
			if(sponge[index-1]==0){
				sponge[index-1]=1;
				sem_post(&mutexS[index-1]);
				sno=index-1;
//				printf("%d took sponge %d\n",s,sno);
				break;
			}
			else{
				sem_post(&mutexS[index-1]);
			}
			sem_wait(&mutexS[index]);
			if(sponge[index]==0){
				sponge[index]=1;
				sem_post(&mutexS[index]);
				sno=index;
//				printf("%d took sponge %d\n",s,sno);
				break;
			}
			else{
				sem_post(&mutexS[index]);
			}
		}
	}
	return sno;
}
