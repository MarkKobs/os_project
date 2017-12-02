
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<pthread.h>
#include<unistd.h>
#include<signal.h>
#include<semaphore.h>
#define Maxbuf 100
#define TimeOfOp 100
#define true 1
#define false 0
//data structure 
struct CircleBuf
{
    int read;
    int write;
    int buf[Maxbuf];
}  circlebuf;

sem_t mutex;
sem_t empty;
sem_t full;

void writeCircleBuf(struct CircleBuf *circlebuf,int *value){//  =  v
    circlebuf->buf[circlebuf->write]=(*value);
    circlebuf->write=(circlebuf->write+1)%Maxbuf;
}

int readCircleBuf(struct CircleBuf *circlebuf){// =  p
    int value=0;
    value=circlebuf->buf[circlebuf->read];
    circlebuf->buf[circlebuf->read]=0;// reset buf[read]=0
    circlebuf->read=(circlebuf->read+1)%Maxbuf;
    return value;
}

void OutCirclebuf(struct CircleBuf *circlebuf){
    int i;
    printf("******************the size in each buffer:");
    for(int i=0;i<Maxbuf;i++){
        printf("%d ",circlebuf->buf[i]);
    }
    printf("\n\n");
}

void sign(int sig){
    exit(0);
}

void* productThread(void *i){//producer thread
    int *n=(int *)i;//thread id
    int t=TimeOfOp;
    while(t--){
    sem_wait(&empty);//sem_t empty -1
	sem_wait(&mutex);//sem_t mutex -1
	writeCircleBuf(&circlebuf,n);//put thread id into buffer
	printf("++++++++++++++++++producer %d put value=%d into buffer.\n",*n,*n);
	sem_post(&mutex);
	sem_post(&full);
    //usleep(50000);
    }
}

void * consumerThread(void *i){
    int *n=(int*)i;//thread id
    int value=0;
    int t=TimeOfOp;
    while(t--){
        sem_wait(&full);
        sem_wait(&mutex);
        value=readCircleBuf(&circlebuf);
    	printf("-----------------consumer %d take value=%d ouf buffer.\n",*n,value);
        	
//	OutCirclebuf(&circlebuf);
	sem_post(&mutex);
	sem_post(&empty);
   // usleep(50000);
    }
}
int main(){
    int i;
    pthread_t cpid,ppid; 
    sem_init(&mutex,0,1);//mutex init 1 
    sem_init(&empty,0,Maxbuf);
    sem_init(&full,0,0);

    signal(SIGINT,sign);
    signal(SIGTERM,sign);
    circlebuf.read=circlebuf.write=0;
    for(int i=0;i<Maxbuf;i++) circlebuf.buf[i]=0;
    int id=1;
    pthread_create(&cpid,NULL,consumerThread,(void*)&id);
    pthread_create(&ppid,NULL,productThread,(void*)&id);
    pthread_join(cpid,NULL);
    pthread_join(ppid,NULL);
    sem_destroy(&mutex);
    sem_destroy(&empty);
    sem_destroy(&full);
    pthread_exit(NULL);
    return 0;
}
