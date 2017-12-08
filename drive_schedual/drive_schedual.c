#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>
#define CAPACITY 10
typedef struct io_node *ioNode;
typedef struct io_table *ioTable;
static void print_current(ioTable it);
static int current_cylinder=0;//current cylinder : default 0
static int current_track=0;
static int current_record=0;
int main(int argc,char **argv);
void print_table(ioTable it);
ioTable init_table();
int get_random();
int isEmpty(ioTable it);
int has_bigger(ioTable it);
int smallest_in_big(ioTable it);
int biggest_in_small(ioTable it);
void delete_io(ioTable it,int k);
void schedual(ioTable it);
int isFull(ioTable it);
void receive(ioTable it);
void print_node(ioNode in);
struct io_node{
        int id;
        int cylinder_number;
        int track_number;
        int record;
        char direction;
};
struct io_table{
    int size;
    int capacity;
    ioNode io_array;
    char direction;//u:up inward  d:down outward
};

/*init the io table*/
ioTable init_table(){
        ioTable it=(ioTable)malloc(sizeof(struct io_table));
        it->io_array=(ioNode)malloc(sizeof(struct io_node)*CAPACITY);
        it->size=0;
        it->capacity=CAPACITY;
        it->direction='u';
/*
        it->io_array[0].id=0;
        it->io_array[0].cylinder_number=0;
        it->io_array[0].track_number=0;
        it->io_array[0].record=0;
        it->size++;
        */
        return it;
}
void print_table(ioTable it){
        printf("Here is the request i/o table:\n");
        for(int i=0;i<it->size;i++){
                ioNode temp=&(it->io_array[i]);
                printf("P%d %d %d %d %c\n",temp->id,temp->cylinder_number,temp->track_number,temp->record,temp->direction);
        }
}
/*get random number:1 or 0*/
int get_random(){
        srand((unsigned)time(NULL));
        return (rand()%2);
}
int isEmpty(ioTable it){
        return it->size==0;
}
int has_bigger(ioTable it){
        for(int i=0;i<it->size;i++){
                if(it->io_array[i].cylinder_number > current_cylinder){
                        return 1;
                }
        }
        return 0;
}
int has_smaller(ioTable it){
        for(int i=0;i<it->size;i++){
                if(it->io_array[i].cylinder_number < current_cylinder){
                        return 1;
                }
        }
        return 0;
}
int smallest_in_big(ioTable it){//return index
        int j=0;
        int x=199;//max is 199
        for(int i=0;i<it->size;i++){
                if(it->io_array[i].cylinder_number > current_cylinder && it->io_array[i].cylinder_number<x){
                        j=i;
                        x=it->io_array[i].cylinder_number;
                }
        }
        return j;
}
int biggest_in_small(ioTable it){
        int j=0;
        int x=0;
        for(int i=0;i<it->size;i++){
                if(it->io_array[i].cylinder_number < current_cylinder && it->io_array[i].cylinder_number > x){
                        j=i;
                        x=it->io_array[i].cylinder_number;
                }
        }
        return j;
}
void delete_io(ioTable it,int k){
        printf("delete: ");
        print_node(&it->io_array[k]);
        for(int i=k;i<it->size-1;i++){
                it->io_array[i]=it->io_array[i+1];
        }
        it->size--;
}
void print_node(ioNode in){
        printf("P%d %d %d %d %c\n",in->id,in->cylinder_number,in->track_number,in->record,in->direction);
}
void schedual(ioTable it){
       // printf("**********************drive schedualing**********************\n");
        if(isEmpty(it)){
                //printf("the I/O table NULL!\n");
                return;
        }
        else{
                int shortest=32000;
                int chosen=0;
                for(int i=0;i<it->size;i++){
                        if(current_cylinder==it->io_array[i].cylinder_number){//have the same cylinder number
                                /*find the shortest*/
                                int distance=abs(it->io_array[i].track_number-current_track)+abs(it->io_array[i].record-current_record);
                                if(distance < shortest){
                                        shortest=distance;
                                        chosen=i;
                                }
                        }
                        else{//do not have the same cylinder number
                                if(it->direction=='u'){//inward
                                        if(has_bigger(it)){
                                                chosen=smallest_in_big(it);
                                        }
                                        else{
                                                it->direction='d';
                                                chosen=biggest_in_small(it);
                                        }
                                }
                                else{//outward , 'd'
                                        if(has_smaller(it)){
                                                chosen=biggest_in_small(it);
                                        }
                                        else{
                                                it->direction='u';
                                                chosen=smallest_in_big(it);
                                        }
                                }
                        }
                }
                it->io_array[chosen].direction=it->direction;
                current_cylinder=it->io_array[chosen].cylinder_number;
                current_track=it->io_array[chosen].track_number;
                current_record=it->io_array[chosen].record;
                print_current(it);
                delete_io(it,chosen);
        }
}
int isFull(ioTable it){
        return it->size==it->capacity;
}

static void print_current(ioTable it){
        printf("current state: %d %d %d %c\n",current_cylinder,current_track,current_record,it->direction);
}

void receive(ioTable it){
        if(isFull(it)){
                printf("the table is full! wait for the drive process\n");
                exit(-1);
        }
        ioNode in=&it->io_array[it->size];
        printf("++++++++++++++++++++receive the requset++++++++++++++++++++\n");
        scanf("%d %d %d %d",&in->id,&in->cylinder_number,&in->track_number,&in->record);
        getchar();
        it->size++;
        print_table(it);
}

int main(int argc,char **argv){
        int ran=get_random();
        ioTable it=init_table();
        char hint='y';
        int time=0;
        while(hint=='y'||hint=='Y'){
			if(ran==1){
                	schedual(it);
        	}
        	else{
                	receive(it);
        	}
		
           // printf("continue ? input y or Y if you want to continue, or any other key to stop now\n");
           // hint=getchar();
           // getchar();//read the return 
            time++;
            ran=get_random();
        }
        
        return 0;

}
