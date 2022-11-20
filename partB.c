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
#define fileName1 "studentMarks"
#define fileName2 "myfile"
typedef struct
{
    char student_index[20]; // EG/XXXX/XXXX
    float assgnmt01_marks;  // 15%
    float assgnmt02_marks;  // 15%
    float project_marks;    // 20%
    float finalExam_marks;  // 50%
} student_marks;

void red();
void yellow();
void reset();
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
    int fd2;
    fd2 = open(fileName2,  O_CREAT | O_TRUNC, 0644);
    close(fd2);
    key_t ky = ftok("myfile", 78); // use to generate a unique key
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
    if (PID1 == -1)
    {
        perror("fork error: ");
        printf("Error No: %d\n", errno);
        exit(0);
    }
    else if (PID1 == 0)
    {   // child 1 process
        sleep(0.001); // suspend the process
        reset();
        printf("Child 1 start\n");
        yellow();
        printf("Child 1 Id : %d  Parent Id : %d\n", getpid(), getppid());
        reset();
        student_marks *childPtr1;
        childPtr1 = (student_marks *)shmat(SMID, NULL, SHM_R | SHM_W); // create a new shared memory segment or to locate an existing one based on a key
        if (childPtr1 == (void *)-1)
        {
            perror("child shmat error: ");
            printf("Error No: %d\n", errno);
            exit(1);
        }
        float *childPtr2;
        childPtr2 = (float *)shmat(SMID, NULL, SHM_R | SHM_W); // create a new shared memory segment or to locate an existing one based on a key
        if (childPtr2 == (float *)-1)
        {
            perror("parent shmat error: ");
            printf("Error No: %d\n", errno);
            exit(1);
        }
        int arraySize = (int)*(childPtr2 + 0);
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

        childPtr2[1] = maximumMarks;

        int childDt2 = shmdt((void *)childPtr2); // detaches the shared memory segments individually
        if (childDt2 == -1)
        {
            perror("child 1 shmdt 2 error : ");
            printf("Error No: %d\n", errno);
            exit(0);
        }
        reset();
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
            // child 2 process
            // read -----
            reset();
            printf("Child 2 start\n");
            yellow();
            printf("Child 2 Id : %d  Parent Id : %d\n", getpid(), getppid());
            reset();
            student_marks *childPtr1;
            childPtr1 = (student_marks *)shmat(SMID, NULL, SHM_R | SHM_W); // create a new shared memory segment or to locate an existing one based on a key
            if (childPtr1 == (void *)-1)
            {
                perror("child shmat error: ");
                printf("Error No: %d\n", errno);
                exit(1);
            }
            float *childPtr2;
            childPtr2 = (float *)shmat(SMID, NULL, SHM_R | SHM_W); // create a new shared memory segment or to locate an existing one based on a key
            if (childPtr2 == (float *)-1)
            {
                perror("parent shmat error: ");
                printf("Error No: %d\n", errno);
                exit(1);
            }
            int arraySize = (int)*(childPtr2 + 0);

            float minValue = minMarks(childPtr1, arraySize);

            //  printf("lowest marks %f \n", minValue);

            //--child 2 write

            childPtr2[2] = minValue;
           
            int childDt1 = shmdt((void *)childPtr1); // detaches the shared memory segments individually
            if (childDt1 == -1)
            {
                perror("child shmdt error 1: ");
                printf("Error No: %d\n", errno);
                exit(0);
            }
            int childDt2 = shmdt((void *)childPtr2); // detaches the shared memory segments individually
            if (childDt2 == -1)
            {
                perror("child 1 shmdt 2 error : ");
                printf("Error No: %d\n", errno);
                exit(0);
            }
            reset();
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
            { // child 3 process
            reset();
            printf("Child 3 start\n");
            yellow();
                printf("Child 3 Id : %d  Parent Id : %d\n", getpid(), getppid());
                reset();
                // read------
                student_marks *childPtr1;
                childPtr1 = (student_marks *)shmat(SMID, NULL, SHM_R | SHM_W); // create a new shared memory segment or to locate an existing one based on a key
                if (childPtr1 == (void *)-1)
                {
                    perror("child shmat error: ");
                    printf("Error No: %d\n", errno);
                    exit(1);
                }
                float *childPtr2;
                childPtr2 = (float *)shmat(SMID, NULL, SHM_R | SHM_W); // create a new shared memory segment or to locate an existing one based on a key
                if (childPtr2 == (float *)-1)
                {
                    perror("parent shmat error: ");
                    printf("Error No: %d\n", errno);
                    exit(1);
                }
                int arraySize = (int)*(childPtr2 + 0);
                float average;
                average = averageMarks(childPtr1, arraySize);

                // write----

                childPtr2[3] = average;
                printf("Child 3 closed\n");

                int childDt1 = shmdt((void *)childPtr1); // detaches the shared memory segments individually
                if (childDt1 == -1)
                {
                    perror("child shmdt error 1: ");
                    printf("Error No: %d\n", errno);
                    exit(0);
                }
                int childDt2 = shmdt((void *)childPtr2); // detaches the shared memory segments individually
                if (childDt2 == -1)
                {
                    perror("child 1 shmdt 2 error : ");
                    printf("Error No: %d\n", errno);
                    exit(0);
                }
            }
            else
            {
                // parent 1 process
                yellow();
                printf("-----------------------------------------\n");
                reset();
                printf("Parent Start\n");
                yellow();
                printf("Parent Id : %d\n", getpid());
                reset();
                // read from file
                student_marks studentList[listSize];
                int studentListSize;
                studentListSize = 0;
                int fd;

                fd = open(fileName1, O_RDONLY);
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

                close(fd);

                student_marks *parentPtr1;
                parentPtr1 = (student_marks *)shmat(SMID, NULL, SHM_R | SHM_W); // create a new shared memory segment or to locate an existing one based on a key
                if (parentPtr1 == (student_marks *)-1)
                {
                    perror("parent shmat error: ");
                    printf("Error No: %d\n", errno);
                    exit(1);
                }
                for (int i = 0; i < studentListSize; i++)
                {
                    parentPtr1[i] = studentList[i];
                }

                float *parentPtr2;
                parentPtr2 = (float *)shmat(SMID, NULL, SHM_R | SHM_W); // create a new shared memory segment or to locate an existing one based on a key
                if (parentPtr2 == (void *)-1)
                {
                    perror("child shmat error: ");
                    printf("Error No: %d\n", errno);
                    exit(1);
                }

                parentPtr2[0] = studentListSize;

               // printf("parent write to shared memory finished \n");

                waitpid(PID1, NULL, 0); // wait untill child 1 terminate
                waitpid(PID2, NULL, 0); // wait untill child 2 terminate
                waitpid(PID2, NULL, 0); // wait untill child 3 terminate
                printf("\nAll 3 childrens are closed \n\n");
                // parent read--------------------------------
               // printf("parent read \n");

                // 10%above number of students
                int arraySize = (int)*(parentPtr2 + 0);
                int studentCount = studentAbovePercentage(parentPtr1, arraySize);
                yellow();
                printf("-----------------------------------------\n");
                reset();
                red();
                printf("\n-------------");
                reset();
                printf("Process Results");
                red();
                printf("-------------\n\n");
                reset();

                printf("From child 1 process: Hightest marks : %f\n", *(parentPtr2 + 1));
                printf("From child 2 process: Lowest marks   : %f\n", *(parentPtr2 + 2));
                printf("From child 3 process: Average marks  : %f\n", *(parentPtr2 + 3));
                printf("From parent 1 process: Number of student higher than 10 Percent : %d\n", studentCount);
                //   printf("Size of student list:------%d\n", (int)*(childPtrResults + 0));
                yellow();
                printf("-----------------------------------------\n");
                reset();
                red();
                printf("\n-------------");
                reset();
                printf("100 Students Analized data");
                red();
                printf("-----------------------\n\n");
                reset();

                yellow();
                printf("\tTopic\t\t\t\t\tResults\n\n");
                reset();

                printf("   Hightest marks\t\t\t\t%f\n", *(parentPtr2 + 1));
                printf("   Lowest marks\t\t\t\t\t%f\n", *(parentPtr2 + 2));
                printf("   Average marks\t\t\t\t%f\n", *(parentPtr2 + 3));
                printf("   Number of student higher than 10 percent\t%d\n", studentCount);
                yellow();
                printf("\n--------------------------------------------------------------\n");
                reset();


                int parntDt1 = shmdt((void *)parentPtr1); // detaches the shared memory segments individually
                if (parntDt1 == -1)
                {
                    perror("parent shmdt error: ");
                    printf("Error No: %d\n", errno);
                    exit(0);
                }
                 int parntDt2 = shmdt((void *)parentPtr2); // detaches the shared memory segments individually
                if (parntDt2 == -1)
                {
                    perror("parent shmdt error: ");
                    printf("Error No: %d\n", errno);
                    exit(0);
                }
                printf("parent closed\n");
                int parntCtl = shmctl(SMID, IPC_RMID, NULL); // control the shared memory identified by SHMID ---- IPC_RMID --destroyed shared memory segment
                if (parntCtl == -1)
                {
                    perror("parent shmctl error: ");
                    printf("Error No: %d\n", errno);
                    exit(0);
                }
                yellow();
                 printf("Process End\n");
                 reset();
            }
        }
    }
}
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

void red()
{
    printf("\033[1;31m");
}

void yellow()
{
    printf("\033[1;33m");
}

void reset()
{
    printf("\033[0m");
}