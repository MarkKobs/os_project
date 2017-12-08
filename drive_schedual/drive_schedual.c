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
};
struct io_table{
    int size;
    ioNode io_array;
};

/*init the io table*/
ioTable init_table(){
        ioTable it=(ioTable)malloc(sizeof(struct io_table));
        it->io_array=(ioNode)malloc(sizeof(struct io_node)*CAPACITY);
        it->size=0;
        return it;
}
void print_table(ioTable it){
        for(int i=0;i<it->size;i++){
                
                printf("P%d %d %d %d\n",it->io_array[i]);
        }
}
/*get random number:1 or 0*/
int get_random(){
        srand((unsigned)time(NULL));
        return (rand()%2);
}


void schedual(){
        printf("drive schedualing\n");
}

void receive(){
        printf("receive the requset\n");
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
                	receive();
        	}
		
            printf("continue ? input y or Y if you want to continue, or any other key to stop now\n");
            hint=getchar();
            getchar();//read the return 
            printf("%c\n",hint);
            time++;
        }
        
        return 0;

}
