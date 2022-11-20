
#include <time.h>    //get and manipulate date and time information
#include <string.h>  // defines one variable type, one macro, and various functions for manipulating arrays of characters
#include <math.h>    //defines various mathematical functions and one macro
#include <stdbool.h> //must be included to use bool in C
#include <ctype.h>   //contains inbuilt functions to handle characters in C
#include <fcntl.h>   //Permissions
#include <unistd.h>  //Low level I/O
#include <errno.h>   //Stores error number
#include <stdio.h>   //Standard I/O
#include <stdlib.h>  //High level I/O, exit

#define listSize 100            // define the student list array size
#define fileName "studentMarks" // define the student marks stored file name

// define the structure for store student marks
typedef struct
{
    char student_index[20]; // EG/XXXX/XXXX
    float assgnmt01_marks;  // 15%
    float assgnmt02_marks;  // 15%
    float project_marks;    // 20%
    float finalExam_marks;  // 50%
} student_marks;

student_marks studentList[listSize]; // define the student marks array

// initialize integer varibles
int studentListSize = 0;     // define integer varible for get student list array size
int regNumberList[listSize]; // define array for store student registration number
int length;                  // define integer varible for store student reg.No array size
int studentIndex = 0;        // define integer varible for get current student index number

// initialize character varibles
char newStudentRegNo[20]; // create string varible for store new student reg.No

// initialize the void functions
void yellow();                  // change colour of text as yellow
void red();                     // change colour of text as red
void reset();                   // change colour of text as default
void greetings();               // function for display menu
void selectOperation(int code); // switch case for navigate to entered menu
void insert();                  // function for insert operation
void addStudent();              // function for add one student to array
void updateStudent();           // function for update one student from array
void deleteStudent();           // function for delete one student from array
void writeFile();               // student data write to the document
void removeStudentFromArray();  // remove student from array
void printStudentList();        // print student list in console
void backToMenu();              // navigate to menu pannel
void addAllStudentMarks();      // function for add all students mannually
void randomStudentGenerate();   // function for random data generate operation
void generateRandomData();      // generate random data

// initialize the float functions
float numGenarator(int lower, int upper); // random number generate

// initialize the bool functions
bool isRegNoCorrect(char *name); // funtion for check registration number format
bool isExistingStudent();        // function for check new student is existing in current document

// initialize the int functions
int readFile(); // read student data from document

// initialize the structure type functions
student_marks randomStudent(); // generate one student Reg.No with marks randomly

//--------------------main function-----------------------------
int main()
{
    readFile();  // read data from document
    greetings(); // display menu
}

//---------------read student marks from the document and save to array-------
int readFile()
{
    studentListSize = 0;
    int count; // define for integer for save error number of success number
    int fd;
    fd = open(fileName, O_RDONLY); // open file as read only mode
    if (fd < 0)                    // error handle for file open
    {
        printf("Error number: %d\n", errno);
        perror("dataFile: ");
        exit(1);
    }
    student_marks lastStudent;
    student_marks tempStudent;
    lseek(fd, -sizeof(lastStudent), SEEK_END); // set file pointer to end of the file
    count = read(fd, &lastStudent, sizeof(lastStudent));
    if (count < 0) // handle read error
    {
        printf("Error Number: %d\n", errno);
        perror("Read Error: ");
        exit(1);
    }
    lseek(fd, 0, SEEK_SET); // set file pointer to start of the file

    for (int i = 0; i < listSize; i++)
    {
        count = read(fd, &tempStudent, sizeof(tempStudent)); // read from document
        if (count < 0)                                       // handle read error
        {
            printf("Error Number: %d\n", errno);
            perror("Read Error: ");
            exit(1);
        }

        studentList[i] = tempStudent; // assign temp student to student list array

        studentListSize++;
        // printf("%d read from file : %s %s \n", i + 1, tempStudent.student_index, lastStudent.student_index);
        if (strcmp(tempStudent.student_index, lastStudent.student_index) == 0)
        {
            break;
        }
    }
    // printf("Read file size : %d\n", studentListSize);
    close(fd); // close the file descripter
}

//----------------------------display menu pannel ---------------------------
void greetings()
{
    char heading[] = "Select following operations";
    char title[] = "code\tDescription\n";
    char ope1[] = "001\tInsert New Student";
    char ope2[] = "002\tUpdate Student";
    char ope3[] = "003\tDelete Student";
    char ope4[] = "004\tAdd Student All Student Marks";
    char ope5[] = "005\tShow Student Marks";
    char ope6[] = "006\tAuto generate 100 Random students with random Marks";
    char ope7[] = "007\tClose the Progrmme";
    int codeNo;
    //  int code;
    while (1)
    {

        red(); // convert bellow text colour as red
        printf("--------------------------------------------\n");
        reset(); // convert bellow text colour as default colour
        printf("%s\n\n", heading);
        yellow(); // convert bellow text colour as yellow
        printf("%s\n", title);
        reset();

        printf("%s\n%s\n%s\n%s\n%s\n%s\n%s\n", ope1, ope2, ope3, ope4, ope5, ope6, ope7);
        red();
        printf("--------------------------------------------\n");
        reset();
        printf("Enter code : ");

        int retValue = scanf("%d", &codeNo);//check user input is in correct data type
        if (retValue > 0)//error handle
        {
            selectOperation(codeNo); // calling the function for navigate to menu items
            break;//stop the loop of the function
        }
        else
        {
            red();
            printf("\nYou Entered Code is Incorrect!\n\n");
            reset();
            exit(1);//exit the current process
        }
    }
}

//-----------------switch function for navigate to menu items-----------------
void selectOperation(int index)
{
    switch (index) // switch operation
    {
    case 1: // insert operation
    {
        insert();
        backToMenu();
        break;
    }
    case 2: // update operation
    {
        updateStudent();
        backToMenu();
        break;
    }
    case 3: // delete operation
    {
        deleteStudent();
        backToMenu();
        break;
    }
    case 4: // add all students marks mannullay operation
    {
        addAllStudentMarks();
        backToMenu();
        break;
    }
    case 5: // print to console all students marks
    {
        printStudentList();
        backToMenu();
        break;
    }
    case 6: // randomly generate all students marks
    {
        randomStudentGenerate();
        backToMenu();
        break;
    }
    case 7: // closed the programme
    {
        yellow();
        printf("\nSuccessfully closed the Programme\n\n");
        reset();
        break;
    }
    default: // error handle for invalid code
    {
        red();
        printf("Invalid error Code\n");
        reset();
        greetings();
        break;
    }
    }
}

//-------------------------new student insert to the list operation-----------------
void insert()
{

    readFile();                      // read from document
    if (studentListSize == listSize) // check student list is full or not
    {
        red();
        printf("Student List is full\n");
        reset();
    }
    else
    {
        addStudent();
    }
}

//----------------------add new student to the student marks array---------------------
void addStudent()
{
    student_marks newStudent;
    int returnVal;
    while (1)
    {
        printf("Enter student regNo (EG/xxxx/xxxx) : ");
        returnVal = scanf("%s", newStudent.student_index); // error handle for user input
        if (returnVal < 1)                                 // if user input is not in correct data type
        {
            red();
            printf("User input is not in correct format!\n");
            exit(1);
        }

        bool isCheck = isRegNoCorrect(newStudent.student_index); // check registraion number is in correct format
        if (isCheck)                                             // if student is in list close the i
        {
            break; // close the loop
        }
        else
        {
            red(); // change text color as red
            printf("\nEntered Student Registraion Number in correct Format!.Try again\n");
            reset(); // change text color as default
        }
    }

    strcpy(newStudentRegNo, newStudent.student_index); // string copy

    bool isNewStudent = isExistingStudent(); // check new student is existing student or not
    if (!isNewStudent)
    {
        // printf("New Student\n");
        int errNum;
        while (1)
        {
            printf("Enter student assignment 01 marks :");
            errNum = scanf("%f", &newStudent.assgnmt01_marks); // check scanf output success or not
            if (errNum <= 0)                                   // error handle for input integer marks
            {
                red();
                printf("\nSomething worng\n\n");
                exit(1);
            }

            if (newStudent.assgnmt01_marks > 15)
            {
                red();
                printf("Assignment 01 marks can't exceed higher than 15 \n");
                reset();
            }
            else
            {
                break;
            }
        }

        while (1)
        {
            printf("Enter student assignment 02 marks :");
            errNum = scanf("%f", &newStudent.assgnmt02_marks); // check scanf output success or not
            if (errNum <= 0)                                   // error handle for input integer marks
            {
                red();
                printf("\nSomething worng\n\n");
                exit(1);
            }
            if (newStudent.assgnmt02_marks > 15)
            {
                red();
                printf("Assignment 02 marks can't exceed higher than 15 \n");
                reset();
            }
            else
            {
                break;
            }
        }

        while (1)
        {

            printf("Enter student project marks :");
            errNum = scanf("%f", &newStudent.project_marks); // check scanf output success or not
            if (errNum <= 0)                                 // error handle for input integer marks
            {
                red();
                printf("\nSomething worng\n\n");
                exit(1);
            }
            if (newStudent.project_marks > 20)
            {
                red();
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

            printf("Enter student finalExam marks :");
            errNum = scanf("%f", &newStudent.finalExam_marks); // check scanf output success or not
            if (errNum <= 0)                                   // error handle for input integer marks
            {
                red();
                printf("\nSomething worng\n\n");
                exit(1);
            }
            if (newStudent.finalExam_marks > 50)
            {
                red();
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
        printf("Successfully Added new New Student\n");
        reset();
        studentListSize++;
        writeFile();
        readFile();
        printStudentList();
    }
    else
    {
        red();
        printf("\n%s is an Existing student! Try Again\n\n", newStudent.student_index);
        reset();
        addStudent();
    }
}
//-----------------------------update existing student from student marks list---------------------------
void updateStudent()
{
    student_marks newStudent; // declare structure type new student
    int errNum;               // initialize the error number
    while (1)
    {
        printf("Enter student regNo (EG/xxxx/xxxx) : ");
        errNum = scanf("%s", newStudent.student_index); // check scanf output success or not
        if (errNum <= 0)                                // error handle for input integer marks
        {
            red();
            printf("\nSomething worng\n\n");
            exit(1);
        }

        bool isCheck = isRegNoCorrect(newStudent.student_index); // check format of user input reg.No
        if (isCheck)                                             // error handling
        {
            break;
        }
        else
        {
            red();
            printf("Entered Student Registraion Number in correct Format!.Try again\n");
            reset();
        }
    }
    strcpy(newStudentRegNo, newStudent.student_index); // string copy
    bool isNewStudent = isExistingStudent();           // check validity
    if (!isNewStudent)
    {
        red();
        printf("\nThis Student not in list\n\n"); // can not do process
        reset();
    }
    else
    {
        // printf("This Student in the list you can update this student\n"); // can do process
        int errNum;
        while (1)
        {
            printf("Enter student assignment 01 marks : ");
            errNum = scanf("%f", &newStudent.assgnmt01_marks); // check scanf output success or not
            if (errNum <= 0)                                   // error handle for input integer marks
            {
                red();
                printf("\nSomething worng\n\n");
                exit(1);
            }
            if (newStudent.assgnmt01_marks > 15)
            {
                red();
                printf("Assgnmt 01 marks can't exceed higher than 15 \n");
                reset();
            }
            else
            {
                break;
            }
        }

        while (1)
        {
            printf("Enter student assignment 02 marks : ");
            errNum = scanf("%f", &newStudent.assgnmt02_marks); // check scanf output success or not
            if (errNum <= 0)                                   // error handle for input integer marks
            {
                red();
                printf("\nSomething worng\n\n");
                exit(1);
            }
            if (newStudent.assgnmt02_marks > 15)
            {
                red();
                printf("Assignment 02 marks can't exceed higher than 15 \n");
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
            errNum = scanf("%f", &newStudent.project_marks); // check scanf output success or not
            if (errNum <= 0)                                 // error handle for input integer marks
            {
                red();
                printf("\nSomething worng\n\n");
                exit(1);
            }
            if (newStudent.project_marks > 20)
            {
                red();
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
            errNum = scanf("%f", &newStudent.finalExam_marks); // check scanf output success or not
            if (errNum <= 0)                                   // error handle for input integer marks
            {
                red();
                printf("\nSomething worng\n\n");
                exit(1);
            }
            if (newStudent.finalExam_marks > 50)
            {
                red();
                printf("FinalExam  marks  can't exceed higher than 250\n");
                reset();
            }
            else
            {
                break;
            }
        }
        studentList[studentIndex] = newStudent;
        yellow();
        printf("Successully updated %s Student\n", newStudent.student_index);
        reset();
        writeFile();
        readFile();
        printStudentList();
    }
}
//------------------------------delete a student from student marks list-------------------------
void deleteStudent()
{
    student_marks newStudent;
    int errNum;
    while (1)
    {
        printf("Enter student regNo (EG/xxxx/xxxx) : ");
        errNum = scanf("%s", newStudent.student_index); // check scanf output success or not
        if (errNum <= 0)                                // error handle for input integer marks
        {
            red();
            printf("\nSomething worng\n\n");
            exit(1);
        }
        bool isCheck = isRegNoCorrect(newStudent.student_index); // check format of the list
        if (isCheck)
        {
            break;
        }
        else
        {
            red();
            printf("Entered Student Registraion Number in correct Format!.Try again\n");
            reset();
        }
    }
    strcpy(newStudentRegNo, newStudent.student_index); // string copy
    bool isNewStudent = isExistingStudent();           // check validity of new student with existing student list
    if (!isNewStudent)
    {
        red();
        printf("\nThis Student is not in the list.You can't delete\n\n");
        reset();
    }
    else
    {

        // printf("Existing student %s delete successfully \n", newStudentRegNo);
        char userInput[5];
        while (1)
        {

            red();
            printf("Are you sure want to delete this Student ? It will Permanently remove the student marks from list! (Y or N) :");
            reset();

            scanf("%s", userInput);
            if (!strcmp(userInput, "Y") || !strcmp(userInput, "y"))//string comparison
            {
                //   printf("index : %d\n", studentIndex);
                removeStudentFromArray();//student remove from list
                yellow();
                printf("The %s Student  delete successfully\n", newStudent.student_index);
                reset();
                writeFile();
                readFile();
                printStudentList();
                break;
            }
            else if (!strcmp(userInput, "N") || !strcmp(userInput, "n"))
            {
                while (1)
                {
                    printf("Do you want to Navigate to menu again? (Y or N) : ");
                    scanf("%s", userInput);
                    if (!strcmp(userInput, "Y") || !strcmp(userInput, "y"))
                    {
                        greetings();
                        break;
                    }
                    else if (!strcmp(userInput, "N") || !strcmp(userInput, "n"))
                    {
                        yellow();
                        printf("programe closed!\n");
                        reset();
                        break;
                    }
                    else
                    {
                        red();
                        printf("Please enter valid command !\n");
                        reset();
                    }
                }
            }
            else
            {
                red();
                printf("Please enter valid command !\n");
                reset();
            }
        }
    }
}
//----------------------------print all student marks to the console----------------------------------
void printStudentList()
{
    // printf("%d\n", studentListSize);
    if (studentListSize == 0)
    {
    }
    else
    {
        int j = 0;
        yellow();
        printf("\nNo\tStudent RegNo\tAssignment 1 marks\tAssignment 2 marks\tProject Marks\tFinal Exam Marks\n\n");
        reset();
        for (int i = 0; i < studentListSize; i++)
        {
            j++;
            printf("%d\t%s\t%f\t\t%f\t\t%f\t%f\n", i + 1, studentList[i].student_index, studentList[i].assgnmt01_marks, studentList[i].assgnmt02_marks, studentList[i].project_marks, studentList[i].finalExam_marks);

            // printf("%d : %s \n", i + 1, studentList[i].student_index);
        }
        yellow();
        printf("\nTotal Number of students in list : %d \n\n", j);
        reset();
    }
}
//-------------------------------check new student is in existing student marks array--------------------
bool isExistingStudent()
{
    studentIndex = 0;
    // printf("Size student list %d\n", studentListSize);
    for (int i = 0; i < studentListSize; i++)
    {

        // printf("%d : %s %s\n", i + 1, studentList[i].student_index, newStudentRegNo);
        if (!strcmp(studentList[i].student_index, newStudentRegNo))
        {

            return true;
        }
        studentIndex++;
    }
    return false;
}

//-------------------------write student data to the document-------------------
void writeFile()
{
    int fd;
    fd = open(fileName, O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) // handle error for file open
    {
        printf("Erro number: %d\n", errno);
        perror("my_file1");
        exit(1);
    }
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
    close(fd); // close the file
}
//----------------------------remove one student details from array ---------------
void removeStudentFromArray()
{
    studentList[studentIndex] = studentList[studentListSize - 1];
    studentListSize--;

}

//-----------------------------navigate to the menu pannel---------------------------
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
    {
        yellow();
        printf("programe closed!\n");
        reset();
    }
    else
    {
        red();
        printf("Please enter valid command !\n");
        reset();
        backToMenu();
    }
}
//------------------------------user mannullya add 100 student marks to the ducument--------------------------
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
        int errNum;
        while (i < listSize)
        {

            while (1)
            {
                printf("Enter %d student regNo (EG/xxxx/xxxx) : ", i + 1);
                errNum = scanf("%s", newStudent.student_index);
                if (errNum <= 0) // error handle for input integer marks
                {
                    red();
                    printf("\nSomething worng\n\n");
                    exit(1);
                }

                bool isCheck = isRegNoCorrect(newStudent.student_index); // check student index is in correct format
                if (isCheck)
                {
                    break;
                }
                else
                {
                    red();
                    printf("Entered Student Registraion Number in correct Format!.Try again\n");
                    reset();
                }
            }

            strcpy(newStudentRegNo, newStudent.student_index); // string copy
            bool isNewStudent = isExistingStudent();           // check student reg.no validity with current student array
            if (!isNewStudent)
            {

                while (1)
                {
                    printf("Enter student assignmet 01 marks : ");
                    errNum = scanf("%f", &newStudent.assgnmt01_marks);
                    if (errNum <= 0) // error handle for input integer marks
                    {
                        red();
                        printf("\nSomething worng\n\n");
                        exit(1);
                    }
                    if (newStudent.assgnmt01_marks > 15)
                    {
                        red();
                        printf("Assignment 01 marks can't exceed higher than 15 \n");
                        reset();
                    }
                    else
                    {
                        break;
                    }
                }

                while (1)
                {
                    printf("Enter student assignment 02 marks : ");
                    errNum = scanf("%f", &newStudent.assgnmt02_marks);
                    if (errNum <= 0) // error handle for input integer marks
                    {
                        red();
                        printf("\nSomething worng\n\n");
                        exit(1);
                    }
                    if (newStudent.assgnmt02_marks > 15)
                    {
                        red();
                        printf("Assgnmt 02 marks can't exceed higher than 15 \n");
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
                    errNum = scanf("%f", &newStudent.project_marks);
                    if (errNum <= 0) // error handle for input integer marks
                    {
                        red();
                        printf("\nSomething worng\n\n");
                        exit(1);
                    }
                    if (newStudent.project_marks > 20)
                    {
                        red();
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

                    printf("Enter student final Exam marks : ");
                    scanf("%f", &newStudent.finalExam_marks);
                    if (newStudent.finalExam_marks > 50)
                    {
                        red();
                        printf("FinalExam  marks  can't exceed higher than 50\n");
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
        yellow();
        printf("Successfully added all Students to the List\n");
        reset();
    }
    else if (!strcmp(userInput, "N") || !strcmp(userInput, "n"))
    {
        backToMenu();
    }
    else
    {
        red();
        printf("Please enter valid command !\n");
        reset();
        addAllStudentMarks();
    }
}

//--------------------------------check registration number is in correct format-------------------------------------
bool isRegNoCorrect(char *name)
{
    bool isFormatTrue = true;
    char val2;
    for (int i = 0; name[i] != '\0'; i++)
    {
        val2 = name[i];
        if (isdigit(val2))
        {
        } // given character is digit or not
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

//------------------------operation for generate random student id and generate all marks randomly---------
void randomStudentGenerate()
{
    char userInput[5];
    while (1)
    {
        red();
        printf("All the previous data will remove before random generate Student with marks!.Are you sure want to proceed? (Y or N) : ");
        reset();

        scanf("%s", userInput);
        if (!strcmp(userInput, "Y") || !strcmp(userInput, "y")) // string compare
        {
            //   printf("index : %d\n", studentIndex);
            generateRandomData();
            yellow();
            printf("\nSuccessfully generate 100 Random Students\n");
            reset();

            printStudentList();
            break;
        }
        else if (!strcmp(userInput, "N") || !strcmp(userInput, "n"))
        {
            while (1)
            {
                printf("Do you want to Navigate to menu again? (Y or N) : ");
                scanf("%s", userInput);
                if (!strcmp(userInput, "Y") || !strcmp(userInput, "y"))
                {
                    greetings();
                    break;
                }
                else if (!strcmp(userInput, "N") || !strcmp(userInput, "n"))
                {
                    yellow();
                    printf("programe closed!\n");
                    reset();
                    break;
                }
                else
                {
                    red();
                    printf("Please enter valid command !\n");
                    reset();
                }
            }
        }
        else
        {
            red();
            printf("Please enter valid command !\n");
            reset();
        }
    }
}
//-----------------------------generate student marks array by using randomly generated data-------------------
void generateRandomData()
{
    studentListSize = 0;
    for (int i = 0; i < listSize; i++)
    {
        student_marks student = randomStudent();
        studentList[i] = student;
        //   printf("%d\t%s\t%f\t%f\t%f\t%f\n",i+1,student.student_index,student.assgnmt01_marks,student.assgnmt02_marks,student.project_marks,student.finalExam_marks);
        studentListSize++;
    }

    writeFile();
    readFile();
}
//------------------------------------generate random number-------------------
float numGenarator(int lower, int upper)
{
    int num1 = (rand() % (upper - lower + 1)) + lower;
    int num2 = (rand() % (99 - 1));
    float num = num1 + (float)num2 / 100;
    if (num > upper)
    {
        num -= 1;
    }
    if (num < lower)
    {
        num += 1;
    }
    return num;
}
//-----------------generate student registration number randomly-------------------
int regNumberGen(int lower, int upper)
{
    int arrayLength = 0;
    int num = (rand() % (upper - lower + 1)) + lower;
    length = sizeof(regNumberList) / sizeof(regNumberList[0]);
    for (int i = 0; i < length; i++)
    {

        if (regNumberList[i] == num)
        {
            num = regNumberGen(lower, upper);
        }
    }
    regNumberList[length] = num;
    return num;
}
//----------------------------generate one student all marks and save to that student -------------
student_marks randomStudent()
{
    int num = regNumberGen(3300, 3600); // add range of student number
    char snum[100];
    sprintf(snum, "EG/2018/%d", num); // sting combine together
    student_marks tempStudent;
    strncpy(tempStudent.student_index, snum, 12);      // string copy
    tempStudent.assgnmt01_marks = numGenarator(0, 15); // get marks with relevent marks range
    tempStudent.assgnmt02_marks = numGenarator(0, 15); // get marks with relevent marks range
    tempStudent.project_marks = numGenarator(0, 20);   // get marks with relevent marks range
    tempStudent.finalExam_marks = numGenarator(0, 50); // get marks with relevent marks range
    return tempStudent;
}
