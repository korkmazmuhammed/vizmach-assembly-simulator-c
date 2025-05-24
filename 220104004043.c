#include <stdio.h>
#include <string.h>

typedef struct
{
    char reg_a[9];
    char reg_b[9];
    char PC[17];
    char IR[25];
    char memory[108][9];
    char opname[30][5];
} MEMORY;

int to_binary(int n, char binary[8])
{
    int i;
    for (i = 7; i >= 0; i--)
    {
        binary[i] = (n % 2) + '0';
        n /= 2;
    }
}

void updatePC(int n, MEMORY *vizmach)
{
    char arr1[9];
    char arr2[9];
    int i, j;
    for (i = 15; i >= 0; i--)
    {
        (*vizmach).PC[i] = (n % 2) + '0';
        n /= 2;
    }
    for (j = 0; j < 8; j++)
    {
        arr1[j] = (*vizmach).PC[i];
    }
    for (i = 0; i < 8; i++)
    {
        arr2[i] = (*vizmach).PC[i];
    }
    strcpy((*vizmach).memory[103], arr1);
    strcpy((*vizmach).memory[104], arr2);
}
int two_power(int num, int my_power)
{
    int k;
    int result = 1;
    for (k = 0; k < my_power; k++)
    {
        result *= 2;
    }
    return result;
}
int to_decimal(char binary_number[])
{
    int k, j = 0;
    int decimal = 0;
    for (k = 7; k >= 0; k--, j++)
    {
        if (binary_number[k] == '1')
        {
            decimal += two_power(2, j);
        }
    }
    return decimal;
}

void init_opcodes(MEMORY *vizmach)
{
    FILE *fp = fopen("instructions.txt", "r");
    int opcode;
    char comma;
    char opname[30][5];
    int j = 0, k = 0;
    for (j = 0; j < 108; j++)
    {
        strcpy((*vizmach).memory[j], "00000000");
    }

    for (j = 0; j < 30; j++)
    {
        for (k = 0; k < 5; k++)
        {
            (*vizmach).opname[j][k] = '\0';
        }
    }

    for (k = 0; k < 8; k++)
    {
        (*vizmach).reg_a[k] = '0';
        (*vizmach).reg_b[k] = '0';
    }
    (*vizmach).reg_a[k] = '\0';
    (*vizmach).reg_b[k] = '\0';

    j = 0;
    k = 0;

    while (fscanf(fp, "%d", &opcode) == 1)
    {
        fscanf(fp, "%c%c%c", &comma, &comma, &comma);
        while (comma != ' ')
        {
            fscanf(fp, "%c", &comma);
            if (comma != ' ')
            {
                (*vizmach).opname[j][k++] = comma;
            }
        }

        j++;
        k = 0;
        while (fscanf(fp, "%c", &comma) != EOF && comma != '\n')
            ;
    }
}

int findOp(MEMORY *vizmach, char target[5])
{
    int i;
    for (i = 0; i < 30; i++)
    {
        if (strcmp((*vizmach).opname[i], target) == 0)
        {
            return i + 1;
        }
    }
}

int convert_num(char *str)
{
    int num = 0;
    int i = 0;
    int len = strlen(str);

    for (i = 0; i < len; i++)
    {
        num = num * 10 + (str[i] - '0');
    }
    return num;
}

void shift_L(char arr[], int times)
{
    int i = 0, j = 0;
    for (i = 0; i < times; i++)
    {
        for (j = 0; j < 7; j++)
        {
            arr[j] = arr[j + 1];
        }
        arr[7] = '0';
    }
}

void shift_R(char arr[], int times)
{
    char binary[8];
    int j = 0;
    int k = 7;
    for (j = 7 - times; j >= 0; j--)
    {
        binary[k--] = arr[j];
    }

    for (; k >= 0; k--)
    {
        binary[k] = '0';
    }
    strcpy(arr, binary);
}

int chef(MEMORY *vizmach, char catch1[], char catch2[], char catch3[])
{
    char binary[9] = {'0', '0', '0', '0', '0', '0', '0', '0', '\0'};
    int dec_num = to_decimal(catch1);
    int number;
    int numA;
    int numB;
    int i = 0;

    switch (dec_num)
    {
    case 1:

        int a;
        a = to_decimal((*vizmach).reg_a) + to_decimal((*vizmach).reg_b);
        to_binary(a, binary);
        if (strcmp(catch2, (*vizmach).memory[101]) == 0)
        {
            strcpy((*vizmach).reg_a, binary);
        }
        else
            strcpy((*vizmach).reg_b, binary);

        break;
    case 2:
        number = convert_num(catch3);
        if (strcmp(catch2, (*vizmach).memory[101]) == 0)
        {
            a = to_decimal((*vizmach).reg_a) + number;
            to_binary(a, binary);
            strcpy((*vizmach).reg_a, binary);
        }
        else
        {
            a = to_decimal((*vizmach).reg_b) + number;
            to_binary(a, binary);
            strcpy((*vizmach).reg_b, binary);
        }

        break;
    case 3:
        if (strcmp(catch2, (*vizmach).memory[101]) == 0)
        {
            number = to_decimal((*vizmach).reg_b);
            numA = to_decimal((*vizmach).memory[number - 1]);
            number = to_decimal((*vizmach).reg_a);
            number += numA;
            to_binary(number, binary);
            strcpy((*vizmach).reg_a, binary);
        }
        else
        {
            number = to_decimal((*vizmach).reg_b);
            numB = to_decimal((*vizmach).memory[number - 1]);
            number = to_decimal((*vizmach).reg_a);
            number += numB;
            to_binary(number, binary);
            strcpy((*vizmach).reg_b, binary);
        }

        break;
    case 4:
        for (i = 0; i < 8; i++)
        {
            if ((*vizmach).reg_a[i] == 1 && (*vizmach).reg_b[i] == 1)
                binary[i] = '1';
            else
                binary[i] = '0';
        }
        if (strcmp(catch2, (*vizmach).memory[101]) == 0)
        {
            strcpy((*vizmach).reg_a, binary);
        }
        else
        {
            strcpy((*vizmach).reg_b, binary);
        }

        break;
    case 5:
        number = convert_num(catch3);
        to_binary(number, binary);
        i = 0;
        if (strcmp(catch2, (*vizmach).memory[101]) == 0)
        {
            for (i = 0; i < 8; i++)
            {
                if ((*vizmach).reg_a[i] == 1 && binary[i] == 1)
                    (*vizmach).reg_a[i] = '1';
                else
                    (*vizmach).reg_a[i] = '0';
            }
        }
        else
        {
            for (i = 0; i < 8; i++)
            {
                if ((*vizmach).reg_b[i] == 1 && binary[i] == 1)
                    (*vizmach).reg_b[i] = '1';
                else
                    (*vizmach).reg_b[i] = '0';
            }
        }

        break;
    case 6:
        char temps[9];
        if (strcmp(catch2, (*vizmach).memory[101]) == 0)
        {
            number = to_decimal((*vizmach).reg_b);
            strcpy(binary, (*vizmach).memory[number - 1]);
            strcpy(temps, (*vizmach).reg_a);
            for (i = 0; i < 8; i++)
            {
                if (temps[i] == 0 && binary[i] == 0)
                    binary[i] = '0';
                else
                    binary[i] = '1';
                strcpy((*vizmach).reg_a, binary);
            }
        }
        else
        {
            number = to_decimal((*vizmach).reg_a);
            strcpy(binary, (*vizmach).memory[number - 1]);
            strcpy(temps, (*vizmach).reg_b);
            for (i = 0; i < 8; i++)
            {
                if (temps[i] == 0 && binary[i] == 0)
                    binary[i] = '0';
                else
                    binary[i] = '1';
                strcpy((*vizmach).reg_b, binary);
            }
        }

        break;
    case 7:
        for (i = 0; i < 8; i++)
        {
            if ((*vizmach).reg_a[i] == 0 && (*vizmach).reg_b[i] == 0)
                binary[i] = '0';
            else
                binary[i] = '1';
        }
        if (strcmp(catch2, (*vizmach).memory[101]) == 0)
        {
            strcpy((*vizmach).reg_a, binary);
        }
        else
        {
            strcpy((*vizmach).reg_b, binary);
        }

        break;
    case 8:
        number = convert_num(catch3);
        to_binary(number, binary);
        if (strcmp(catch2, (*vizmach).memory[101]) == 0)
        {
            for (i = 0; i < 8; i++)
            {
                if ((*vizmach).reg_a[i] == 0 && binary[i] == 0)
                    (*vizmach).reg_a[i] = '0';
                else
                    (*vizmach).reg_a[i] = '1';
            }
        }
        else
        {
            for (i = 0; i < 8; i++)
            {
                if ((*vizmach).reg_b[i] == 0 && binary[i] == 0)
                    (*vizmach).reg_b[i] = '0';
                else
                    (*vizmach).reg_b[i] = '1';
            }
        }
        break;
    case 9:
        char temp[9];
        if (strcmp(catch2, (*vizmach).memory[101]) == 0)
        {
            number = to_decimal((*vizmach).reg_b);
            strcpy(binary, (*vizmach).memory[number - 1]);
            strcpy(temp, (*vizmach).reg_a);
        }
        else
        {
            number = to_decimal((*vizmach).reg_a);
            strcpy(binary, (*vizmach).memory[number - 1]);
            strcpy(temp, (*vizmach).reg_b);
        }

        for (i = 0; i < 8; i++)
        {
            if (temp[i] == 0 && binary[i] == 0)
                binary[i] = '0';
            else
                binary[i] = '1';
        }

        break;
    case 10:
        int b;
        b = to_decimal((*vizmach).reg_a) - to_decimal((*vizmach).reg_b);
        to_binary(a, binary);
        if (strcmp(catch2, (*vizmach).memory[101]) == 0)
        {
            strcpy((*vizmach).reg_a, binary);
        }
        else
            strcpy((*vizmach).reg_b, binary);

        break;
    case 11:
        number = convert_num(catch3);
        if (strcmp(catch2, (*vizmach).memory[101]) == 0)
        {
            a = to_decimal((*vizmach).reg_a) - number;
            if (a < 0)
                a *= -1;
            to_binary(a, binary);
            strcpy((*vizmach).reg_a, binary);
        }
        else
        {
            a = to_decimal((*vizmach).reg_b) + number;
            to_binary(a, binary);
            strcpy((*vizmach).reg_b, binary);
        }
        break;
    case 12: /*TODO:bitmedi*/
        if (strcmp(catch2, (*vizmach).memory[101]) == 0)
        {
            number = to_decimal((*vizmach).reg_b);
            numA = to_decimal((*vizmach).reg_a);
            numB = to_decimal((*vizmach).memory[number - 1]);
            numB -= numA;
            if (numB < 0)
                numB *= -1;
            to_binary(numB, binary);
            strcpy((*vizmach).reg_a, binary);
        }
        else
        {
            number = to_decimal((*vizmach).reg_a);
            numB = to_decimal((*vizmach).reg_b);
            numA = to_decimal((*vizmach).memory[number - 1]);
            numA -= numB;
            if (numA < 0)
                numA *= -1;
            to_binary(numA, binary);
            strcpy((*vizmach).reg_b, binary);
        }

        break;
    case 13:
        strcpy(binary, catch2);
        number = to_decimal(binary);
        number += 1;
        to_binary(number, binary);
        if (strcmp(catch2, (*vizmach).memory[101]) == 0)
            strcpy((*vizmach).reg_a, binary);
        else
            strcpy((*vizmach).reg_b, binary);

        break;
    case 14:
        strcpy(binary, catch2);
        number = to_decimal(binary);
        number -= 1;
        to_binary(number, binary);
        if (strcmp(catch2, (*vizmach).memory[101]) == 0)

            strcpy((*vizmach).reg_a, binary);
        else
            strcpy((*vizmach).reg_b, binary);
        break;
    case 15:
        number = to_decimal(catch3);
        if (strcmp(catch2, (*vizmach).memory[101]) == 0)

        {
            strcpy(binary, (*vizmach).reg_a);
            shift_L(binary, number);
            strcpy((*vizmach).reg_a, binary);
        }
        else
        {
            strcpy(binary, (*vizmach).reg_b);
            shift_L(binary, number);
            strcpy((*vizmach).reg_b, binary);
        }

        break;
    case 16:
        number = to_decimal(catch3);
        if (strcmp(catch2, "01100110") == 0)
        {
            strcpy(binary, (*vizmach).reg_a);
            shift_R(binary, number);
            strcpy((*vizmach).reg_a, binary);
        }
        else
        {
            strcpy(binary, (*vizmach).reg_b);
            shift_R(binary, number);
            strcpy((*vizmach).reg_b, binary);
        }

        break;
    case 17:
        if (strcmp((*vizmach).reg_a, (*vizmach).reg_b) == 0)
        {
            if (strcmp(catch2, (*vizmach).memory[101]) == 0)
            {
                number = to_decimal((*vizmach).reg_a);
            }
            else
            {
                number = to_decimal((*vizmach).reg_b);
            }
            return number;
        }

        break;
    case 18:
        if (strcmp((*vizmach).reg_a, (*vizmach).reg_b) != 0)
        {
            if (strcmp(catch2, (*vizmach).memory[101]) == 0)
            {
                number = to_decimal((*vizmach).reg_a);
            }
            else
            {
                number = to_decimal((*vizmach).reg_b);
            }
            return number;
        }

        break;
    case 19:
        number = to_decimal((*vizmach).reg_a) + to_decimal((*vizmach).reg_b);
        return number;
        break;
    case 20:
        for (i = 0; i < 8; i++)
        {
            if ((*vizmach).reg_a[i] == 0 && (*vizmach).reg_b[i] == 0)
                binary[i] = '1';
            else
                binary[i] = '0';
        }
        if (strcmp(catch2, (*vizmach).memory[101]) == 0)
        {
            strcpy((*vizmach).reg_a, binary);
        }
        else
        {
            strcpy((*vizmach).reg_b, binary);
        }
        break;
    case 21:

        number = to_decimal(catch3);
        if (number == 101) /*if it is adresses of A*/
        {
            number = to_decimal((*vizmach).reg_a);
            strcpy((*vizmach).reg_a, (*vizmach).memory[number - 1]);
        }
        else
        {
            number = to_decimal((*vizmach).reg_b);
            strcpy((*vizmach).reg_b, (*vizmach).memory[number - 1]);
        }
        break;
    case 22:
        if (strcmp(catch2, (*vizmach).memory[101]) == 0)
        {
            number = to_decimal((*vizmach).reg_b);
            to_binary(number, binary);
            strcpy((*vizmach).reg_a, binary);
        }
        else
        {
            number = to_decimal((*vizmach).reg_a);
            to_binary(number, binary);
            strcpy((*vizmach).reg_b, binary);
        }

        break;
    case 23:
        strcpy(binary, (*vizmach).reg_a);
        strcpy((*vizmach).reg_a, (*vizmach).reg_b);
        strcpy((*vizmach).reg_b, binary);
        break;
    case 24:
        numA = to_decimal((*vizmach).reg_a);
        numB = to_decimal((*vizmach).reg_b);
        if (strcmp(catch2, (*vizmach).memory[101]) == 0)
        {
            if (numA < numB)
                numA = 1;
            else
                numA = 0;
            to_binary(numA, binary);
            strcpy((*vizmach).reg_a, binary);
        }
        else
        {
            if (numB < numA)
                numB = 1;
            else
                numB = 0;
            to_binary(numB, binary);
            strcpy((*vizmach).reg_b, binary);
        }
        break;
    case 25:
        numA = to_decimal((*vizmach).reg_a);
        numB = to_decimal((*vizmach).reg_b);
        if (strcmp(catch2, (*vizmach).memory[101]) == 0)
        {
            if (numA > numB)
                numA = 1;
            else
                numA = 0;
            to_binary(numA, binary);
            strcpy((*vizmach).reg_a, binary);
        }
        else
        {
            if (numB > numA)
                numB = 1;
            else
                numB = 0;
            to_binary(numB, binary);
            strcpy((*vizmach).reg_b, binary);
        }
        break;
    case 26:
        if (strcmp(catch2, (*vizmach).memory[101]) == 0)
            printf("%d\n", to_decimal((*vizmach).reg_a));
        else
            printf("%d\n", to_decimal((*vizmach).reg_b));

        break;
    case 27:
        printf("%d", to_decimal(catch2));
        break;
    case 28:
        number = to_decimal(catch2);
        if (number == 101)
        {
            number = to_decimal((*vizmach).reg_a);
        }
        else
            number = to_decimal((*vizmach).reg_a);

        break;
    case 29:
        if (strcmp(catch2, "01100110") == 0)
        {
            strcpy((*vizmach).reg_a, catch3);
            strcpy((*vizmach).memory[101], catch3);
        }
        else
        {
            strcpy((*vizmach).reg_b, catch3);
            strcpy((*vizmach).memory[102], catch3);
        }

        break;
    case 30:

        break;
    default:
        break;
    }
    return -1;
}

int init_mem(MEMORY *vizmach, FILE *fp)
{
    char catcher[5];
    int line = 0;
    char comma;
    char binary[9];

    int result = 0;
    int decimal = 0;

    while (fscanf(fp, "%s", catcher) != EOF)
    {
        result = findOp(vizmach, catcher);
        to_binary(result, binary);
        strcpy((*vizmach).memory[line++], binary);

        if (result != 30 && result != 28 && result != 27 && result != 26 && result != 14 && result != 13)
        {
            fscanf(fp, "%s", catcher);
            if (strcmp(catcher, "A") == 0)
            {
                to_binary(102, binary);
                strcpy((*vizmach).memory[line++], binary);
            }
            else
            {
                to_binary(103, binary);
                strcpy((*vizmach).memory[line++], binary);
            }
            fscanf(fp, "%s", catcher);
            if (strcmp(catcher, "M[A]") == 0)
            {
                to_binary(101, binary);
                strcpy((*vizmach).memory[line++], binary);
            }
            else if (strcmp(catcher, "M[B]") == 0)
            {
                to_binary(102, binary);
                strcpy((*vizmach).memory[line++], binary);
            }
            else if (strcmp(catcher, "A") == 0)
            {
                to_binary(102, binary);
                strcpy((*vizmach).memory[line++], binary);
            }
            else if (strcmp(catcher, "B") == 0)
            {
                to_binary(103, binary);
                strcpy((*vizmach).memory[line++], binary);
            }
            else
            {
                int number = convert_num(catcher);
                to_binary(number, binary);
                strcpy((*vizmach).memory[line++], binary);
            }
        }

        else
        {
            if (result == 14 && result == 13 && result == 27)
            {
                fscanf(fp, "%s", catcher);
                int number = convert_num(catcher);
                to_binary(number, binary);
                strcpy((*vizmach).memory[line++], binary);
            }
            else if (result != 30)
            {
                fscanf(fp, "%s", catcher);
                if (strcmp(catcher, "M[A]") == 0)
                {
                    to_binary(101, binary);
                    strcpy((*vizmach).memory[line++], binary);
                }
                else if (strcmp(catcher, "M[B]") == 0)
                {
                    to_binary(102, binary);
                    strcpy((*vizmach).memory[line++], binary);
                }
                else if (strcmp(catcher, "A") == 0)
                {
                    to_binary(102, binary);
                    strcpy((*vizmach).memory[line++], binary);
                }
                else if (strcmp(catcher, "B") == 0)
                {
                    to_binary(103, binary);
                    strcpy((*vizmach).memory[line++], binary);
                }
            }
            else
            {
                strcpy((*vizmach).memory[line++], "00000000");
            }
            strcpy((*vizmach).memory[line++], "00000000");
        }
    }
    return line;
}

int update_IR(MEMORY *vizmach, char catch1[], char catch2[], char catch3[])
{
    int i = 0;
    int j = 0;
    strcmp((*vizmach).memory[105], catch1);
    strcmp((*vizmach).memory[106], catch3);
    strcmp((*vizmach).memory[107], catch3);

    for (i = 0; i < 24; i++)
    {
        if (i < 8)
            (*vizmach).IR[i] = catch1[j++];
        else if (j < 16)
            (*vizmach).IR[i] = catch2[j++];
        else
            (*vizmach).IR[i] = catch3[j++];
        if (j == 8 || j == 16)
            j = 0;
    }
}

int main()
{
    MEMORY vizmach;
    FILE *fp;
    init_opcodes(&vizmach); /*init the opcodes*/
    strcpy((vizmach).memory[99], "00000101");
    strcpy((vizmach).memory[100], "00000110");
    char catch1[9];
    char catch2[9];
    char catch3[9];
    char filename[10];
    int flag = 1;
    while (flag)
    {
        printf("Enter the filename: ");
        scanf("%s", filename);
        fp = fopen(filename, "r");
        if (fp == NULL)
        {
            printf("WRONG FILENAME\n");
            flag = 0;
        }
        else
        {
            printf("Result of execution: ");
            int line = init_mem(&vizmach, fp);
            int i;
            int ci = -1;
            for (i = 0; i < line; i += 3)
            {
                updatePC(line + 1, &vizmach);
                strcpy(catch1, vizmach.memory[i]);
                strcpy(catch2, vizmach.memory[i + 1]);
                strcpy(catch3, vizmach.memory[i + 2]);
                update_IR(&vizmach, catch1, catch2, catch3);
                ci = chef(&vizmach, catch1, catch2, catch3);
                if (ci != -1)
                {
                    line = ci - 1;
                }
            }
        }
        printf("Do you want to execute another assembly code : Press(1) for yes Press(0) for exit: ");
        scanf(" %d", &flag);
    }
}