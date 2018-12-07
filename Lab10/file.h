#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<pthread.h>
#include<unistd.h>
#define NODE struct node
void insert();
void list();
void show_dept();
int check_duplicate();
void remve();
void showInfo();
void changeDept();
void readFromFile();
void saveToFile();
void delete();
void* autoSave();
void readAutoSave();
int main();

extern pthread_mutex_t mutex;

union injuries
{
        float fever;
        char kindOfPain[20];
        int numDaysSick;
};

struct node
{
        char entryName[20];
        int deptNum;
        union injuries injury;
        NODE *next;
        NODE *prev;
};

extern struct LIST
{
        NODE *head;
        NODE *tail;
}LIST;

extern struct LIST depts[4];
