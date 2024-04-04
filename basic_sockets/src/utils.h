#ifndef _UTILS_H
#define _UTILS_H

#define S1 "aries"
#define S2 "tauro"
#define S3 "geminis"
#define S4 "cancer"
#define S5 "leo"
#define S6 "virgo"
#define S7 "libra"
#define S8 "escorpio"
#define S9 "sagitario"
#define S10 "capricornio"
#define S11 "acuario"
#define S12 "piscis"

struct Date stringToDate(char *dateString);

struct Date
{
    int year;
    int month;
    int day;
};

#endif