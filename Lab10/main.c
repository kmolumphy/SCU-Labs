#include "file.h"
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
struct LIST depts[4];
int main(int argc, char* argv[])
{
	pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
	if(argc == 1)
        {
                printf("The name of the file is missing!\n");
                return 1;
        }

        if(argc == 2)
        {
                printf("The name of the second file is missing!\n");
                return 1;
        }

        if(argc == 3)
        {
                printf("The key is missing!\n");
                return 1;
        }

        int key;
        key = atoi(argv[3]);

        int q=1;
        for(;q<5; q++)
        {
                depts[q].head = NULL;
                depts[q].tail = NULL;
        }

        readFromFile(argv[1], key);

        pthread_t myPthread;
        pthread_mutex_init(&mutex, NULL);
        pthread_create(&myPthread, NULL, autoSave, argv[2]);


        int x = 0;
        while(x == 0)
        {
                printf("\nType 0 to quit.\nType 1 to add to the waiting list.\nType 2 to see the waiting list.\nType 3 to remove the oldest entry in a department\nType 4 to show the entries in a department\nType 5 to find a person's info.\nType 6 to switch an entry's department\nType 7 to print the last save\n:");
                int opt;
                scanf("%d",&opt);
                switch(opt)
                {
                        case 1:
                        {
                                char name[20];
                                int dept = 0;
                                union injuries u;
                                printf("What is the name?\n");
                                scanf("%s",name);
                                printf("What is the department?\n");
                                scanf("%i",&dept);
                                if(dept == 1)
                                {
                                        printf("How high is the fever?\n");
                                        scanf("%f",&u.fever);
                                }
                                if(dept == 2)
                                {
                                         printf("What kind of pain is it?\n");
                                        scanf("%s",u.kindOfPain);
                                }
                                if(dept == 3 || dept == 4)
                                {
                                        printf("How many days have you been sick?\n");
                                        scanf("%d",&u.numDaysSick);
                                }
                                insert(dept,name,u);
                                break;
                        }
                        case 2:
                        {
                                list();
                                break;
                        }
                        case 3:
                        {
                                remve();
                                break;
			}
                        case 4:
                        {
                                show_dept();
                                break;
                        }
                        case 5:
                        {
                                char name[20];
                                printf("Who's info would you like to see?\n");
                                scanf("%s",name);
                                showInfo(name);
                                break;
                        }
                        case 6:
                        {
                                changeDept();
                                break;
                        }
                        case 7:
                        {
                                readAutoSave(argv[2]);
                                break;
                        }
                        case 0:
                        {
                                saveToFile(argv[1], key);
                                int i=1;
                                for(; i<5; i++)
                                {
                                        struct node* current = depts[i].head;
                                        delete(current);
                                }
                                pthread_cancel(myPthread);
                                return 0;
                        }
                }
        }
        return 0;
}


