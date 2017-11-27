/* Project name : Process Schedualing
 * Author : Mark Kobs
 * Date : 2017.11.25
 *
 * structure design:
 * 1.PCB
 * 2.priority_queue based on max_heap(binary heap)

 * functions:
 * 1.1 initialize
 * 1.2 lessen
 * 2.1 initialize by inserting
 * 2.2 insert
 * 2.3 delete(remaining)
 * main (imitating process schedualing in modern computer)
 */

#include<unistd.h>//use usleep
#include<limits.h>//use MAX_INT
#include<stdio.h>
#include<stdlib.h>

//declaration
typedef struct PCB PCB;
typedef struct max_heap *priority_queue;
PCB initialize_pcb(PCB* const pcb,int id,int requireTime,int priority,char state);
void heap_insert(priority_queue const H,int id,int requireTime,int priority,char state);
priority_queue initialize_queue(int element_length);
void pcb_lessen(PCB * const pcb);
int isFull(priority_queue const H);
void init_heap_data(priority_queue const H);//put the pcb data into heap by inserting
void pcb_print(int id,int requireTime,int priority,char state);
void heap_print(priority_queue const H);
int isEmpty(priority_queue const H);
void percolateDown(priority_queue const H,int i);
void reconstruct_heap(priority_queue const H);
void pcb_swap(PCB *p1,PCB *p2);
void delete_pcb(priority_queue const H);

struct PCB{
    int p_id;
    int p_requireTime;
    int p_priority;
    char p_state;//E:end,R:ready
};

struct max_heap{
        int capacity;
        int size;
        PCB *pcb_array;
};

PCB initialize_pcb(PCB* const pcb,int id,int requireTime,int priority,char state){
    //PCB* pcb=malloc(sizeof(struct PCB));
    pcb->p_id=id;
    pcb->p_requireTime=requireTime;
    pcb->p_priority=priority;
    pcb->p_state=state;
    return *pcb;
}

priority_queue initialize_queue(int element_length){
        priority_queue H;
        if(element_length<=0){
                printf("Error,length must be over 0!\n");
        }
        H=malloc(sizeof(struct max_heap));
        if(H==NULL){
                printf("out of space!\n");
        }
        H->pcb_array=malloc(sizeof(struct PCB)*(element_length+1));//the first space for storing empty data.
        if(H->pcb_array==NULL){
                printf("out of space!\n");
        }
        H->capacity=element_length;
        H->size=0;
        H->pcb_array[0]=initialize_pcb(&H->pcb_array[0],0,INT_MAX,INT_MAX,' ');
        return H;
}

int isFull(priority_queue const H){// 1 full, 0 not full
        if(H->size==H->capacity){
                return 1;
        }
        return 0;
}

int isEmpty(priority_queue const H){//1 empty,0 not empty
        if(H->size==0){
                return 1;
        }
        return 0;
}

void heap_insert(priority_queue const H,int id,int requireTime,int priority,char state){
        int i;
        if(isFull(H)){
                printf("Queue is full,\n");
                return;
        }
        //up overflow
        for(i=++H->size;H->pcb_array[i/2].p_priority<priority;i/=2){//change the place of the tree. 
                //and the H->size increase
                H->pcb_array[i]=H->pcb_array[i/2];
        }
        //find the right place , current i is the right place to store input pcb information.
        H->pcb_array[i].p_id=id;
        H->pcb_array[i].p_requireTime=requireTime;
        H->pcb_array[i].p_priority=priority;
        H->pcb_array[i].p_state=state; 
        //change some heap information. (has done , the size has been increased.)
}

void init_heap_data(priority_queue const H){
//put the pcb data into heap by inserting
        int id[H->capacity];
        int requireTime[H->capacity];
        int priority[H->capacity];
        char state[H->capacity];
        printf("Initialize the information of pcbs...\n");
        for(int i=0;i<H->capacity;i++){
                printf("%d-Enter process Id,requiretime,priority,state: \n",(i+1));
                scanf("%d %d %d %c",&id[i],&requireTime[i],&priority[i],&state[i]);
                heap_insert(H,id[i],requireTime[i],priority[i],state[i]);
        }
        printf("Initialized - -!\n");
        heap_print(H);
}

void heap_print(priority_queue const H){
        if(isEmpty(H)){
                printf("the heap is Empty,Error.\n");
                return;
        }
        
        for(int i=1;i<=H->size;i++){
                printf("P%d: requiretime %d,priority %d,state %c.\n",H->pcb_array[i].p_id, H->pcb_array[i].p_requireTime,H->pcb_array[i].p_priority,H->pcb_array[i].p_state);
        }

}

void pcb_print(int id, int requireTime,int priority,char state){
        printf("P%d:requiretime %d,priority %d,state %c\n",id,requireTime,priority,state);
}

void pcb_lessen(PCB * const pcb){
        if(pcb==NULL){
                printf("Null Pcb* , Lessen Error! \n");
        }
        else{
                if(pcb->p_requireTime==0){
                        printf("P%d\'s state is \'End\'\n",pcb->p_id);
                }
                else{//requireTime>0
                        pcb->p_requireTime--;
                        pcb->p_priority--;
                        printf("P%d lessen successfully,current requireTime:%d , priority:%d\n",pcb->p_id,pcb->p_requireTime,pcb->p_priority);
                        if(pcb->p_requireTime==0){
                                pcb->p_state='E';
                                printf("P%d\'s state changed to \'End\'\n",pcb->p_id);
                        }
                }
        }
}

void reconstruct_heap(priority_queue const H){
        for(int i=H->size/2;i>0;i--){//important : i>0 not i>=0
                percolateDown(H,i);//find if there are bigger ones in child nodes.
        }
        heap_print(H);
}

void percolateDown(priority_queue const H,int i){
        int r=i;//root:subscript of the root of subtree where the process will begin
        int c=2*r;//c:subscript of the left child
       // int right_child=2*i+1;
       // int big_child=(H->pcb_array[left_child].p_priority>H->pcb_array[right_child].p_priority)?left_child:right_child;
        int n=H->size;
        while(c<=n){
                if(c<n-1 && H->pcb_array[c].p_priority<H->pcb_array[c+1].p_priority){
                        c++;
                }
                if(H->pcb_array[r].p_priority<H->pcb_array[c].p_priority){
                        pcb_swap(&H->pcb_array[r],&H->pcb_array[c]);//swap r and c
                        r=c;
                        c=2*c;
                }
                else{
                        break;
                }
                c++;
        }
}

void pcb_swap(PCB *p1,PCB *p2){
        PCB *temp=malloc(sizeof(struct PCB));
        if(temp==NULL){
                printf("out of space!\n");
                return;
        }
        *temp=*p1;
        *p1=*p2;
        *p2=*temp;
}

void delete_pcb(priority_queue const  H){
        int i,c;//c:child
        /*last pcb  h->size--*/
        PCB last_pcb=H->pcb_array[H->size--];
        if(isEmpty(H)){
                printf("the priority queue is empty!\n");
                return;
        }
        for(i=1;i*2<=H->size;i=c){
                /*find larger child*/
                c=i*2;//left child
                if(c!=H->size && H->pcb_array[c].p_priority<H->pcb_array[c+1].p_priority){
                        c++;
                }
                /*percolate one level*/
                if(last_pcb.p_priority<H->pcb_array[c].p_priority){//TODO
                        H->pcb_array[i]=H->pcb_array[c];
                }
                else {
                        break;
                }
        }
        H->pcb_array[i]=last_pcb;
        printf("delete ! size : %d--->%d\n",H->size+1,H->size);
}
int main(){
        priority_queue queue=initialize_queue(5);
        init_heap_data(queue);
        PCB *pcb_toRun=&queue->pcb_array[1];//get the next PCB to run, always get the one with biggest priority(pcb_array[1])
        while(queue->size!=0){
                //run
                usleep(3000);
                pcb_lessen(pcb_toRun);
                //first method:1.delete old pcb,2.insert new pcb
                //second method:reconstruct heap
                reconstruct_heap(queue);
               // pcb_toRun=&queue->pcb_array[1];
                pcb_toRun=&queue->pcb_array[1];
                if(pcb_toRun->p_requireTime==0){
                        delete_pcb(queue);
                }
        }
        return 0;
}
