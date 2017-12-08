#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#define CAPACITY 10
int main(int argc,char **argv);
typedef struct io_node *ioNode;
typedef struct io_table *ioTable;
void print_table(ioTable it);
struct io_node{
        int id;
        int cylinder_number;
        int track_number;
        int record;
        char direction;//u:up inward  d:down outward
};
struct io_table{
    int size;
    int capacity;
    ioNode io_array;
};

/*init the io table*/
ioTable init_table(){
        ioTable it=(ioTable)malloc(sizeof(struct io_table));
        it->io_array=(ioNode)malloc(sizeof(struct io_node)*CAPACITY);
        it->size=0;
        it->capacity=CAPACITY;
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


void schedual(){
        printf("**********************drive schedualing**********************\n");
}
int isFull(ioTable it){
        return it->size==it->capacity;
}
void receive(ioTable it){
        if(isFull(it)){
                printf("the table is full! wait for the drive process\n");
                return;
        }
        ioNode in=&it->io_array[it->size];
        printf("++++++++++++++++++++receive the requset++++++++++++++++++++\n");
        scanf("%d %d %d %d %c",&in->id,&in->cylinder_number,&in->track_number,&in->record,&in->direction);
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
                	schedual();
        	}
        	else{
                	receive(it);
        	}
		
            printf("continue ? input y or Y if you want to continue, or any other key to stop now\n");
            hint=getchar();
            getchar();//read the return 
            time++;
            ran=get_random();
        }
        
        return 0;

}
