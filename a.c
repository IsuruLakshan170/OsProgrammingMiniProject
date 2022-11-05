#include <stdio.h>    
     
int main()    
{    
    //Initialize array     

    int arr[20];
    int size = sizeof(arr)/sizeof(arr[0]);
    for(int i = 0;i<20;i++)
    {
        arr[i]=0;
    }
     for(int i = 0;i<10;i++)
    {
        arr[i]=i+1;

    }
    int count =0;
    for(int i = 0;i<20;i++)
    { if(arr[i] == 0){
        break;
         }
         count ++;
    }
    printf("count : %d\n",count);
    return 0;    
}    