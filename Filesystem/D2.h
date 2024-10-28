//D2.h
#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>

#define MAX_STR 50

typedef struct file{
    char *file_name;
    struct file* next_file;
}MYFILE;

typedef struct m_directory{
    char* directoryName;
    struct m_directory *subdirectories;     //subdirs
    struct m_directory *next;       //next of current dir
    MYFILE *files;      //head of files linkedlist
    struct m_directory *parent;     //ptr to parent dir
}M_DIRECTORY;

typedef struct FileSystem{
    M_DIRECTORY* rootDirectory;
} M_FILESYSTEM;

enum CmdVal{CD_CMD, MKDIR_CMD, LS_CMD, TOUCH_CMD, EXIT_CMD, INVALID_CMD};
typedef struct cmdval{
    char *s_cmd;
    enum CmdVal Val;
}CMDVAL;


//
int ValidateCmd(char*);     //validates command, returns one of the enums in CMDVAL
//
M_FILESYSTEM* allocate_fs(char*);
//
M_DIRECTORY* allocate_directory(char*, M_DIRECTORY*);
//
int MakeDirectory(M_DIRECTORY*,char*);  //-1 ON FAILURE
//
M_DIRECTORY* ChangeDirectory(M_FILESYSTEM*, char*);
//
void Showfiles(M_FILESYSTEM*);
//
int CreateFile(M_DIRECTORY*,char*);      //-1 ON FAILURE


//
void AddSibling(M_DIRECTORY*,M_DIRECTORY*);
//
void free_mem(M_FILESYSTEM*);