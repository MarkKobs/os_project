/* 课堂上老师进行了提问，对程序中消费者函数和生产者程序到底哪个先进行产生了疑问，调换程序块的顺序是否会对程序结果产生影响？
 * 我的观点：没有影响
 * 假设消费者在前，生产者在后，因为我在程序中是使用POSIX线程的方式实现的，所以消费者子线程和生产者子线程都将被主线程唤起，
 * 我们都知道计算机系统并发的模型，虽然线程之间共享资源，但是运行相对独立。（特点：并发、资源共享且运行相对独立）所以，无论
 * 你的电脑是单核还是多核，并发总是存在的。
 * 那么，什么情况就是有影响的了呢？
 * pthread_join() 函数要求，假如子线程被当前线程join了，那么子线程结束前，主线程将被阻塞。
 *
 * 还有一些疑问也一起写在这里：
 * 1.pthread_create的参数是什么意思：第一个参数是指线程地址，存放了它的一些属性，比如线程号，程序计数器、若干堆栈信息啊，一些有关于线程的信息
 * 第二个参数是pthread_attr_t结构的地址，一般初始化为NULL，比如默认设置为joinable属性
 * 第三个参数是被调用的函数名，第四个就是被调函数的参数。
 * 2.实际上sem_t和pthread_t什么的设想都没错，就是信号量，和线程号。只不过是结构体，要想调用值输出就用int sem_getvalue(sem_t* sem,int *sval)就ok.
 * example:
 *       int value; 
 *       sem_getvalue(&sem_name, &value); 
 *       printf("The value of the semaphors is %d\n", value);
 *
 * */

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
