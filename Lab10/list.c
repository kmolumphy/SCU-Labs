#include "file.h"
void list() //Lists everyone on the list
{
        printf("The list of names is...\n");
        int i = 1;
        for(;i<5; i++)
        {
                struct node *current = depts[i].head;
                while(current !=NULL)
                {
                        if(current->deptNum == 3 || current->deptNum == 4)
                        {
                                printf("%s    -    %d    -    %i\n", current->entryName, current->deptNum,current->injury.numDaysSick);
                        }
                        if(current->deptNum == 2)
                        {
                                printf("%s    -    %d    -    %s\n", current->entryName, current->deptNum,current->injury.kindOfPain);
                        }
                        if(current->deptNum == 1)
                        {
                                printf("%s    -    %d    -    %f\n", current->entryName, current->deptNum,current->injury.fever);
                        }
                        current = current->next;
                }
        }
}

