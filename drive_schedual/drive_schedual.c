#include<stdio.h>
#include<stdlib.h>
#include<time.h>
int main(int argc,char **argv);
/*get random number:1 or 0*/
int get_random(){
        srand((unsigned)time(NULL));
        return (rand()%2);
}
int main(int argc,char **argv){
        int ran=get_random();
        printf("%d",ran);
        return 0;

}
