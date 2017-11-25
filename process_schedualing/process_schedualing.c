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
    char state;//E:end,R:ready
};

struct max_heap{
        int heap_capacity;
        int size;
        PCB *pcb_array;
};
int main(){
        return 0;
}
