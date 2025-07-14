#include <mqueue.h>
#include <fcntl.h> 
#include <sys/stat.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define QUEUE_NAME  "/myqueue"

typedef struct {
    long priority;
    char name[50];
    int old;
    char hometown[100];
    char phone_number[100];
} message_t;

void trim_newline(char *str) {
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n')
        str[len - 1] = '\0';
}

void show_menu(){
    printf("------MENU------\n");
    printf("01-Enter information from keyboard\n");
    printf("02-Show information through ID\n");
    printf("03-Show full studen\n");
    printf("04-Delete information through ID\n");
    printf("05-Exit\n");
    printf("----------\n");
    printf("Please choose function you want:....");
}

void function01(mqd_t mqid){
    message_t message;
    printf("01-Enter information from keyboard\n");
    
    printf("Please enter ID (ID > 0): ");
    scanf("%ld", &message.priority);
    getchar();

    printf("Please enter name of student: ");
    fgets(message.name, sizeof(message.name), stdin);
    trim_newline(message.name);

    printf("Please enter the old of student: ");
    scanf("%d", &message.old);
    getchar();

    printf("Please enter hometown: ");
    fgets(message.hometown, sizeof(message.hometown), stdin);
    trim_newline(message.hometown);

    printf("Please enter phone number: ");
    fgets(message.phone_number, sizeof(message.phone_number), stdin);
    trim_newline(message.phone_number);

    if (mq_send(mqid, (char* )&message, sizeof(message), 0) == -1) {
        perror("mq_send");
    } else {
        printf("✅ Added student.\n");
    }
}

void function02(mqd_t mqid){
    long id;
    printf("Please enter ID you want to show: ");
    scanf("%ld", &id);
    getchar();

    struct mq_attr attr;
    mq_getattr(mqid, &attr);
    long count = attr.mq_curmsgs;

    int founded = 0;
    message_t message;

    for(int i=0; i < count; i++){
        mq_receive(mqid, (char* )&message, sizeof(message), NULL);
        if(id == message.priority){
            printf("Found information student:\n");
            printf("ID: %ld, Name: %s, Old: %d, Hometown: %s, Phone number: %s\n",message.priority, message.name, message.old, message.hometown, message.phone_number);
            founded++;
        }
        mq_send(mqid, (char* )&message, sizeof(message), 0);
    }
    if(founded == 0){
        printf("There is no information in this list\n");
    }
}

void function03(mqd_t mqid){
    struct mq_attr attr;
    mq_getattr(mqid, &attr);
    long count = attr.mq_curmsgs;
    message_t message;
    printf("All students:\n");
    for(int i=0; i < count; i++){
        mq_receive(mqid, (char* )&message, sizeof(message), NULL);
        printf("Student %d:\n", i+1);
        printf("ID: %ld, Name: %s, Old: %d, Hometown: %s, Phone number: %s\n",message.priority, message.name, message.old, message.hometown, message.phone_number);
        mq_send(mqid, (char* )&message, sizeof(message), 0);
    }
}

void function04(mqd_t mqid){
    long id;
    printf("Please enter ID you want to delete: ");
    scanf("%ld", &id);
    getchar();

    struct mq_attr attr;
    mq_getattr(mqid, &attr);
    long count = attr.mq_curmsgs;

    int founded = 0;
    message_t message;

    for(int i=0; i < count; i++){
        mq_receive(mqid, (char* )&message, sizeof(message), NULL);
        if(id != message.priority){
            mq_send(mqid, (char* )&message, sizeof(message), 0);
        }else{
            printf("Deleted complete!\n");
            founded++;
        }
    }
    if(founded == 0){
        printf("There is no information that needed to delete in this list\n");
    }
}

void function05(mqd_t mqid){
    while(1){
        printf("Do you want to save information? (y/n)\n");
        char save;
        scanf(" %c", &save);        
        switch(save){
            case 'y':{
                mq_close(mqid);
                exit(EXIT_SUCCESS);
            }
            case 'n':{
                mq_close(mqid);
                mq_unlink(QUEUE_NAME);
                exit(EXIT_SUCCESS);
            }
            default:
                break;
        }
    }
}

int main(){
    //declare
    message_t message;
    struct mq_attr attr;
    int function_num;

    // Setup message queue attributes
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = sizeof(message_t);
    attr.mq_curmsgs = 0;

    //create message queue or open if available
    mqd_t mqid = mq_open(QUEUE_NAME, O_RDWR | O_CREAT | O_NONBLOCK, 0600, &attr);
    if (mqid == (mqd_t)-1) {
        perror("mq_open");
        exit(1);
    }

    while(1){
        //menu show function
        show_menu();
        scanf("%d", &function_num);

        switch (function_num)
        {
            case 1:{
                function01(mqid);
                sleep(1);
                break;
            }

            case 2:{
                function02(mqid);
                sleep(1);
                break;
            }
 
            case 3:{
                function03(mqid);
                sleep(1);
                break;
            }
            
            case 4:{
                function04(mqid);
                sleep(1);
                break;
            }

            case 5:{
                function05(mqid);
            }
            default:
                printf("❌ Chosen invalid!\n");
        }
    }
    

    return 0;
}