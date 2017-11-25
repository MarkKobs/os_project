/* Project name : Process Schedualing
 * Author : Mark Kobs
 * Date : 2017.11.25
 *
 * structure design:
 * 1.PCB
 * 2.priority_queue based on max_heap(binary heap)

 * functions:
 * 1(1)initialize
 * 1(2)lessen
 * 2(1)initialize by inserting
 * 2(2)insert
 * 2(3)delete(remaining)
 * main(imitating process schedualing in modern computer)
 */
#include<stdio.h>
#include<stdlib.h>
typedef struct PCB PCB;
typedef struct max_heap *priority_queue;
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
        printf("%d\n",H->pcb_array[0].p_requireTime);
}
int main(){
        //PCB *p=initialize_pcb(0,0,0,' ');
       // printf("%d",p->p_id);
       priority_queue p=initialize_queue(5);
        return 0;
}
