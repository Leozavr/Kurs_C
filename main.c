#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    int year;
    int month;
    int day;
    int hour;
    int minute;
    int temp;
    int status;
} record;

const int STATUS_GOOD = 0; // признак корректных данных
const int STATUS_INVALID = 1; // признак отсутствия данных

int get_int(FILE *file, int *v) //функция для перевода строки в число
{
    char buf[11]; //буфер для строки с данными
    int i = -1;
    char c;
    for (;;) //бесконечный цикл
    {
        c = getc(file);
        if (c == EOF)
        {
            return c;
        }
        i++;
        if ((c == ';') || (c == '\n')) //условие для окончания считывания точка с запятой или конец строки следует из формата подачи файла с данными
        {
            buf[i] = '\0';
            if (!sscanf(buf, "%d", v))
            {
                return STATUS_INVALID;
            }
            return 0;
        }
        else
        {
            buf[i] = c;
        }
    }
}

int get_rows_count(char *s) // определение количества строк в файле
{
    FILE *file_in;
    if ((file_in = fopen(s, "r")) == NULL)
    {
        fprintf(stdout, "%s\n", "не могу отрыть файл для чтения");
        exit(EXIT_FAILURE);
    }
    char c;
    int stringsInFile = 0;
    while (fscanf(file_in, "%c", &c) != EOF) //определяем количество строк в файле
    {
        if (c == '\n')
        {
            stringsInFile++;
        }
    }
    return stringsInFile;
}

int main(int agrc, char *argv[])
{
    if (!(agrc == 3 || agrc == 4))
    {
        fprintf(stdout, "Необходимо ввести: %s <Путь к файлу с данными> <Год> <Месяц>\n", "IoT.exe");
        exit(EXIT_FAILURE);
    }

    int stringsInFile = get_rows_count(argv[1]);
    // record *statistic = malloc(stringsInFile * sizeof(record));
    record statistic[stringsInFile];

    FILE *file_in;
    if ((file_in = fopen(argv[1], "r")) == NULL)
    {
        fprintf(stdout, "%s\n", "Не могу отрыть файл с данными для чтения");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < stringsInFile; i++)
    {
        get_int(file_in, &statistic[i].year);
        get_int(file_in, &statistic[i].month);
        get_int(file_in, &statistic[i].day);
        get_int(file_in, &statistic[i].hour);
        get_int(file_in, &statistic[i].minute);
        statistic[i].status = get_int(file_in, &statistic[i].temp);
    }
    fclose(file_in);

    int year;
    int month = 0;
    if (!sscanf(argv[2], "%d", &year))
    {
        fprintf(stdout, "%s\n", "bla bla bla");
        exit(EXIT_FAILURE);
    }
    if (agrc == 4) {
        if (!sscanf(argv[3], "%d", &month))
        {
            fprintf(stdout, "%s\n", "bla bla bla");
            exit(EXIT_FAILURE);
        }
    }
    
    for (int i = 0; i < stringsInFile; i++)
    {
        if (month == 0) {
            if ((statistic[i].year != year))
            {
                continue;
            }
        } else {
            if ((statistic[i].year != year) || (statistic[i].month != month))
            {
                continue;
            }
        }
        if (statistic[i].status == STATUS_INVALID)
        {
            fprintf(stdout, "%d. date: %d.%d.%d %d:%d temp: %s\n", i, statistic[i].year, statistic[i].month, statistic[i].day, statistic[i].hour, statistic[i].minute, "invalid");
        }
        else
        {
            fprintf(stdout, "%d. date: %d.%d.%d %d:%d temp: %d\n", i, statistic[i].year, statistic[i].month, statistic[i].day, statistic[i].hour, statistic[i].minute, statistic[i].temp);
        }
    }
    exit(EXIT_SUCCESS);
}
