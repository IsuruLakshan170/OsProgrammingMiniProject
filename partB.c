#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <fcntl.h>
#include <errno.h>
#include <stdbool.h>

#include <sys/ipc.h>  // interprocess communication
#include <sys/shm.h>  // definitions for the shared memory facility
#include <sys/stat.h> //define the structure of the data returned by the function

#define listSize 20
#define fileName "studentMarks1"

int readFile();
void printStudentList();
void displayAssignment01Marks();
void maxMarks();
void minMarks();
void averageMarks();
void studentAbovePercentage();

typedef struct
{
    char student_index[20]; // EG/XXXX/XXXX
    float assgnmt01_marks;  // 15%
    float assgnmt02_marks;  // 15%
    float project_marks;    // 20%
    float finalExam_marks;  // 50%
} student_marks;

student_marks studentList[listSize];
int studentListSize;

int main()
{

    /*
        key_t ky = ftok("file.txt", 78); // use to generate a unique key
        if (ky == -1)
        {
            perror("ftok error: ");
            printf("Error No: %d\n", errno);
            exit(1);
        }

        int SMID = shmget(ky, 4096, IPC_CREAT | 0666); // create new system virutal shared memory segment
        if (SMID == -1)
        {
            perror("shmget error: ");
            printf("Error No: %d\n", errno);
            exit(1);
        }
            pid_t PID = fork();
            if (PID == -1){
                perror("fork error: ");
                printf("Error No: %d\n",errno);
                exit(0);
            }
            else if(PID == 0){
                printf("Child \n:");

            }else
            {
               // sleep(0.2);
                printf("Parent \n:");

            }
            */

    readFile();
    printStudentList();
    if (studentListSize < listSize)
    {
        printf("Student list lower than %d\n", listSize);
    }
    else
    {
        printf("Student list full \n");
    }
    // displayAssignment01Marks();
    // maxMarks();
    // minMarks();

    // averageMarks();
    studentAbovePercentage();
}

int readFile()
{
    studentListSize = 0;
    int fd;

    fd = open(fileName, O_RDONLY);
    student_marks lastStudent;
    student_marks tempStudent;
    lseek(fd, -sizeof(lastStudent), SEEK_END);
    read(fd, &lastStudent, sizeof(lastStudent));
    lseek(fd, 0, SEEK_SET);

    for (int i = 0; i < listSize; i++)
    {
        read(fd, &tempStudent, sizeof(tempStudent));

        studentList[i] = tempStudent;

        studentListSize++;
        printf("%d read from file : %s %s \n", i + 1, tempStudent.student_index, lastStudent.student_index);
        if (strcmp(tempStudent.student_index, lastStudent.student_index) == 0)
        {
            break;
        }
    }
    printf("Read file size : %d\n", studentListSize);
    close(fd);
}
void printStudentList()
{
    int j = 0;
    for (int i = 0; i < studentListSize; i++)
    {
        j++;
        printf("%d\t%s\t%f\t%f\t%f\t%f\n", i + 1, studentList[i].student_index, studentList[i].assgnmt01_marks, studentList[i].assgnmt02_marks, studentList[i].project_marks, studentList[i].finalExam_marks);

        // printf("%d : %s \n", i + 1, studentList[i].student_index);
    }
    printf("total read from array of student List  size: %d \n", j);
}

void displayAssignment01Marks()
{
    for (int i = 0; i < studentListSize; i++)
    {
        printf("%d %s %f \n", i + 1, studentList[i].student_index, studentList[i].assgnmt02_marks);
    }
}

void maxMarks()
{
    float maxMarks = 0;
    char studentNo[20];
    int index = 0;
    for (int i = 0; i < studentListSize; i++)
    {
        if (maxMarks < studentList[i].assgnmt02_marks)
        {
            index = i;
            maxMarks = studentList[i].assgnmt02_marks;
        }
        printf("%d %s %f \n", i + 1, studentList[i].student_index, studentList[i].assgnmt02_marks);
    }
    printf("%s has highest marks %f \n", studentList[index].student_index, maxMarks);
}

void minMarks()
{
    float minMarks = studentList[1].assgnmt02_marks;
    char studentNo[20];
    int index = 0;
    for (int i = 0; i < studentListSize; i++)
    {
        if (minMarks > studentList[i].assgnmt02_marks)
        {
            index = i;
            minMarks = studentList[i].assgnmt02_marks;
        }
        printf("%d %s %f \n", i + 1, studentList[i].student_index, studentList[i].assgnmt02_marks);
    }
    printf("%s has lowest marks %f \n", studentList[index].student_index, minMarks);
}

void averageMarks()
{
    float average;
    float sum = 0;
    for (int i = 0; i < studentListSize; i++)
    {
        sum = sum + studentList[i].assgnmt02_marks;
    }
    average = sum / studentListSize;
    printf("sum: %f average: %f\n", sum, average);
}

void studentAbovePercentage()
{
    float marginalMarks = 1.5;
    int studentCount = 0;
    for (int i = 0; i < studentListSize; i++)
    {  
    
        if (marginalMarks < studentList[i].assgnmt02_marks)
        {
           studentCount ++;
           printf("%s student has %f  \n", studentList[i].student_index,studentList[i].assgnmt02_marks);
        }
    }
    printf("%d student have higher than 10 percentage marks\n",studentCount);
}