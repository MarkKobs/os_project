/* Project Name:use Fifo to deal with missing page inter
 * Date:2017.12.1
 * Author:Mark Kobs
 * 内容：模拟分页式虚拟存储管理中的地址转化和缺页中断，
 * 以及选择页面调度算法处理缺页中断,这里主要是实现FIFO调度算法处理缺页中断。
 * 页表的格式为 |页号|标志位|主存块号|在磁盘上的位置|(页号与主存块号之间一一映射)
 * 其中：标志位1表示对应页已经装入内存，0表示该页未装入内存
 * 主存块号：用来标志装入内存的页所占的块号
 * 在磁盘上的位置：表示页在磁盘上的位置
 * 
 * 在实现FIFO的时候务必会用到队列知识，这里用了循环队列的处理方法
 * 但是在循环队列的基础上，需要用到遍历数组的算法，算法复杂度为O(n)
 * */

#include<stdlib.h>
#include<stdio.h>
#define OPERATION 12
#define PAGES 7
#define INIT_SIZE 4
typedef struct page *Page;
typedef struct page_table *PageTable;
typedef struct instruction_sequence *InstructionSequence;
typedef struct queue *Queue;
int main(int argc,char **argv);
void print_page(Page page);
int isEmpty(Queue q);
struct page{
        int page_number;
        int flag;//flag=1 in MM, flag=0 not in MM
        int block_number;
        int change;//表示是否修改过
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
struct queue{
        int capacity;
        int size;
        int k;
        int rear;
        int *array;
};
Queue createQueue(int length){
      Queue q=malloc(sizeof(struct queue));
      q->capacity=length;
      q->array=malloc(length*sizeof(int));
      q->size=0;
      q->k=0;
      q->rear=-1;
      for(int i=0;i<4;i++){
              q->array[i]=i;
              q->size++;
              q->rear++;
      }
      return q;
}
void print_queue(Queue q){
        printf("queue capacity:%d,size:%d,k:%d,rear:%d\n",q->capacity,q->size,q->k,q->rear);
        if(isEmpty(q)){
                printf("the queue is empty!\n");
        }
        else{
                printf("the queue here:");
                if(q->k<=q->rear){
                        for(int i=q->k;i<=q->rear;i++){
                                printf("%d ",q->array[i]);
                        }
                }
                else{//k>rear
                        for(int i=q->k;i<q->size;i++){
                                printf("%d ",q->array[q->size+q->k-q->capacity]);
                        } 
                       printf("\n"); 
                }
        }
}
static int succ(int value,Queue q){//进位
        if(++value==q->capacity){
                value=0;
        }
        return value;
}
int isEmpty(Queue q){
        return q->size==0;
}
int isFull(Queue q){
        return q->size==q->capacity; 
}
int pageIsInArray(int page_number,int *array,int array_length){
        for(int i=0;i<array_length;i++){
                if(page_number==array[i]){
                        return 1;
                }
        }
        return 0;
}
void Enqueue(int x,Queue q){//队列中存放的实际上是页表，x就是要插入的页表
        if(isFull(q)){//满了，就要调出，再将指令中的页号x插入调出的位置
                printf("%d -----> %d\n",q->array[q->k],x);//原先是array
                q->k=x;
                q->k++;
                
        }
        else{
                q->size++;
                q->rear=succ(q->rear,q);
                q->array[q->rear]=x;
        }
}
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
void init_page(Page p,int p_n,int f,int b_n,int c,int p_i_d){
        p->page_number=p_n;
        p->flag=f;
        p->block_number=b_n;
        p->change=c;
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
        printf("初始化:\n");
        printf("page|flag|block|change|disk position\n ");//默认修改位都为0
        init_page(pt->page_list,0,1,5,0,11);
        init_page(pt->page_list+1,1,1,8,0,12);
        init_page(pt->page_list+2,2,1,9,0,13);
        init_page(pt->page_list+3,3,1,1,0,21);
        init_page(pt->page_list+4,4,0,0,0,22);
        init_page(pt->page_list+5,5,0,0,0,23);
        init_page(pt->page_list+6,6,0,0,0,121);
        return pt;
}
/*打印函数*/
void print_instruction(InstructionSequence is){
        printf("%c %d %d\n",is->operation_type,is->page_number,is->unit_number);
}
void print_page(Page onePage){
        printf("%d %d %d %d %d\n",onePage->page_number,onePage->flag,onePage->block_number,onePage->change,onePage->position_in_disk);
}
/*page函数*/
int isInMemory(PageTable pt,int page_number){//page number from 0 to 6 
        Page page=pt->page_list+page_number;
        return page->flag;
}
/*形成绝对地址*/
int position_transformation(PageTable pt,InstructionSequence is){
        int pg=is->page_number;
        int un=is->unit_number;
        int bn=pt->page_list[pg].block_number;
        /* absolute path = bn x 128 + un*/
        int absolute=bn*128+un;

        return absolute;
}
int isFullPT(PageTable pt){
        int full=(pt->size==pt->capacity)?1:0;
        return full;
}
int isStore(InstructionSequence is){
        int store=(is->operation_type=='w')?1:0;
        return store;
}
void setFlagToOne(PageTable pt,int page_number){
        pt->page_list[page_number].flag=1;
        
}
void process_interruption(){//处理中断程序
//TODO
} 
int main(int argc,char **argv){
        InstructionSequence seq=init_instruction();//an array actually
        PageTable pt=init_page_table();
        Queue queue=createQueue(7);
        print_queue(queue);
        /*实现模拟FIFO执行流程*/
        for(int i=0;i<OPERATION;i++){
                int page_number=((InstructionSequence)&seq[i])->page_number;
                printf("read instruction %d:%c\n",i+1,seq[i].operation_type);//read an instruction
                if(isInMemory(pt,page_number)){
                        /*形成绝对地址*/
                        int absolute=position_transformation(pt,(InstructionSequence)&seq[i]);
                        printf("%d\n",absolute);
                        if(isStore((InstructionSequence)&seq[i])){//写操作
                                /*置该页修改标志为1*/
                                pt->page_list[page_number].change=1;
                                printf("%d changed!\n",page_number);
                        }

                }
                else{
                        /*缺页中断处理*/
                        process_interruption();                   
                        /*输出缺页号*/
                        printf("* %d\n",page_number);
                }
        }
        return 0;
}
