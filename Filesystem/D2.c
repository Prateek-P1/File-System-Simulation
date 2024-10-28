//D2.C

#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>
#include"D2.h"

//static array of structures used to validate commands throughout programs 
static CMDVAL s_cmds[]={{"cd",CD_CMD},{"mkdir",MKDIR_CMD},{"ls",LS_CMD},{"touch",TOUCH_CMD},{"exit",EXIT_CMD},{NULL,INVALID_CMD}};

int ValidateCmd(char *m_cmd){
    int i;
    for(i=0;s_cmds[i].s_cmd != NULL; i++)
        if(strcmp(m_cmd,s_cmds[i].s_cmd)==0)
            break;
    return (s_cmds[i].Val);
}

void InitDirName(M_DIRECTORY* p_mdir, char* dir_name){
    if((p_mdir->directoryName=(char*)malloc(strlen(dir_name)+1))==NULL){
        perror("Memory allocation failure for directoryName");
        exit(0);
    }
    strcpy(p_mdir->directoryName,dir_name);
}

M_DIRECTORY* ChangeDirectory(M_FILESYSTEM* p_fs, char* dirName){
    M_DIRECTORY* p_mdir=NULL;

    if(strcmp(dirName,"root")==0)
        p_mdir = p_fs->rootDirectory;
    else{
        for(p_mdir = p_fs->rootDirectory->subdirectories ; p_mdir != NULL ;p_mdir=p_mdir->next){
            if(strcmp(dirName,p_mdir->directoryName)==0)
                break;
        }
    }
    return p_mdir;
}

bool CheckDuplicates(M_DIRECTORY* p_mdir,char *dir_name){
    bool retval = false;
    M_DIRECTORY* p_temp;

    for(p_temp=p_mdir;p_temp!=NULL;p_temp=p_temp->next){
        if(strcmp(p_temp->directoryName,dir_name)==0){
            retval=true;
            break;
        }
    }
    return retval;
}

int MakeDirectory(M_DIRECTORY* p_curdir, char *dir_name){
    M_DIRECTORY* p_mdir;
    int retval=0;
    if(!CheckDuplicates(p_curdir->subdirectories,dir_name)){
        if((p_mdir = (M_DIRECTORY*)malloc(sizeof(M_DIRECTORY)))==NULL){
            perror("Mem alloc failure in allocate_directory 2");
            exit(0);
        }
        p_mdir->parent=p_curdir;
        InitDirName(p_mdir,dir_name);

        //if head of subdirectories is NULL
        if(p_curdir->subdirectories==NULL)
            p_curdir->subdirectories = p_mdir;
        else
            AddSibling(p_curdir->subdirectories,p_mdir);
        p_mdir->next=NULL;
        p_mdir->files=NULL;
    }
    else{
        retval=-1;
        printf("This name already exists\n");
    }
    return retval;
}


void AddSibling(M_DIRECTORY *p_curdir,M_DIRECTORY* newDir){
    M_DIRECTORY *p_cur,*p_prev;
    for(p_cur=p_curdir ; p_cur!=NULL ; p_prev=p_cur, p_cur=p_cur->next);
    p_prev->next=newDir;
    //todo idk if complete!!!
}

void Showfiles(M_FILESYSTEM* p_fs){
    M_DIRECTORY* p_mdir;
    M_DIRECTORY* p_temp;
    MYFILE* p_myfile;

    if(p_fs!=NULL){
        p_mdir=p_fs->rootDirectory;
        printf("%s\n",p_mdir->directoryName);
        p_mdir=p_mdir->subdirectories;
        //display names of all subdirectories under root

        if(p_mdir!=NULL){
            printf("\t%s",p_mdir->directoryName);
            for(p_temp=p_mdir->next ; p_temp!=NULL ; p_temp=p_temp->next)
                printf("\t%s",p_temp->directoryName);
        }
        printf("\n");
        p_mdir=p_fs->rootDirectory;

        //Display all files in the root directory
        if(p_mdir!=NULL){
            for(p_myfile=p_mdir->files; p_myfile!=NULL; p_myfile=p_myfile->next_file)
                printf("%s\n",p_myfile->file_name);
        }
        p_mdir=p_fs->rootDirectory->subdirectories;
        
        //Display files in subdirectories
        for(p_temp=p_mdir;p_temp!=NULL; p_temp=p_temp->next){
            printf("Directory name: %s\n",p_temp->directoryName);
            for(p_myfile=p_temp->files ; p_myfile!=NULL; p_myfile=p_myfile->next_file)
                printf("%s\t",p_myfile->file_name);
            printf("\n");
        }
    }
}

//Allocates memory to filesystem
M_FILESYSTEM* allocate_fs(char *dir_name){
    M_FILESYSTEM* p_mfs;

    if((p_mfs=(M_FILESYSTEM*)malloc(sizeof(M_FILESYSTEM)))==NULL){
        perror("memory allocation failure in allocate_directory");
        exit(0);
    }
    p_mfs->rootDirectory=allocate_directory(dir_name,NULL);
    return p_mfs;
}

//Allocates memory to directories
M_DIRECTORY* allocate_directory(char *dir_name, M_DIRECTORY* parent_dir){
    M_DIRECTORY* p_mdir;
    if((p_mdir=(M_DIRECTORY*)malloc(sizeof(M_DIRECTORY)))==NULL){
        perror("Memory allocation failure in allocate_directory 2");
        exit(0);
    }
    p_mdir->subdirectories=NULL;
    p_mdir->next=NULL;
    p_mdir->files=NULL;
    p_mdir->parent=parent_dir;
    InitDirName(p_mdir,dir_name);
    return p_mdir;
}

bool CheckDuplicateFiles(MYFILE* p_files,char *file_name){
    bool retval = false;
    MYFILE* p_temp;

    for(p_temp=p_files;p_temp!=NULL;p_temp=p_temp->next_file){
        if(strcmp(p_temp->file_name,file_name)==0){
            retval=true;
            break;
        }
    }
    return retval;
}

int CreateFile(M_DIRECTORY* p_mdir, char* fileName){
    MYFILE* p_myfile;
    MYFILE *p_temp,*p_prev;
    int len=strlen(fileName)+1;
    int retval=0;        //0 - success ; 1 - failure
    if(!CheckDuplicateFiles(p_mdir->files,fileName)){
        if((p_myfile=(MYFILE*)malloc(sizeof(MYFILE)))==NULL){
            printf("Malloc failure in CreateFile\n");
            exit(0);
        }
        p_myfile->next_file=NULL;

        if(p_mdir->files==NULL){
            p_mdir->files=p_myfile;
            if((p_mdir->files->file_name =(char*) malloc(len))==NULL){
                printf("Malloc failure in CreateFile\n");
                exit(0);
            }
            strcpy(p_mdir->files->file_name,fileName);
        }
        else{
            for(p_temp=p_mdir->files;p_temp!=NULL;p_prev=p_temp,p_temp=p_temp->next_file);
            p_prev->next_file=p_myfile;
            if((p_prev->next_file->file_name=(char*)malloc(len))==NULL){
                printf("Malloc failure in CreateFile\n");
                exit(0);
            }
            strcpy(p_prev->next_file->file_name,fileName);
        }
    }
    else{
        retval=-1;
        printf("A file with this name already exists in this dir\n");
    }
    return retval;
}



void free_mem(M_FILESYSTEM* p_fs){//fixme don't know if it works
    //implement on own
    M_DIRECTORY* p_mdir,*p_mdir_sub;
    M_DIRECTORY* p_mdir_temp, *p_temp_sub;
    MYFILE* p_myfile, *p_temp_file;

        //root
            //directories
                //files

    if(p_fs!=NULL){
        p_mdir = p_fs->rootDirectory;
        p_mdir_sub = p_mdir->subdirectories;
        p_myfile = p_mdir_sub->files;
        
        if(p_mdir->files!=NULL){
            for(p_temp_file=p_myfile ; p_temp_file->next_file!=NULL ; p_temp_file=p_myfile,p_myfile=p_myfile->next_file);
                free(p_temp_file);
                printf("freeing files under root\n");
        }

        for(p_mdir_temp=p_mdir;p_mdir_temp->next!=NULL;p_mdir_temp=p_mdir,p_mdir=p_mdir->next){
            for(p_temp_sub=p_mdir_sub ; p_temp_sub->next!=NULL;p_temp_sub=p_mdir_sub,p_mdir_sub=p_mdir_sub->next){
                for(p_temp_file=p_myfile ; p_temp_file->next_file!=NULL ; p_temp_file=p_myfile,p_myfile=p_myfile->next_file);
                free(p_temp_file);
                printf("freeing file\n");
            }
            free(p_temp_sub);
            printf("Freeing subdirs\n");
        }
        free(p_mdir_temp);
        printf("freeing dirs\n");
    }

    free(p_fs);
    printf("freeing root");
}
