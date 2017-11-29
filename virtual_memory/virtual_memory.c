/* Project Name:Virtual Memory
 * Date:2017.11.29
 * Author:Mark Kobs
 * 内容：模拟分页式虚拟存储管理中的地址转化和缺页中断，
 * 以及选择页面调度算法处理缺页中断
 * 页表的格式为 |页号|标志位|主存块号|在磁盘上的位置|(页号与主存块号之间一一映射)
 * 其中：标志位1表示对应页已经装入内存，0表示该页未装入内存
 * 主存块号：用来标志装入内存的页所占的块号
 * 在磁盘上的位置：表示页在磁盘上的位置
 *
 * */

#include<stdlib.h>
#include<stdio.h>
typedef page_table *pageTable;
struct page_table{
        int page_number;
        int flag;//flag=1 in MM, flag=0 not in MM
        int block_number;
        int position_in_disk;
}
