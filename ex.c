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

int main()
{
    char codeNo[5];
    int code;
 
    while (1)
    {
        printf("Enter code : ");
        scanf("%s", codeNo);

      
        printf("code is : %s \n", codeNo);

        if (!strcmp(codeNo, "001"))
        {
            printf("True\n");
            break;
        }
        else
        {
            printf("false\n");
        }
     /* if (codeNo == "001" || codeNo == "002" || codeNo == "003" || codeNo == "004" || codeNo == "005")
        {
            if (codeNo == "001")
            {
                code = 1;
            };
            if (codeNo == "002")
            {
                code = 2;
            };
            if (codeNo == "003")
            {
                code = 3;
            };
            if (codeNo == "004")
            {
                code = 4;
            };
            if (codeNo == "005")
            {
                code = 5;
            };
            // selectOperation(code);
            break;
        }

        */
    }

    return 0;
}
