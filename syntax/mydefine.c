#include "stdio.h"
#include "unistd.h"

struct person
{
    char *name;
    int age;
    int sex;
};

#define INIT_SEX(sex) .sex = 1,

#define INIT_PERSON(person) \
    {                       \
        .name = "axa",      \
        INIT_SEX(sex)       \
            .age = 12,      \
    };

int main()
{
    struct person xiaoming = INIT_PERSON(xiaoming);
    printf("%s,%d,%d\n", xiaoming.name, xiaoming.age, xiaoming.sex);
}