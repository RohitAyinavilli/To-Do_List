#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX 200
char filename[]="To-Do_List.txt";

typedef struct Node
{
    char task[MAX];
    struct Node *next;
} 
Node;

Node *createNode(char task[]) 
{
    Node *newTask=(Node *)malloc(sizeof(Node));
    if(newTask==NULL) 
    {
        printf("Memory allocation failed!\n");
        return NULL;
    }

    strcpy(newTask->task,task);
    char temparr[MAX]="";
    strcpy(temparr,newTask->task);
    if(temparr[0]!='[')
    {
        char temp[MAX]="";
        strcpy(temp,"[ ] ");
        strcat(temp,newTask->task);
        strcpy(newTask->task,temp);
    }
    newTask->next = NULL;

    return newTask;
}

Node *insert(Node *head, char task[]) 
{
    if(head==NULL)
    head=createNode(task);
    
    else
    head->next=insert(head->next,task);

    return head;
}

Node *delete(Node *head, int pos) 
{
    if(head==NULL) 
    {
        printf("There are no tasks left!\n");
        return NULL;
    }

    int n=0;
    Node *temp=head;
    while(temp!=0)
    {
        temp=temp->next;
        n++;
    }

    if(pos<1 || pos>n)
    {
        printf("Invalid Position chosen\n");
        return head;
    }

    
    Node *current=head;
    Node *prev=NULL;
    
    if(pos==1)
    {
        head=head->next;
        free(current);
        return head;
    }

    for(int i=0;i<(pos-1);i++)
    {
        prev=current;
        current=current->next;
    }
    
    prev->next=current->next;
    free(current);

    return head;
}

Node *taskDone(Node *head, int pos)
{
    Node *temp=head;
    for(int i=0;i<(pos-1);i++)
    temp=temp->next;

    char temparr[MAX]="";
    strcpy(temparr,temp->task);
    if(temparr[1]==' ')
    {
        temparr[1]='X';
        strcpy(temp->task,temparr);
    }
    else
    printf(">>>> Task Already Completed <<<<\n");

    return head;
}


void saveAndOpenFile(Node *head)
{
    FILE *fileptr;
    Node *temp=head;
    
    fileptr=fopen(filename,"w");
    
    while(temp!=NULL)
    {
        fputs(temp->task,fileptr);
        temp=temp->next;
    }

    fclose(fileptr);
    
    char command[100];
    sprintf(command, "start %s", filename);
    system(command);

}

Node *loadFromFile()
{
    FILE *fileptr=fopen(filename,"r");
    if(fileptr==NULL)
    return NULL;

    Node *head=NULL;
    char task[MAX];
    while(fgets(task,MAX,fileptr)!=NULL)
    head=insert(head,task);

    fclose(fileptr);

    return head;
}

void displayTasks()
{
    int i=1;
    FILE *fileptr=fopen(filename,"r");
    char task[MAX];
    while(fgets(task,MAX,fileptr)!=NULL)
    {
        printf("%d) -- %s",i,task);
        i++;
    }
    fclose(fileptr);
    printf("\n");
}

int main()
{
    Node *head=loadFromFile();
    int option=0;

    while(option!=6)
    {
        printf("1. Add new Task\n");
        printf("2. Mark a Task as completed\n");
        printf("3. Delete a Task\n");
        printf("4. View all my Tasks\n");
        printf("5. Empty all the tasks\n");
        printf("6. Exit\n");
        printf(">>>> Choose your option: ");
        scanf("%d", &option);
        getchar();

        switch(option) 
        {
            case 1:
            {
                printf("Enter the task to be added: ");
                char task[MAX];
                fgets(task,MAX,stdin);
                head=insert(head,task);
                saveAndOpenFile(head);
                break;
            }

            case 2: 
                displayTasks();
                printf("Select which task is completed: ");
                int pos;
                scanf("%d",&pos);
                head=taskDone(head,pos);
                saveAndOpenFile(head);
                break;
            
            case 3:
            {
                displayTasks();
                printf("Select which task to be deleted: ");
                int pos;
                scanf("%d",&pos);
                head=delete(head,pos);
                saveAndOpenFile(head);
                break;
            }

            case 4:
            {
                saveAndOpenFile(head);
                break;
            }

            case 5:
            {
                head=NULL;
                FILE *fileptr=fopen(filename,"w");
                fclose(fileptr);
                saveAndOpenFile(head);
                break;
            }

            case 6:
                printf(">>>> Successfully exited <<<<\n");
                break;
            
            default:
                printf(">>>> Please choose one of below options <<<<\n");
                break;
        }
    }
    return 0;
}