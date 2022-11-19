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

typedef struct
{
    char student_index[20]; // EG/XXXX/XXXX
    float assgnmt01_marks;  // 15%
    float assgnmt02_marks;  // 15%
    float project_marks;    // 20%
    float finalExam_marks;  // 50%
} student_marks;

student_marks studentList[100];
int codeNo;
char newStudentRegNo[20];
int readList();
void greetings();
void selectOperation(int codeNo);
void insert();
void update();
void delete ();
bool isExistingStudent();
void addStudent();
bool isRegNoCorrect();
bool fistLetter(char *name);

bool isRegNoCorrect(char *name);
int main()
{
    /*
    char str1[50];  // declaration of char array
   char str2[50]="EG/2018/3365";  // declaration of char array
   int value; // declaration of integer variable

   printf("Enter the first string : ");
   scanf("%s",str1);


   // comparing both the strings using strcmp() function

   value = strcmp(str1,str2);
   if(value == 0)
   printf("strings are same");
   else
   printf("strings are not same");


   */

    int i;
    char name[] = "EG/2018/33665";
    // for (i = 0; name[i] != '\0'; ++i);
    // printf("Length of the string: %d\n", i);
    bool isCheck = isRegNoCorrect(name);
    if (isCheck)
    {
        printf("True");
    }
    else
    {
        printf("False");
    }

    return 0;
}

bool fistLetter(char *firstThree)
{
    // firstThree ="EG/";
    int value = strcmp(firstThree, "EG/");
    printf("%s\n", firstThree);
    if (value == 0)
    {
        printf("strings are same\n");
        return true;
    }
    else
    {
        printf("strings are not same\n");
        return false;
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

int readList()
{

    int fd;
    int arrySize;
    fd = open(fileName, O_RDONLY);
    student_marks lastStudent;
    student_marks tempStudent;
    lseek(fd, 0, SEEK_END);
    read(fd, &lastStudent, sizeof(lastStudent));
    lseek(fd, 0, SEEK_SET);

    for (int i = 0; i < listSize; i++)
    {
        read(fd, &tempStudent, sizeof(tempStudent));
        arrySize++;
        studentList[i] = tempStudent;
        printf("%s \n", tempStudent.student_index);
        if (strcmp(tempStudent.student_index, lastStudent.student_index) == 0)
        {
            break;
        }
    }
    close(fd);
    return arrySize;
}

void greetings()
{

    char heading[] = "Select following operations";
    char title[] = "code\tDescription\n";
    char ope1[] = "001\tInsert Student";
    char ope2[] = "002\tUpdate Student";
    char ope3[] = "003\tDelete Student";
    printf("%s\n%s\n%s\n%s\n%s\n", heading, title, ope1, ope2, ope3);
    printf("Enter code : ");
    scanf("%d", &codeNo);
    selectOperation(codeNo);
}

void selectOperation(int index)
{
    switch (index)
    {
    case 1:
    {
        insert();
        break;
    }
    case 2:
    {
        update();
        break;
    }
    case 3:
    {
        delete ();
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

void insert()
{
    printf("Insert\n");
    addStudent();
}
void update()
{
    printf("update\n");
}
void delete ()
{
    printf("delete\n");
}

void addStudent()
{
    printf("Enter student regNo (EG/xxxx/xxxx)\n");
    printf("Reg No : ");
    scanf("%s", newStudentRegNo);
    printf("Entered New Student : %s\n", newStudentRegNo);
    bool isNewStudent = isExistingStudent();
    if (isNewStudent)
    {
        printf("New Student\n");
    }
    else
    {
        printf("Existing student\n");
    }
}

bool isExistingStudent()
{
    printf("Size %d\n", listSize);
    for (int i = 0; i < listSize; i++)
    {
        if (!strcmp(studentList[i].student_index, newStudentRegNo))
        {

            return true;
        }
    }
    return false;
}
