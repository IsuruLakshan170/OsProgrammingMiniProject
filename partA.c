#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <fcntl.h>
#include <errno.h>
#include <stdbool.h> 


#define listSize 100
#define fileName "studentMarks1"

typedef struct {
char student_index[20]; //EG/XXXX/XXXX
float assgnmt01_marks; //15%
float assgnmt02_marks; //15%
float project_marks; //20%
float finalExam_marks; //50%
}student_marks;

student_marks studentList[100];
int codeNo;
char newStudentRegNo[20];
int readList();   
void greetings();
void selectOperation(int codeNo);
void insert();
void update();
void delete();
bool isExistingStudent();
void addStudent();
void updateStudent();
void deleteStudent();

int main()
{
    int arraySize = readList();
    if(arraySize < 100){
        printf("Student list lower than 100\n");

    }
    else{
     printf("Student list FUll\n");
     greetings();
    }
}
int readList(){

    int fd;
    int arrySize;
    fd = open(fileName,O_RDONLY);
    student_marks lastStudent;
    student_marks tempStudent;
    lseek(fd,0,SEEK_END);
    read(fd,&lastStudent, sizeof(lastStudent));
    lseek(fd,0,SEEK_SET);
 
    for (int i=0 ;i<listSize;i++){
        read(fd,&tempStudent, sizeof(tempStudent));
        arrySize ++;
        studentList[i]=tempStudent;
        printf("%s \n",tempStudent.student_index);
        if(strcmp(tempStudent.student_index , lastStudent.student_index) == 0){
            break;
        }
    }
    close(fd);
    return arrySize;
}     

void greetings(){
   
    char heading[]="Select following operations";
    char title[]="code\tDescription\n";
    char ope1[] ="001\tInsert Student";
    char ope2[] ="002\tUpdate Student";
    char ope3[] ="003\tDelete Student";
    printf("%s\n%s\n%s\n%s\n%s\n",heading,title,ope1,ope2,ope3);
    printf("Enter code : ");
    scanf("%d",&codeNo);
   selectOperation(codeNo);

}

void selectOperation(int index)
{
    switch (index)
    {
    case 1:
       { insert();
        break;
        }
     case 2:
      {  update();
        break;
        }
         case 3:
        {delete();
        break;
        }
    default:
        {
            printf("Invalid error Code\n");
            greetings();
            break;
        }
        
    }
}

void insert(){
    printf("Insert\n");
    int studentListSize = readList();
    if(studentListSize == 100){
        printf("Student List is full\n");
    }
    else
    {
        addStudent();
    }
 
}
void update(){
    printf("update\n");
    updateStudent();
}
void delete(){
    printf("delete\n");
    deleteStudent();
}


void addStudent(){
    printf("Enter student regNo (EG/xxxx/xxxx)\n");
    printf("Reg No : ");
    scanf("%s",newStudentRegNo);
    printf("Entered New Student : %s\n",newStudentRegNo);
   bool isNewStudent = isExistingStudent();
   if(isNewStudent)
   {
    printf("New Student\n");
       }
   else
   {
       printf("Existing student\n");
      }

}

void updateStudent(){
    printf("Enter student regNo (EG/xxxx/xxxx)\n");
    printf("Reg No : ");
    scanf("%s",newStudentRegNo);
    printf("Entered the Student : %s\n",newStudentRegNo);
   bool isNewStudent = isExistingStudent();
   if(isNewStudent)
   {
    printf("This Student not in list\n");//can not do process
    }
   else
   {
       printf("Existing student\n");//can do process
      }

}
void deleteStudent(){
    printf("Enter student regNo (EG/xxxx/xxxx)\n");
    printf("Reg No : ");
    scanf("%s",newStudentRegNo);
    printf("Entered New Student : %s\n",newStudentRegNo);
   bool isNewStudent = isExistingStudent();
   if(isNewStudent)
   {
    printf("This Student not in list can not delete\n");
       }
   else
   {
       printf("Existing student\n");
      }

}
bool isExistingStudent(){
    printf("Size %d\n",listSize);
    for(int i =0;i<listSize;i++)
    { 
        if(!strcmp(studentList[i].student_index,newStudentRegNo))
        {

           return true;
        }
   }
  return false;
}
