/* Project Name:use Fifo to deal with missing page inter
 * Date:2017.11.29
 * Author:Mark Kobs
 * content:the program is the simulation of virtual storage management based on pagination,
 * and choose one proper type of page schedualing program to deal with the page interruption.
 * Here, we choose algorithm FIFO.
 * the format of the page  |page number|flag|Main memory block number|position in disk|
 * flag:1 means the page is in the MM(main memory),0 - not in.
 * block number：the place to find the page on MM.
 * the postion in disk: the place to find the page in disk.
 * 
 * When we choose the Algorithm FIFO,we have to construct it with circle queue , there are two 
 * solutions, one is array ,the other is linklist, here is using the array.
 * 
 * * */
#include<stdlib.h>
#include<stdio.h>
#define OPERATION 12
#define PAGES 7
#define INIT_SIZE 4
typedef struct page *Page;
typedef struct page_table *PageTable;
typedef struct instruction_sequence *InstructionSequence;
int main(int argc,char **argv);
void print_page(Page page);
struct page{
        int page_number;
        int flag;//flag=1 in MM, flag=0 not in MM
        int block_number;
        int position_in_disk;

};
struct page_table{
        int capacity;
        int size;//the size:the number of page in main memory.
        /*meanwhile,subindex: size-1*/
        Page page_list;
};
struct instruction_sequence{
        char operation_type;
        /*operation type includes : + * r w - m */
        int page_number;
        int unit_number;
};
void setInstruction(InstructionSequence is,char o_t,int p_n,int u_n){
        is->operation_type=o_t;
        is->page_number=p_n;
        is->unit_number=u_n;
}
InstructionSequence init_instruction(){
        InstructionSequence instructionSequence=(InstructionSequence)malloc(OPERATION*sizeof(struct instruction_sequence));
        if(instructionSequence==NULL){
                printf("out of space!\n");
        }
        /*initialize 12 instruction sequence*/
        setInstruction(instructionSequence,'+',0,70);
        setInstruction(instructionSequence+1,'+',1,50);
        setInstruction(instructionSequence+2,'x',2,15);
        setInstruction(instructionSequence+3,'w',3,21);
        setInstruction(instructionSequence+4,'r',0,56);
        setInstruction(instructionSequence+5,'-',6,40);
        setInstruction(instructionSequence+6,'m',4,53);
        setInstruction(instructionSequence+7,'+',5,23);
        setInstruction(instructionSequence+8,'w',1,37);
        setInstruction(instructionSequence+9,'r',2,78);
        setInstruction(instructionSequence+10,'+',4,1);
        setInstruction(instructionSequence+11,'w',6,84);
        return instructionSequence;

}
void init_page(Page p,int p_n,int f,int b_n,int p_i_d){
        p->page_number=p_n;
        p->flag=f;
        p->block_number=b_n;
        p->position_in_disk=p_i_d;
        print_page(p);
}
PageTable init_page_table(){
        PageTable pt=(PageTable)malloc(sizeof(struct page_table));
        if(pt==NULL){
                printf("out of space! \n");
        }
        pt->capacity=PAGES;
        pt->size=INIT_SIZE;
        pt->page_list=(Page)malloc(pt->capacity*sizeof(struct page));//default page list has 7 pages ,array
        if(pt->page_list==NULL){
                printf("out of space!\n");
        }
        printf("the page initialized:\n");
        printf("page|flag|block|disk position\n ");
        init_page(pt->page_list,0,1,5,11);
        init_page(pt->page_list+1,1,1,8,12);
        init_page(pt->page_list+2,2,1,9,13);
        init_page(pt->page_list+3,3,1,1,21);
        init_page(pt->page_list+4,4,0,0,22);
        init_page(pt->page_list+5,5,0,0,23);
        init_page(pt->page_list+6,6,0,0,121);
        return pt;
}
void print_instruction(InstructionSequence is){
        printf("%c %d %d\n",is->operation_type,is->page_number,is->unit_number);
}
void print_page(Page onePage){
        printf("%d %d %d %d\n",onePage->page_number,onePage->flag,onePage->block_number,onePage->position_in_disk);
}
int isInMemory(PageTable pt,int page_number){//page number from 0 to 6 
        Page page=pt->page_list+page_number;
        return page->flag;
}
int position_transformation(PageTable pt,InstructionSequence is){
        int pg=is->page_number;
        int un=is->unit_number;
        int bn=pt->page_list[pg].block_number;
        /* absolute path = bn x 128 + un*/
        int absolute=bn*128+un;

        return absolute;
}

int main(int argc,char **argv){
        InstructionSequence seq=init_instruction();//an array actually
        PageTable pt=init_page_table();
        /*simulate*/
        for(int i=0;i<OPERATION;i++){
                int page_number=((InstructionSequence)&seq[i])->page_number;
                printf("read instruction %d:%c\n",i+1,seq[i].operation_type);
                if(isInMemory(pt,page_number)){
                        /*absolute path*/
                        int absolute=position_transformation(pt,(InstructionSequence)&seq[i]);
                        printf("%d\n",absolute);

                }
                else{
                        /*output the interruption*/
                        printf("* %d\n",page_number);
                }
        }
        return 0;
}
