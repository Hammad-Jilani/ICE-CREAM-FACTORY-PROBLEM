#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<pthread.h>

#define BUFFER_SIZE 5
int main(){
    int buffer[BUFFER_SIZE];
    for(int i =0;i<BUFFER_SIZE;i++){
        printf("%d\n",buffer[i]);
    }
    return 0;
}