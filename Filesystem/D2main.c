//D2Main.c

#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>
#include"D2.h"

int main(void){
    char choice[MAX_STR];
    enum CmdVal ValCmd;
    int retval;
    bool go_on = true;
    char m_cmd[10];
    char m_arg[20];
    M_FILESYSTEM* fs_head;      //Points to filesystem.
    M_DIRECTORY* cur_dir,*temp_dir;       //Points to current working direcory

    fs_head=allocate_fs("root");
    cur_dir= fs_head->rootDirectory;    //Current directory is root
    while(go_on){
        m_arg[0]=0;
        printf("\nChoose one of the following options:\n cd <dirname> \n touch <filename> \n mkdir <dirname> \n ls \n exit:");
        scanf(" %[^\n]s",choice);
        sscanf(choice, "%s%s", m_cmd, m_arg);
        ValCmd = ValidateCmd(m_cmd);

        switch(ValCmd){
            case CD_CMD:
                        if(m_arg[0] != 0){
                            if(strcmp(m_arg,"..")==0)
                                cur_dir = cur_dir->parent;
                            else{
                                temp_dir=cur_dir;
                                cur_dir = ChangeDirectory(fs_head, m_arg);
                                if(cur_dir==NULL)
                                    {
                                        printf("Directory does not exist");
                                        cur_dir=cur_dir->parent;
                                    }
                                else
                                    printf("Current Working directory is %s\n", m_arg);
                            }
                            printf("Current working directory is %s\n",cur_dir->directoryName);//optimize printing twice on success
                        }
                        else
                            printf("Second arg not provided\n");
            break;

            case MKDIR_CMD:
                        if(m_arg[0]!=0){
                            retval=MakeDirectory(cur_dir,m_arg);
                            if(retval==0)
                                printf("A new directory %s is created in the folder %s\n",m_arg,cur_dir->directoryName);
                        }
                        else
                            printf("Second arg not provided\n");
            break;

            case LS_CMD:
                        Showfiles(fs_head);
            break;

            case TOUCH_CMD:
                        if(m_arg[0]!=0){
                            retval=CreateFile(cur_dir,m_arg);
                            if(retval=0)
                                printf("A file %s created in folder %s\n",m_arg, cur_dir->directoryName);
                        }
                        else
                            printf("Second arg not provided");
            break;

            case EXIT_CMD:
                        printf("Exit command is chosen\n");
                        go_on=false;
            break;

            default : 
                        printf("Unsupported Command...\n");
            break;
        }
    }
    free_mem(fs_head);//Need to implement on own
    return 0;
}