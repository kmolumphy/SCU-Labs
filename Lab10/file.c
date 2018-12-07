#include "file.h"
void *autoSave(void *file)
{
        int n;
        FILE *fp;
        while(n)
        {
                fp = fopen(file, "w");
                if(fp == NULL)
                {
                        return 0;
                }

                pthread_mutex_lock(&mutex);

                int i=1;
                for(; i<5; i++)
                {
                         struct node *current = depts[i].head;
                         while(current != NULL)
                        {
                                fwrite(current, sizeof(struct node), 1, fp);
                                current=current->next;
                        }
                }

                pthread_mutex_unlock(&mutex);
                fclose(fp);
                sleep(15);
        }
}

void readAutoSave(char* file, char* key)
{
        FILE *fp;
        fp = fopen(file,"r");
        if(fp == NULL)
                return;
        struct node current;

        pthread_mutex_lock(&mutex);

        while(fread(&current, sizeof(struct node), 1, fp) == 1)
        {
                if(current.deptNum == 3 || current.deptNum == 4)
                        {
                                printf("%s    -    %d    -    %i\n", current.entryName, current.deptNum,current.injury.numDaysSick);
                        }
                        if(current.deptNum == 2)
                        {
                                printf("%s    -    %d    -    %s\n", current.entryName, current.deptNum,current.injury.kindOfPain);
                        }
                        if(current.deptNum == 1)
                        {
                                printf("%s    -    %d    -    %f\n", current.entryName, current.deptNum,current.injury.fever);
                        }
        }

        pthread_mutex_unlock(&mutex);

        fclose(fp);
}

void delete(struct node* current) //Removes the entire list from memory
{
        struct node* temp;
        if(current == NULL)
                return;
        else
        {
                temp = current;
                free(current);
                delete(temp->next);
        }
}


void readFromFile(char fileName[], int key) //Reads the file and puts it into the array of structs
{
        FILE *fp;
        fp = fopen(fileName,"r");
        if(fp == NULL)
                return;

        char transfer[300];
        while(fgets(transfer, 300, fp) != NULL)
        {
                char tempName[20];
                int tempDept;
                union injuries tempInjury;
                for(int i=0; i<300; i++)
                {
                        transfer[i] = transfer[i]^key;
                }
                sscanf(transfer, "%s    %d", tempName, &tempDept);
                if(tempDept == 1)
                {
                        sscanf(transfer, "%s    %d      %f", tempName, &tempDept, &tempInjury.fever);
                        insert(tempDept, tempName, tempInjury);

                }
                if(tempDept == 2)
                {
                        sscanf(transfer, "%s    %d      %s", tempName, &tempDept, tempInjury.kindOfPain);
                        insert(tempDept, tempName, tempInjury);
                }
                if(tempDept == 3 || tempDept ==4)
                {
                        sscanf(transfer, "%s    %d      %d", tempName, &tempDept, &tempInjury.numDaysSick);
                        insert(tempDept, tempName, tempInjury);
                }

        }
        fclose(fp);
}

void saveToFile(char fileName[], int key) //Saves everything in the array of structs to the file
{
        FILE *fp;
        fp = fopen(fileName, "w");
        if(fp == NULL)
        {
                return;
        }
        int i = 1;
        for(;i<5; i++)
        {
                struct node *current = depts[i].head;
                while(current != NULL)
                {
                        char transfer[50];
                        if(current->deptNum == 3 || current->deptNum == 4)
                        {
                                sprintf(transfer, "%s   %d      %d", current->entryName, current->deptNum,current->injury.numDaysSick);

                                for(int i=0; i<50; i++)
                                {
                                        transfer[i] = transfer[i]^key;
                                }
                                fprintf(fp,"%s\n", transfer);
                        }

                        if(current->deptNum == 2)
                        {
                                sprintf(transfer, "%s   %d      %s", current->entryName, current->deptNum,current->injury.kindOfPain);
                                for(int i=0; i<50; i++)
                                {
                                        transfer[i] = transfer[i]^key;
                                }
                                fprintf(fp, "%s\n", transfer);
                        }

                        if(current->deptNum == 1)
                        {
                                sprintf(transfer, "%s   %d      %f", current->entryName, current->deptNum,current->injury.fever);
                                for(int i=0; i<50; i++)
                                {
                                        transfer[i] = transfer[i]^key;
                                }
                                fprintf(fp, "%s\n", transfer);
			}
                        current = current->next;
                }
        }
        fclose(fp);    
}

void changeDept() //Changes the department of an entry in the list
{
        char name[20];
        printf("Whose Department would you like to change?\n");
        scanf("%s", name);
        if(check_duplicate(name) == -1)
        {
                printf("There is nobody with the name %s in the waiting list", name);
                return;
        }
        int oldDeptNumber;
        printf("Which department are they in?\n");
        scanf("%d", &oldDeptNumber);

        int newDeptNumber;
        printf("Which department would you like them to be in?\n");
        scanf("%d", &newDeptNumber);
        union injuries u;
        struct node *current = depts[oldDeptNumber].head;


        //Trying to delete from old department
        int changed = 0;
        struct node *temp = depts[oldDeptNumber].head;
        while(temp != NULL)
        {
                int compare = strcmp(temp->entryName,name);
                if(compare == 0)
                {      
                        if(newDeptNumber == 1)
                        {
                                printf("How high is the fever?\n");
                                scanf("%f",&u.fever);
                        }
                        if(newDeptNumber == 2)
                        {
                                printf("What kind of pain is it?\n");
                                scanf("%s",u.kindOfPain);
                        }
                        if(newDeptNumber == 3 || newDeptNumber == 4)
                        {
                                printf("How many days have you been sick?\n");
                                scanf("%d",&u.numDaysSick);
                        }


                        if(depts[oldDeptNumber].head == depts[oldDeptNumber].tail)
                        {
                                depts[oldDeptNumber].head = depts[oldDeptNumber].tail = NULL;
                                changed++;
                        }
                        else if(temp == depts[oldDeptNumber].head)
                        {
                                depts[oldDeptNumber].head = depts[oldDeptNumber].head->next;
                                depts[oldDeptNumber].head->prev = NULL;
                                changed++;
                        }
                        else if(current == depts[oldDeptNumber].tail)
                        {
                                depts[oldDeptNumber].tail = depts[oldDeptNumber].tail->prev;
                                depts[oldDeptNumber].tail->next = NULL;
                                changed++;
                        }
			else
                        {
                                temp->prev->next = temp->next;
                                temp->next->prev = temp->prev;
                                free(temp);
                                changed++;
                        }

                }
                current = current->next;
                temp = temp->next;
        }
        if(changed == 0)
        {       
                printf("There is nobody named %s in department #%d", name, oldDeptNumber);
                return;
        }
        insert(newDeptNumber, name, u);
}


void showInfo(char name[]) //Shows the info of one person on the list
{
        if(check_duplicate(name) == 1)
        {
                printf("That person is not on the list!\n");
                return;
        }
        int i=1;
        for(;i<5; i++)
        {
                struct node *current = depts[i].head;
                while(current != NULL)
                {
                        if(strcmp(current->entryName,name) == 0)
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
                        }
                        current = current->next;
                }
        }
}

void insert(int dept, char name[],union injuries u) //Inserts someone into the list
{
        if(dept != 1 && dept != 2 && dept != 3 && dept != 4)
        {
                printf("Please choose a correct department!\n");
                return;
        }
        if(check_duplicate(name) == 0)
        {
                printf("Sorry that name was a duplicate\n");
                return;
        }

        struct node *current;
        current = (NODE*)malloc(sizeof(NODE));
        if(depts[dept].head == NULL)
        {
                depts[dept].head = current;
                depts[dept].tail = current;
                current->prev = NULL;
                current->next = NULL;
        }
        else
        {
                current->prev = depts[dept].tail;
                current->next = NULL;
                depts[dept].tail->next = current;
                depts[dept].tail = current;
        }
        current->injury.fever = u.fever;
        strcpy(current->injury.kindOfPain,u.kindOfPain);
        current->injury.numDaysSick = u.numDaysSick;
        strcpy(current->entryName,name);
        current->deptNum = dept;
}

void show_dept() //Shows everyone in a certain department
{
        printf("Which department would you like to check?\n");
        int deptSelect;
        scanf("%d",&deptSelect);
        printf("The list of names in the %d department is:\n", deptSelect);
        struct node *current = depts[deptSelect].head;
        while(current !=NULL)
	{
                if(current->deptNum == deptSelect)
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
                }
                current = current->next;
        }
}

int check_duplicate(char name[]) //Checks if there are multiple names
{
        int i = 1;
        for(;i<5; i++)
        {
                struct node *current = depts[i].head;
                while(current !=NULL)
                {
                        int compare = strcmp(current->entryName,name);
                        if(compare == 0)
                        {
                                return 0;
                        }
                        current=current->next;
                }
        }
        return 1;
}

void remve() //Removes the oldest entry from a department
{
        printf("Which department would you like to remove the oldest entry from?\n");
        int deptSelection;
        scanf("%d",&deptSelection);
        struct node *current = depts[deptSelection].head;
        while(current !=NULL)
        {
                if(current->deptNum == deptSelection)
                {
                        if(depts[deptSelection].head == depts[deptSelection].tail)
                                depts[deptSelection].head = depts[deptSelection].tail = NULL;
                        else if(current == depts[deptSelection].head)
                        {
                                depts[deptSelection].head = depts[deptSelection].head->next;
                                depts[deptSelection].head->prev = NULL;
                        }
                        else if(current == depts[deptSelection].tail)
                        {
                                depts[deptSelection].tail = depts[deptSelection].tail->prev;
                                depts[deptSelection].tail->next = NULL;
                        }
                        else
                        {
                                current->prev->next = current->next;
                                current->next->prev = current->prev;
                                free(current);
                        }
                        return;
                }
                current = current->next;
        }
        printf("Nobody is in that department!");
}



