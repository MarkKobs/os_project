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
        H->pcb_array[0]=initialize_pcb(&H->pcb_array[0],0,0,0,' ');
        return H;
}
int isFull(priority_queue const H){// 1 full, 0 not full
        if(H->size==H->capacity){
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
        for(i=++H->size;H->pcb_array[i/2].p_priority>priority;i/=2){//change the place of the tree.
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
void pcb_lessen(PCB * const pcb){
        if(pcb==NULL){
                printf("Null Pcb* , Lessen Error! \n");
        }
        else{
                if(pcb->p_requireTime==0){
                        printf("P%d\'s state is \'End\'\n",pcb->p_id);
                }
                else{//requireTime>0
                        pcb->p_requireTime-=1;
                        pcb->p_priority-=1;
                        printf("P%d lessen successfully,current requireTime:%d , priority:%d\n",pcb->p_id,pcb->p_requireTime,pcb->p_priority);
                        if(pcb->p_requireTime==0){
                         //       pcb->p_state='E';
                                printf("P%d\'s state changed to \'End\'\n",pcb->p_id);
                        }
                }
        }
}

int main(){
        priority_queue p=initialize_queue(5);
        printf("%d",p->pcb_array[0].p_id);

        pcb_lessen(&p->pcb_array[0]);
        return 0;
}
