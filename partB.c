#include <fcntl.h>    //Permissions
#include <errno.h>    //Stores error number
#include <stdio.h>    //Standard I/O
#include <string.h>   //handle string string copy //strcpy
#include <stdlib.h>   //provide exit
#include <unistd.h>   // and read
#include <sys/ipc.h>  // interprocess communication
#include <sys/shm.h>  // definitions for the shared memory facility
#include <sys/wait.h> //declares for used to holding processes
#include <sys/ipc.h>  // interprocess communication
#include <sys/shm.h>  // definitions for the shared memory facility
#include <sys/stat.h> //define the structure of the data returned by the function

#define listSize 100             // define student marks array size
#define fileName1 "studentMarks" // define student marks stored document name
#define fileName2 "myfile1"      // define the file name for generate unique id

// define the structure for store student marks
typedef struct
{
    char student_index[20]; // EG/XXXX/XXXX
    float assgnmt01_marks;  // 15%
    float assgnmt02_marks;  // 15%
    float project_marks;    // 20%
    float finalExam_marks;  // 50%
} student_marks;

void red();              // change colour of text as red
void yellow();           // change colour of text as yellow
void reset();            // change colour of text as default
int readFile();          // read student data from document
void printStudentList(); // print student list in console

float maxMarks(student_marks *arry, int size);             // function for find maximum marks for assignment 02
float minMarks(student_marks *arry, int size);             // function for find minimum marks for assignment 02
float averageMarks(student_marks *arry, int size);         /// function for find average marks for assignment 02
int studentAbovePercentage(student_marks *arry, int size); // function for find the number of student higher than 10% of assignment 02 marks

//--------------------main function-----------------------------
int main()
{
    int fd2;                                        // initialize integer for file discripter
    fd2 = open(fileName2, O_CREAT | O_TRUNC, 0644); // open file for create update  the file
    if (fd2 < 0)                                    // handle error file open
    {
        printf("Erro number: %d\n", errno);
        perror("file open error");
        exit(1);
    }
    close(fd2);                    // close the file descripter
    key_t ky = ftok("myfile", 78); // generate a unique key
    if (ky == -1)                  // error handle for that key
    {
        red();
        perror("\nftok error when generating key: \n");
        printf("Error No: %d\n", errno);
        reset();
        exit(1);
    }

    int SMID = shmget(ky, 4096, IPC_CREAT | 0666); // create new  virutal shared memory segment
    if (SMID == -1)                                // error handling for creaded shared memory
    {
        perror("shmget error: ");
        printf("Error No: %d\n", errno);
        exit(1);
    }
    // 1 duplication of process
    pid_t PID1 = fork(); // duplicate the bellow process
    if (PID1 == -1)      // error handle for process id
    {
        perror("fork error: ");
        printf("Error No: %d\n", errno);
        exit(0);
    }
    else if (PID1 == 0)
    { // child 1 process
        reset();
        printf("Child 1 (C1) start\n");
        yellow();
        printf("Child 1 (C1) Id : %d  Parent (P) Id : %d\n", getpid(), getppid());
        reset();
        student_marks *childPtr1;
        childPtr1 = (student_marks *)shmat(SMID, NULL, SHM_R | SHM_W); // attaches structure type pointer to the shared memory segment in identified by shared memory ID
        if (childPtr1 == (void *)-1)                                   // error handle for shared memory attachmet
        {
            perror("child 1 shmat error: ");
            printf("Error No: %d\n", errno);
            exit(1);
        }
        float *childPtr2;
        childPtr2 = (float *)shmat(SMID, NULL, SHM_R | SHM_W); // attaches float type pointer to the shared memory segment in identified by shared memory ID
        if (childPtr2 == (float *)-1)                          // error handle for shared memory attachmet
        {
            perror("parent shmat error: ");
            printf("Error No: %d\n", errno);
            exit(1);
        }
        int arraySize = (int)*(childPtr2 + 0);
        float maximumMarks = maxMarks(childPtr1, arraySize);
        int childDt1 = shmdt((void *)childPtr1); // detaches the structed pointer from the shared memory segments individually by child
        if (childDt1 == -1)                      // error handle for detaches the shared memory
        {
            perror("child shmdt error 1: ");
            printf("Error No: %d\n", errno);
            exit(0);
        }
        red();
        printf("Child 1 (C1) process: Hightest marks : %.2f\n", maximumMarks);
        reset();
        childPtr2[1] = maximumMarks;             // write to the shared memory
        int childDt2 = shmdt((void *)childPtr2); // detaches the float pointer form shared memory segments individually by child
        if (childDt2 == -1)                      // error handle for detaches the shared memory
        {
            perror("child 1 shmdt 2 error : ");
            printf("Error No: %d\n", errno);
            exit(0);
        }
        reset();
        printf("Child 1 (C1) closed\n");
    }
    else
    {
        // 2 nd duplication of process
        pid_t PID2 = fork(); // duplicate the bellow processs
        if (PID2 == -1)      // error handle for process id
        {
            perror("fork error: ");
            printf("Error No: %d\n", errno);
            exit(0);
        }
        else if (PID2 == 0)
        {
            // child 2 process
            reset();
            printf("Child 2 (C2) start\n");
            yellow();
            printf("Child 2 (C2) Id : %d  Parent Id : %d\n", getpid(), getppid());
            reset();
            student_marks *childPtr1;
            childPtr1 = (student_marks *)shmat(SMID, NULL, SHM_R | SHM_W); // attaches structure type pointer to the shared memory segment in identified by shared memory ID
            if (childPtr1 == (void *)-1)                                   // error handle for shared memory attachmet
            {
                perror("child shmat error: ");
                printf("Error No: %d\n", errno);
                exit(1);
            }
            float *childPtr2;
            childPtr2 = (float *)shmat(SMID, NULL, SHM_R | SHM_W); // attaches float type pointer to the shared memory segment in identified by shared memory ID
            if (childPtr2 == (float *)-1)                          // error handle for shared memory attachmet
            {
                perror("parent shmat error: ");
                printf("Error No: %d\n", errno);
                exit(1);
            }
            int arraySize = (int)*(childPtr2 + 0);
            float minValue = minMarks(childPtr1, arraySize);
            childPtr2[2] = minValue; //--child 2 write to shared memory
            red();
            printf("Child 2 (C2) process: Lowest marks   : %.2f\n", minValue);
            reset();
            int childDt1 = shmdt((void *)childPtr1); // detaches the structure pointer form shared memory segments individually by child
            if (childDt1 == -1)                      // error handle for detaches the shared memory
            {
                perror("child shmdt error 1: ");
                printf("Error No: %d\n", errno);
                exit(0);
            }
            int childDt2 = shmdt((void *)childPtr2); // detaches the float pointer form shared memory segments individually by child
            if (childDt2 == -1)                      // error handle for detaches the shared memory
            {
                perror("child 1 shmdt 2 error : ");
                printf("Error No: %d\n", errno);
                exit(0);
            }
            reset();
            printf("child 2 (C2) closed\n");
        }
        else
        {
            // 3 duplication of process
            pid_t PID3 = fork(); // duplicate the bellow process
            if (PID3 == -1)      // error handle for process id
            {
                perror("fork error: ");
                printf("Error No: %d\n", errno);
                exit(0);
            }
            else if (PID3 == 0)
            { // child 3 process
                reset();
                printf("Child 3 (C3) start\n");

                yellow();
                printf("Child 3 (C3) Id : %d  Parent Id : %d\n", getpid(), getppid());
                reset();
                // read------
                sleep(1);
                student_marks *childPtr1;
                childPtr1 = (student_marks *)shmat(SMID, NULL, SHM_R | SHM_W); // attaches structure type pointer to the shared memory segment in identified by shared memory ID
                if (childPtr1 == (void *)-1)                                   // error handle for shared memory attachmet
                {
                    perror("child shmat error: ");
                    printf("Error No: %d\n", errno);
                    exit(1);
                }
                float *childPtr2;
                childPtr2 = (float *)shmat(SMID, NULL, SHM_R | SHM_W); // attaches float type pointer to the shared memory segment in identified by shared memory ID
                if (childPtr2 == (float *)-1)                          // error handle for shared memory attachmet
                {
                    perror("parent shmat error: ");
                    printf("Error No: %d\n", errno);
                    exit(1);
                }
                int arraySize = (int)*(childPtr2 + 0);
                float average;                                // initialize the float varible for store average marks
                average = averageMarks(childPtr1, arraySize); // call the function of average marks
                childPtr2[3] = average;                       // write to the shared memory the average marks
                red();
                printf("Child 3 (C3) process: Average marks  : %.2f\n", average);
                reset();
                int childDt1 = shmdt((void *)childPtr1); // detaches the structure pointer form shared memory segments individually by child
                if (childDt1 == -1)                      // error handle for detaches the shared memory
                {
                    perror("child shmdt error 1: ");
                    printf("Error No: %d\n", errno);
                    exit(0);
                }
                int childDt2 = shmdt((void *)childPtr2); // attaches float type pointer to the shared memory segment in identified by shared memory ID
                if (childDt2 == -1)                      // error handle for detaches the shared memory
                {
                    perror("child 1 shmdt 2 error : ");
                    printf("Error No: %d\n", errno);
                    exit(0);
                }
                printf("Child 3 closed\n"); // child 3 process closed
            }
            else
            {
                // parent 1 process
                yellow();
                printf("-----------------------------------------\n");
                reset();

                red();
                printf("\n-------------");
                reset();
                printf("Start to analizing Assignment 2 marks");
                red();
                printf("-------------\n\n");
                reset();

                printf("Parent (P) Start\n");
                yellow();
                printf("Parent (P) Id : %d\n", getpid());
                reset();
                // read from file
                student_marks studentList[listSize];
                int studentListSize;
                studentListSize = 0;
                int fd, count;

                fd = open(fileName1, O_RDONLY); // open the file with only read only mode
                if (fd < 0)                     // handle error file open
                {
                    printf("Erro number: %d\n", errno);
                    perror("file open error");
                    exit(1);
                }
                student_marks lastStudent;
                student_marks tempStudent;
                lseek(fd, -sizeof(lastStudent), SEEK_END);
                count = read(fd, &lastStudent, sizeof(lastStudent)); // read form file descripter
                if (count < 0)                                       // error hadle for read from file
                {
                    printf("Error Number: %d\n", errno);
                    perror("Read Error: ");
                    exit(1);
                }
                lseek(fd, 0, SEEK_SET); // nevigate to specific place of the file discripter

                for (int i = 0; i < listSize; i++)
                {
                    count = read(fd, &tempStudent, sizeof(tempStudent)); // read form file descripter
                    if (count < 0)                                       // error hadle for read from file
                    {
                        printf("Error Number: %d\n", errno);
                        perror("Read Error: ");
                        break;
                        exit(1);
                    }

                    studentList[i] = tempStudent;

                    studentListSize++;
                    // printf("%d read from file : %s %s \n", i + 1, tempStudent.student_index, lastStudent.student_index);
                    if (strcmp(tempStudent.student_index, lastStudent.student_index) == 0)
                    {
                        break;
                    }
                }

                close(fd); // close the file descripter

                student_marks *parentPtr1;
                parentPtr1 = (student_marks *)shmat(SMID, NULL, SHM_R | SHM_W); // attaches structure type pointer to the shared memory segment in identified by shared memory ID
                if (parentPtr1 == (student_marks *)-1)                          // error handle for shared memory attachment
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
                parentPtr2 = (float *)shmat(SMID, NULL, SHM_R | SHM_W); // attaches float type pointer to the shared memory segment in identified by shared memory ID
                if (parentPtr2 == (void *)-1)                           // error handle for shared memory attachment
                {
                    perror("child shmat error: ");
                    printf("Error No: %d\n", errno);
                    exit(1);
                }

                parentPtr2[0] = studentListSize;

                // printf("parent write to shared memory finished \n");

                waitpid(PID1, NULL, 0); // wait untill child 1 terminate
                waitpid(PID2, NULL, 0); // wait untill child 2 terminate
                waitpid(PID3, NULL, 0); // wait untill child 3 terminate
                printf("\nAll 3 childrens are closed \n\n");
                // parent read--------------------------------

                // 10%above number of students
                int arraySize = (int)*(parentPtr2 + 0);
                int studentCount = studentAbovePercentage(parentPtr1, arraySize); // call function for count number of student have higher than 10% marks

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

                printf("   Hightest marks\t\t\t\t%.2f\n", *(parentPtr2 + 1));
                printf("   Lowest marks\t\t\t\t\t%.2f\n", *(parentPtr2 + 2));
                printf("   Average marks\t\t\t\t%.2f\n", *(parentPtr2 + 3));
                printf("   Number of student higher than 10 percent\t%d\n", studentCount);
                yellow();
                printf("\n--------------------------------------------------------------\n");
                reset();

                int parntDt1 = shmdt((void *)parentPtr1); // detaches the structure pointer form shared memory segments individually by child
                if (parntDt1 == -1)                       // error handle for detaches the shared memory
                {
                    perror("parent shmdt error: ");
                    printf("Error No: %d\n", errno);
                    exit(0);
                }
                int parntDt2 = shmdt((void *)parentPtr2); // detaches the float pointer form shared memory segments individually by child
                if (parntDt2 == -1)                       // error handle for detaches the shared memory
                {
                    perror("parent shmdt error: ");
                    printf("Error No: %d\n", errno);
                    exit(0);
                }
                printf("parent closed\n");
                int parntCtl = shmctl(SMID, IPC_RMID, NULL); // control the shared memory identified by SHMID ---- IPC_RMID --destroyed shared memory segment
                if (parntCtl == -1)                          // error handle for destroy created shared memory
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
//----------------------------find maximum marks from assignment 2 --------------------
float maxMarks(student_marks *arry, int size)
{
    float maxMarks = 0; // initialize the float for store maximum marks
    int index = 0;
    for (int i = 0; i < size; i++)
    {
        if (maxMarks < arry[i].assgnmt02_marks)
        {
            index = i;
            maxMarks = arry[i].assgnmt02_marks;
        }
    }
    return maxMarks;
}
//----------------------------find minimum marks from assignment 2 --------------------
float minMarks(student_marks *arry, int size)
{
    float minMarks = arry[0].assgnmt02_marks; // Initialize array[0].assgnmt02_marks;
    int index = 0;
    for (int i = 0; i < size; i++)
    {
        if (minMarks > arry[i].assgnmt02_marks)
        {
            index = i;
            minMarks = arry[i].assgnmt02_marks;
        }
    }
    return minMarks;
}
//----------------------------find average marks for assignment 2 --------------------
float averageMarks(student_marks *arry, int size)
{

    float average; // initialize a float varible for store average marks
    float sum = 0; // initialize a float varible for sum of assignment 2 marks
    for (int i = 0; i < size; i++)
    {
        sum = sum + arry[i].assgnmt02_marks;
    }
    average = sum / size;
    return average;
}

//-----------------find the number of students having higher than 10% of marks from 15 marks in form assignment 1 -------------------
int studentAbovePercentage(student_marks *arry, int size)
{
    float marginalMarks = 1.5; // initialize a float varible for store marginalMarks
    int studentCount = 0;      // initialize a int varible for store count no of students
    for (int i = 0; i < size; i++)
    {
        if (marginalMarks < arry[i].assgnmt02_marks)
        {
            studentCount++;
        }
    }
    return studentCount;
}

//-----------------------change the text colour as red-----------------------------
void red()
{
    printf("\033[1;31m");
}
//--------------------change the text colour as yellow-----------------------------
void yellow()
{
    printf("\033[1;33m");
}
//---------------------reset the text colur as default----------------------------
void reset()
{
    printf("\033[0m");
}