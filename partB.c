#include <fcntl.h>   //Permissions
#include <errno.h>   //Stores error number
#include <stdio.h>   //Standard I/O
#include <string.h>  //handle string string copy //strcpy
#include <stdlib.h>  //provide exit
#include <unistd.h>  // and read
#include <sys/ipc.h> // interprocess communication
#include <sys/shm.h> // definitions for the shared memory facility
#include <sys/wait.h>
#include <sys/ipc.h>  // interprocess communication
#include <sys/shm.h>  // definitions for the shared memory facility
#include <sys/stat.h> //define the structure of the data returned by the function

#define listSize 100
#define fileName "studentMarks1"

typedef struct
{
    char student_index[20]; // EG/XXXX/XXXX
    float assgnmt01_marks;  // 15%
    float assgnmt02_marks;  // 15%
    float project_marks;    // 20%
    float finalExam_marks;  // 50%
} student_marks;

int readFile();
void printStudentList();
void displayAssignment01Marks();
int sizeOfArray(student_marks *arry);
float maxMarks(student_marks *arry, int size);
float minMarks(student_marks *arry, int size);
float averageMarks(student_marks *arry, int size);
int studentAbovePercentage(student_marks *arry, int size);



int main()
{
   
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
    pid_t PID1 = fork();
    //  pid_t PID2 = fork();
    if (PID1 == -1)
    {
        perror("fork error: ");
        printf("Error No: %d\n", errno);
        exit(0);
    }
    else if (PID1 == 0)
    {                 // child 1
        sleep(0.001); // suspend the process

        printf("Child 1 Id : %d  Parent Id : %d\n", getpid(), getppid());

        student_marks *childPtr1;
        childPtr1 = (student_marks *)shmat(SMID, NULL, SHM_R | SHM_W); // create a new shared memory segment or to locate an existing one based on a key
        if (childPtr1 == (void *)-1)
        {
            perror("child shmat error: ");
            printf("Error No: %d\n", errno);
            exit(1);
        }
        float *parentPtr2;
        parentPtr2 = (float *)shmat(SMID, NULL, SHM_R | SHM_W); // create a new shared memory segment or to locate an existing one based on a key
        if (parentPtr2 == (float *)-1)
        {
            perror("parent shmat error: ");
            printf("Error No: %d\n", errno);
            exit(1);
        }
        int arraySize = (int)*(parentPtr2 + 0);
        //  printf("---------------->size:%d\n",arraySize);
        float maximumMarks = maxMarks(childPtr1, arraySize);

        //   printf("--->-------higtest marks: %f\n",maxMarks);
        int childDt1 = shmdt((void *)childPtr1); // detaches the shared memory segments individually
        if (childDt1 == -1)
        {
            perror("child shmdt error 1: ");
            printf("Error No: %d\n", errno);
            exit(0);
        }
        // child 1 write ------------------

        parentPtr2[1] = maximumMarks;

        int childDt = shmdt((void *)parentPtr2); // detaches the shared memory segments individually
        if (childDt == -1)
        {
            perror("child 1 shmdt 2 error : ");
            printf("Error No: %d\n", errno);
            exit(0);
        }
        printf("Child 1 closed\n");
    }
    else
    {

        pid_t PID2 = fork();
        if (PID2 == -1)
        {
            perror("fork error: ");
            printf("Error No: %d\n", errno);
            exit(0);
        }
        else if (PID2 == 0)
        {
            // child 2
            // read -----
            printf("Child 2 Id : %d  Parent Id : %d\n", getpid(), getppid());

            student_marks *childPtr2;
            childPtr2 = (student_marks *)shmat(SMID, NULL, SHM_R | SHM_W); // create a new shared memory segment or to locate an existing one based on a key
            if (childPtr2 == (void *)-1)
            {
                perror("child shmat error: ");
                printf("Error No: %d\n", errno);
                exit(1);
            }
            float *parentPtr3;
            parentPtr3 = (float *)shmat(SMID, NULL, SHM_R | SHM_W); // create a new shared memory segment or to locate an existing one based on a key
            if (parentPtr3 == (float *)-1)
            {
                perror("parent shmat error: ");
                printf("Error No: %d\n", errno);
                exit(1);
            }
            int arraySize = (int)*(parentPtr3 + 0);

            float minValue = minMarks(childPtr2, arraySize);

            //  printf("lowest marks %f \n", minValue);

            //--child 2 write

            parentPtr3[2] = minValue;
            printf("child 2 closed\n");
        }
        else
        {

            pid_t PID3 = fork();
            if (PID3 == -1)
            {
                perror("fork error: ");
                printf("Error No: %d\n", errno);
                exit(0);
            }
            else if (PID3 == 0)
            { // child 3
                printf("Child 3 Id : %d  Parent Id : %d\n", getpid(), getppid());
                // read------
                student_marks *childPtr3;
                childPtr3 = (student_marks *)shmat(SMID, NULL, SHM_R | SHM_W); // create a new shared memory segment or to locate an existing one based on a key
                if (childPtr3 == (void *)-1)
                {
                    perror("child shmat error: ");
                    printf("Error No: %d\n", errno);
                    exit(1);
                }
                float *parentPtr2;
                parentPtr2 = (float *)shmat(SMID, NULL, SHM_R | SHM_W); // create a new shared memory segment or to locate an existing one based on a key
                if (parentPtr2 == (float *)-1)
                {
                    perror("parent shmat error: ");
                    printf("Error No: %d\n", errno);
                    exit(1);
                }
                int arraySize = (int)*(parentPtr2 + 0);
                float average;
                average = averageMarks(childPtr3, arraySize);

                // write----

                parentPtr2[3] = average;
                printf("Child 3 closed\n");
            }
            else
            {
                // parent process
                printf("Parent id : %d\n", getpid());

                // read from file
                student_marks studentList[listSize];
                int studentListSize;
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
                    // printf("%d read from file : %s %s \n", i + 1, tempStudent.student_index, lastStudent.student_index);
                    if (strcmp(tempStudent.student_index, lastStudent.student_index) == 0)
                    {
                        break;
                    }
                }
                printf("Read file size : %d\n", studentListSize);
                close(fd);

                
                student_marks *parentPtr;
                parentPtr = (student_marks *)shmat(SMID, NULL, SHM_R | SHM_W); // create a new shared memory segment or to locate an existing one based on a key
                if (parentPtr == (student_marks *)-1)
                {
                    perror("parent shmat error: ");
                    printf("Error No: %d\n", errno);
                    exit(1);
                }
                for (int i = 0; i < studentListSize; i++)
                {
                    parentPtr[i] = studentList[i];
                }

                float *childPtrResults;
                childPtrResults = (float *)shmat(SMID, NULL, SHM_R | SHM_W); // create a new shared memory segment or to locate an existing one based on a key
                if (childPtrResults == (void *)-1)
                {
                    perror("child shmat error: ");
                    printf("Error No: %d\n", errno);
                    exit(1);
                }

                childPtrResults[0] = studentListSize;

                printf("parent write to shared memory finished \n");

                waitpid(PID1, NULL, 0); // wait untill child 1 terminate
                waitpid(PID2, NULL, 0); // wait untill child 2 terminate
                waitpid(PID2, NULL, 0); // wait untill child 3 terminate
                printf("all child are closed  \n");
                // parent read--------------------------------
                printf("parent read \n");

                // 10%above number of students
                int arraySize = (int)*(childPtrResults + 0);
                int studentCount = studentAbovePercentage(parentPtr, arraySize);

                printf("From child 1 hightest marks %f\n", *(childPtrResults + 1));
                printf("From child 2 lowest marks %f\n", *(childPtrResults + 2));
                printf("From child 3 average  marks %f\n", *(childPtrResults + 3));
                printf("Parent process number of student higher than 10 Percent : %d\n", studentCount);
                //   printf("Size of student list:------%d\n", (int)*(childPtrResults + 0));
                printf("parent process finished \n");

                int parntDt = shmdt((void *)parentPtr); // detaches the shared memory segments individually
                if (parntDt == -1)
                {
                    perror("parent shmdt error: ");
                    printf("Error No: %d\n", errno);
                    exit(0);
                }
                int parntCtl = shmctl(SMID, IPC_RMID, NULL); // control the shared memory identified by SHMID ---- IPC_RMID --destroyed shared memory segment
                if (parntCtl == -1)
                {
                    perror("parent shmctl error: ");
                    printf("Error No: %d\n", errno);
                    exit(0);
                }
            }
        }
    }
}
/*
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
        // printf("%d read from file : %s %s \n", i + 1, tempStudent.student_index, lastStudent.student_index);
        if (strcmp(tempStudent.student_index, lastStudent.student_index) == 0)
        {
            break;
        }
    }
    printf("Read file size : %d\n", studentListSize);
    close(fd);
}*/
/*
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
*/
/*
void displayAssignment01Marks()
{
    for (int i = 0; i < studentListSize; i++)
    {
        printf("%d %s %f \n", i + 1, studentList[i].student_index, studentList[i].assgnmt02_marks);
    }
}
*/
/*
int sizeOfArray(student_marks *arry)
{
    printf("----------------------- test array ---------------\n");
    for (int i = 0; i < 103; i++)
    {
        printf("%d  %f \n", i + 1, (arry[i].assgnmt02_marks));
    }
}
*/
float maxMarks(student_marks *arry, int size)
{

    float maxMarks = 0;
    char studentNo[20];
    int index = 0;
    for (int i = 0; i < size; i++)
    {
        if (maxMarks < arry[i].assgnmt02_marks)
        {
            index = i;
            maxMarks = arry[i].assgnmt02_marks;
        }
        //  printf("%d %s %f \n", i + 1, arry[i].student_index, arry[i].assgnmt02_marks);
    }
    //  printf("%s has highest marks %f \n", arry[index].student_index, maxMarks);

    return maxMarks;
}

float minMarks(student_marks *arry, int size)
{

    float minMarks = arry[0].assgnmt02_marks; //= array[0].assgnmt02_marks;
    char studentNo[20];
    int index = 0;
    for (int i = 0; i < size; i++)
    {
        if (minMarks > arry[i].assgnmt02_marks)
        {
            index = i;
            minMarks = arry[i].assgnmt02_marks;
        }
        // printf("%d %s %f \n", i + 1, studentList[i].student_index, studentList[i].assgnmt02_marks);
    }
    //  printf("%s has lowest marks %f \n", studentList[index].student_index, minMarks);
    return minMarks;
}

float averageMarks(student_marks *arry, int size)
{

    float average;
    float sum = 0;
    for (int i = 0; i < size; i++)
    {
        sum = sum + arry[i].assgnmt02_marks;
    }
    average = sum / size;
    return average;
}

int studentAbovePercentage(student_marks *arry, int size)
{
    float marginalMarks = 1.5;
    int studentCount = 0;
    for (int i = 0; i < size; i++)
    {

        if (marginalMarks < arry[i].assgnmt02_marks)
        {
            studentCount++;
            // printf("%s student has %f  \n", studentList[i].student_index, studentList[i].assgnmt02_marks);
        }
    }
    // printf("%d student have higher than 10 percentage marks\n", studentCount);
    return studentCount;
}