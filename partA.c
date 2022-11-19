#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <fcntl.h>
#include <errno.h>
#include <stdbool.h>
#include <ctype.h>

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

student_marks studentList[listSize];
int studentListSize;

int codeNo;

int studentIndex = 0;
char newStudentRegNo[20]; // = "EG/2018/3366";
void yellow();
void red();
void reset();
int readFile();
void greetings();
void selectOperation(int codeNo);
void insert();
void update();
void delete ();
bool isExistingStudent();
bool isNewStudent();
void addStudent();
void updateStudent();
void deleteStudent();
void writeFile();
void removeStudentFromArray();
int getStudentListSize();
void printStudentList();
void backToMenu();
void addAllStudentMarks();
bool isRegNoCorrect(char *name);

int main()
{
    readFile();
    greetings();
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
    // printf("Read file size : %d\n", studentListSize);
    close(fd);
}

void greetings()
{

    char heading[] = "Select following operations";
    char title[] = "code\tDescription\n";
    char ope1[] = "001\tInsert New Student";
    char ope2[] = "002\tUpdate Student";
    char ope3[] = "003\tDelete Student";
    char ope4[] = "004\tAdd Student All Student Marks";
    char ope5[] = "005\tShow Student Marks";
   
    while (1)
    {   codeNo =001;
        red();
        printf("--------------------------------------------\n");
        reset();
        printf("%s\n",heading);
        yellow();
        printf("%s\n",title);
        reset();
        
        printf("%s\n%s\n%s\n%s\n%s\n",ope1, ope2, ope3, ope4, ope5);
        red();
        printf("--------------------------------------------\n");
        reset();
        printf("Enter code : ");
        scanf("%d", &codeNo);
       
        if (codeNo == 001 || codeNo == 002 || codeNo == 003 || codeNo == 004 || codeNo == 005)
        {
            selectOperation(codeNo);
            break;
        }
        else
        {   red();
            printf("You Entered Code is Incorrect! Try Gain\n");
           reset();
        }
        sleep(1);
    }
}

void selectOperation(int index)
{
    switch (index)
    {
    case 1:
    {
        insert();
        backToMenu();
        break;
    }
    case 2:
    {
        update();
        backToMenu();
        break;
    }
    case 3:
    {
        delete ();
        backToMenu();
        break;
    }
    case 4:
    {
        // delete ();
        addAllStudentMarks();
        backToMenu();
        break;
    }
    case 5:
    {
        printStudentList();
        backToMenu();
        break;
    }

    default:
    {   red();
        printf("Invalid error Code\n");
        reset();
        greetings();
        break;
    }
    }
}

void insert()
{

    readFile();
    if (studentListSize == listSize)
    {   red();
        printf("Student List is full\n");
        reset();
    }
    else
    {
        addStudent();
    }
}
void update()
{
    updateStudent();
}
void delete ()
{
    deleteStudent();
}

void addStudent()
{
    student_marks newStudent;
    while (1)
    {
        printf("Enter student regNo (EG/xxxx/xxxx) : ");
        scanf("%s", newStudent.student_index);

        bool isCheck = isRegNoCorrect(newStudent.student_index);
        if (isCheck)
        {
            break;
        }
        else
        { red();
            printf("Entered Student Registraion Number in correct Format!.Try again\n");
             reset();
        }
    }

    strcpy(newStudentRegNo, newStudent.student_index);
    // printf("Entered New Student : %s\n", newStudentRegNo);
    bool isNewStudent = isExistingStudent();
    if (!isNewStudent)
    {
        printf("New Student\n");

        while (1)
        {
            printf("Enter student assgnmt01_marks \n");
            scanf("%f", &newStudent.assgnmt01_marks);
            if (newStudent.assgnmt01_marks > 15)
            {red();
                printf("Assgnmt01 marks can't exceed higher than 15 \n");
                reset();
            }
            else
            {
                break;
            }
        }

        while (1)
        {
            printf("Enter student assgnmt02 marks \n");
            scanf("%f", &newStudent.assgnmt02_marks);
            if (newStudent.assgnmt02_marks > 15)
            {red();
                printf("Assgnmt02 marks can't exceed higher than 15 \n");
                reset();
            }
            else
            {
                break;
            }
        }

        while (1)
        {

            printf("Enter student project marks \n");
            scanf("%f", &newStudent.project_marks);
            if (newStudent.project_marks > 20)
            {red();
                printf("project marks  can't exceed higher than 20 \n");
                reset();
            }
            else
            {
                break;
            }
        }
        while (1)
        {

            printf("Enter student finalExam marks  \n");
            scanf("%f", &newStudent.finalExam_marks);
            if (newStudent.finalExam_marks > 50)
            {red();
                printf("finalExam  marks  can't exceed higher than 250\n");
                reset();
            }
            else
            {
                break;
            }
        }

        studentList[studentListSize] = newStudent;
        yellow();
        printf("Added new New Student\n");
        reset();
        studentListSize++;
        printStudentList();
        writeFile();
        readFile();
        printStudentList();
    }
    else
    {red();
        printf("Existing student\n");
        reset();
    }
}

void updateStudent()
{
    student_marks newStudent;

    while (1)
    {
        printf("Enter student regNo (EG/xxxx/xxxx) : ");
        scanf("%s", newStudent.student_index);

        bool isCheck = isRegNoCorrect(newStudent.student_index);
        if (isCheck)
        {
            break;
        }
        else
        {red();
            printf("Entered Student Registraion Number in correct Format!.Try again\n");
            reset();
        }
    }
    strcpy(newStudentRegNo, newStudent.student_index);

    //  printf("Entered the Student : %s\n", newStudent.student_index);
    bool isNewStudent = isExistingStudent();
    if (!isNewStudent)
    {red();
        printf("This Student not in list\n"); // can not do process
        reset();
    }
    else
    {
        printf("This Student in the list you can update this student\n"); // can do process

        while (1)
        {
            printf("Enter student assgnmt01_marks : ");
            scanf("%f", &newStudent.assgnmt01_marks);
            if (newStudent.assgnmt01_marks > 15)
            {red();
                printf("Assgnmt01 marks can't exceed higher than 15 \n");
                 reset () ;
            }
            else
            {
                break;
            }
        }

        while (1)
        {
            printf("Enter student assgnmt02 marks : ");
            scanf("%f", &newStudent.assgnmt02_marks);
            if (newStudent.assgnmt02_marks > 15)
            {red();
                printf("Assgnmt02 marks can't exceed higher than 15 \n");
                reset();
            }
            else
            {
                break;
            }
        }

        while (1)
        {

            printf("Enter student project marks : ");
            scanf("%f", &newStudent.project_marks);
            if (newStudent.project_marks > 20)
            {red();
                printf("project marks  can't exceed higher than 20 \n");
                reset();
            }
            else
            {
                break;
            }
        }
        while (1)
        {

            printf("Enter student finalExam marks  : ");
            scanf("%f", &newStudent.finalExam_marks);
            if (newStudent.finalExam_marks > 50)
            {red();
                printf("finalExam  marks  can't exceed higher than 250\n");
                reset();
            }
            else
            {
                break;
            }
        }
        studentList[studentIndex] = newStudent;
        yellow();
        printf("Successully updated Existing Student\n");
reset();
        writeFile();
        readFile();
    }
}
void deleteStudent()
{
    student_marks newStudent;

    while (1)
    {
        printf("Enter student regNo (EG/xxxx/xxxx) : ");
        scanf("%s", newStudent.student_index);

        bool isCheck = isRegNoCorrect(newStudent.student_index);
        if (isCheck)
        {
            break;
        }
        else
        {red();
            printf("Entered Student Registraion Number in correct Format!.Try again\n");
            reset();
        }
    }

  //  printf("Entered Student No: %s\n", newStudentRegNo);
    bool isNewStudent = isExistingStudent();
    if (!isNewStudent)
    {red();
        printf("This Student not in list can not delete\n");
        reset();
    }
    else
    {
       
        // printf("Existing student %s delete successfully \n", newStudentRegNo);

        printf("index : %d\n", studentIndex);
        removeStudentFromArray();
        yellow();
        printf("The Student  delete successfully\n");
        reset();
        writeFile();
        readFile();
    }
}
void printStudentList()
{
    int j = 0;
    yellow();
    printf("No\tStudent RegNo\tAssignment 1 marks\tAssignment 2 marks\tProject Marks\tFinal Exam Marks\n");
    reset();
    for (int i = 0; i < studentListSize; i++)
    {
        j++;
        printf("%d\t%s\t%f\t\t%f\t\t%f\t%f\n", i + 1, studentList[i].student_index, studentList[i].assgnmt01_marks, studentList[i].assgnmt02_marks, studentList[i].project_marks, studentList[i].finalExam_marks);

        // printf("%d : %s \n", i + 1, studentList[i].student_index);
    }
    printf("total read from array of student List  size: %d \n", j);
}
bool isExistingStudent()
{
    studentIndex = 0;
    // printf("Size student list %d\n", studentListSize);
    for (int i = 0; i < studentListSize; i++)
    {

        // printf("%d : %s %s\n", i + 1, studentList[i].student_index, newStudentRegNo);
        if (!strcmp(studentList[i].student_index, newStudentRegNo))
        {
            //

            return true;
        }
        studentIndex++;
    }
    return false;
}

void writeFile()
{
    int fd;
    fd = open(fileName, O_RDWR | O_CREAT | O_TRUNC, 0644);

    for (int j = 0; j < studentListSize; j++)
    {
        student_marks tempStudent = *(studentList + j);
        int errorWrite = write(fd, &tempStudent, sizeof(tempStudent));
        if (errorWrite == -1)
        {
            perror("Wrting error : ");
            printf("Error No: %d ", errno);
            exit(1);
        }
        // printf("%d Write again Studen : %s\n", j + 1, (tempStudent.student_index));
    }
    close(fd);
}

void removeStudentFromArray()
{   yellow();
    printf("Remove student index %d: \n", studentIndex);
    reset();
    studentList[studentIndex] = studentList[studentListSize - 1];
    studentListSize--;

    //  listSize =listSize+1;
}

void backToMenu()
{
    char userInput[5];
    printf("Do you want to Navigate to menu again? (Y or N) : ");
    scanf("%s", userInput);
    if (!strcmp(userInput, "Y") || !strcmp(userInput, "y"))
    {
        greetings();
    }
    else if (!strcmp(userInput, "N") || !strcmp(userInput, "n"))
    {yellow();
        printf("programe closed!\n");
        reset();
    }
    else
    {   red();
        printf("Please enter valid command !\n");
        reset();
        backToMenu();
    }
}

void addAllStudentMarks()
{
    char userInput[5];
    red();
    printf("All the previous data will remove before adding new data!.Are you sure want to add all the data? (Y or N) : ");
    reset();
    scanf("%s", userInput);
    if (!strcmp(userInput, "Y") || !strcmp(userInput, "y"))
    {

        studentListSize = 0;

        student_marks newStudent;
        int i = 0;
        while (i < listSize)
        {

            while (1)
            {
                printf("Enter %d student regNo (EG/xxxx/xxxx) : ", i + 1);
                scanf("%s", newStudent.student_index);

                bool isCheck = isRegNoCorrect(newStudent.student_index);
                if (isCheck)
                {
                    break;
                }
                else
                {   red();
                    printf("Entered Student Registraion Number in correct Format!.Try again\n");
                    reset();
                }
            }

            strcpy(newStudentRegNo, newStudent.student_index);
            bool isNewStudent = isExistingStudent();
            if (!isNewStudent)
            {

                while (1)
                {
                    printf("Enter student assgnmt01_marks : ");
                    scanf("%f", &newStudent.assgnmt01_marks);
                    if (newStudent.assgnmt01_marks > 15)
                    {   red();
                        printf("Assgnmt01 marks can't exceed higher than 15 \n");
                        reset();
                    }
                    else
                    {
                        break;
                    }
                }

                while (1)
                {
                    printf("Enter student assgnmt02 marks : ");
                    scanf("%f", &newStudent.assgnmt02_marks);
                    if (newStudent.assgnmt02_marks > 15)
                    {red();
                        printf("Assgnmt02 marks can't exceed higher than 15 \n");
                        reset();
                    }
                    else
                    {
                        break;
                    }
                }

                while (1)
                {

                    printf("Enter student project marks : ");
                    scanf("%f", &newStudent.project_marks);
                    if (newStudent.project_marks > 20)
                    {red();
                        printf("project marks  can't exceed higher than 20 \n");
                        reset();
                    }
                    else
                    {
                        break;
                    }
                }
                while (1)
                {

                    printf("Enter student finalExam marks : ");
                    scanf("%f", &newStudent.finalExam_marks);
                    if (newStudent.finalExam_marks > 50)
                    {red();
                        printf("finalExam  marks  can't exceed higher than 250\n");
                        reset();
                    }
                    else
                    {
                        break;
                    }
                }

                studentList[i] = newStudent;
                i++;
                studentListSize++;
            }
            else
            {
                printf("Existing student\n");
            }
        }

        writeFile();
        readFile();
        printf("Added all Students\n");
    }
    else if (!strcmp(userInput, "N") || !strcmp(userInput, "n"))
    {
        backToMenu();
    }
    else
    {red();
        printf("Please enter valid command !\n");
        reset();
        addAllStudentMarks();
    }
}

bool isRegNoCorrect(char *name)
{
    bool isFormatTrue = true;
    char val2;
    for (int i = 0; name[i] != '\0'; i++)
    {
        val2 = name[i];
        if (isdigit(val2))
        {
        }
        else
        {
            if (i == 0 && val2 == 'E')
            {
            }
            else if (i == 1 && val2 == 'G')
            {
            }
            else if ((i == 2 || i == 7) && (val2 == '/'))
            {
            }
            else
            {
                isFormatTrue = false;
                break;
            }
        }
    }
    int i;
    for (i = 0; name[i] != '\0'; ++i)
        ;
    if (i != 12)
    {
        isFormatTrue = false;
    }

    return isFormatTrue;
}

void red () {
  printf("\033[1;31m");
}

void yellow (){
  printf("\033[1;33m");
}

void reset () {
  printf("\033[0m");
}

