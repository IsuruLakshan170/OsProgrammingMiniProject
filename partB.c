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
void maxMarks(student_marks arry);
void minMarks();
void averageMarks();
void studentAbovePercentage();

student_marks studentList[listSize];
int studentListSize;

int main()
{
    readFile();
    printStudentList();
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
    { // child 1

        printf("Child 1 Id : %d  Parent Id : %d\n", getpid(), getppid());

        sleep(0.001); // suspend the process
        student_marks *childPtr1;
        childPtr1 = (student_marks *)shmat(SMID, NULL, SHM_R | SHM_W); // create a new shared memory segment or to locate an existing one based on a key
        if (childPtr1 == (void *)-1)
        {
            perror("child shmat error: ");
            printf("Error No: %d\n", errno);
            exit(1);
        }

        // find maximum -------------------------

        float maxMarks = 0;
        char studentNo[20];
        int index = 0;
        for (int i = 0; i < studentListSize; i++)
        {
            if (maxMarks < childPtr1[i].assgnmt02_marks)
            {
                index = i;
                maxMarks = childPtr1[i].assgnmt02_marks;
            }
            //  printf("%d %s %f \n", i + 1, childPtr1[i].student_index, childPtr1[i].assgnmt02_marks);
        }
        printf("%s has highest marks %f \n", childPtr1[index].student_index, maxMarks);
        //-----------------------

        int childDt1 = shmdt((void *)childPtr1); // detaches the shared memory segments individually
        if (childDt1 == -1)
        {
            perror("child shmdt error 1: ");
            printf("Error No: %d\n", errno);
            exit(0);
        }
        // child write ------------------

        float *parentPtr2;
        parentPtr2 = (float *)shmat(SMID, NULL, SHM_R | SHM_W); // create a new shared memory segment or to locate an existing one based on a key
        if (parentPtr2 == (float *)-1)
        {
            perror("parent shmat error: ");
            printf("Error No: %d\n", errno);
            exit(1);
        }

        parentPtr2[0] = maxMarks;

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
            float minMarks = childPtr2[1].assgnmt02_marks;
            char studentNo[20];
            int index = 0;
            for (int i = 0; i < studentListSize; i++)
            {
                if (minMarks > childPtr2[i].assgnmt02_marks)
                {
                    index = i;
                    minMarks = childPtr2[i].assgnmt02_marks;
                }
                //   printf("%d %s %f \n", i + 1, childPtr2[i].student_index, childPtr2[i].assgnmt02_marks);
            }
            printf("%s has lowest marks %f \n", studentList[index].student_index, minMarks);
            printf("child 2 closed\n");

            //--write
            float *parentPtr2;
            parentPtr2 = (float *)shmat(SMID, NULL, SHM_R | SHM_W); // create a new shared memory segment or to locate an existing one based on a key
            if (parentPtr2 == (float *)-1)
            {
                perror("parent shmat error: ");
                printf("Error No: %d\n", errno);
                exit(1);
            }

            parentPtr2[1] = minMarks;
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

                float average;
                float sum = 0;
                for (int i = 0; i < studentListSize; i++)
                {
                    sum = sum + childPtr3[i].assgnmt02_marks;
                }
                average = sum / studentListSize;
                printf("sum: %f average: %f\n", sum, average);

                // write----
                float *parentPtr2;
                parentPtr2 = (float *)shmat(SMID, NULL, SHM_R | SHM_W); // create a new shared memory segment or to locate an existing one based on a key
                if (parentPtr2 == (float *)-1)
                {
                    perror("parent shmat error: ");
                    printf("Error No: %d\n", errno);
                    exit(1);
                }

                parentPtr2[2] = average;
                printf("Child 3 closed\n");
            }
            else
            {
                // parent process
                printf("Parent id : %d\n", getpid());
                // int marks[10] = {3, 8, 1, 3, 2, 7, 9, 6, 4, 6};
                //   int marks =10;
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

                printf("parent write to shared memory finished \n");

                waitpid(PID1, NULL, 0); // wait untill child terminate
                waitpid(PID2, NULL, 0); // wait untill child terminate
                waitpid(PID2, NULL, 0); // wait untill child terminate
                printf("all child are closed  \n");
                // parent read--------------------------------
                printf("parent read \n");

                float *childPtrResults;
                childPtrResults = (float *)shmat(SMID, NULL, SHM_R | SHM_W); // create a new shared memory segment or to locate an existing one based on a key
                if (childPtrResults == (void *)-1)
                {
                    perror("child shmat error: ");
                    printf("Error No: %d\n", errno);
                    exit(1);
                }
                //10%above number of students
                float marginalMarks = 6;
                int studentCount = 0;
                for (int i = 0; i < studentListSize; i++)
                {

                    if (marginalMarks < studentList[i].assgnmt02_marks)
                    {
                        studentCount++;
                       
                    }
                }
                printf("From child 1 hightest marks %f\n", *childPtrResults);
                printf("From child 2 lowest marks %f\n", *(childPtrResults + 1));
                printf("From child 3 average  marks %f\n", *(childPtrResults + 2));
                printf("Parent process number of student higher than 10 Percent : %d\n", studentCount);

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

                // displayAssignment01Marks();
                // maxMarks();
                //  minMarks();

                //  averageMarks();
                // studentAbovePercentage();
            }
        }
    }

    /*
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
       // studentAbovePercentage();

       */
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
        // printf("%d read from file : %s %s \n", i + 1, tempStudent.student_index, lastStudent.student_index);
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
        // printf("%d\t%s\t%f\t%f\t%f\t%f\n", i + 1, studentList[i].student_index, studentList[i].assgnmt01_marks, studentList[i].assgnmt02_marks, studentList[i].project_marks, studentList[i].finalExam_marks);

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

void maxMarks(student_marks arry)
{

    // student_marks studentMarks[listSize] = arry;
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
            studentCount++;
            printf("%s student has %f  \n", studentList[i].student_index, studentList[i].assgnmt02_marks);
        }
    }
    printf("%d student have higher than 10 percentage marks\n", studentCount);
}