#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<iostream>

#define MAXINODE 50

#define READ 1
#define WRITE 2

#define MAXFILESIZE 2048

#define REGULAR 1
#define SPECIAL 2

#define START 0
#define CURRENT 1
#define END 2

typedef struct superblock
{
    int TotalInodes;
    int FreeInode;

}SUPERBLOCK,*PSUPERBLOCK;

typedef struct inode
{
    char FileName[50];
    int InodeNumber;
    int FileSize;
    int FileActualSize;
    int FileType;
    char *Buffer;
    int ReferenceCount;
    int LinkCount;
    int permission;
    struct inode *next;
}
INODE,*PINODE,**PPINODE;

typedef struct filetable
{
    int readoffset;
    int writeoffset;
    int count;
    int mode;
    PINODE printnode;
}
FILETABLE,*PFILETABLE;

typedef struct ufdt
{
    PFILETABLE ptrfiletable;
}UFDT;

UFDT UFDTArr[50];
SUPERBLOCK SUPERBLOCKobj;
PINODE head=NULL;

PINODE Get_Inode(char * name)
{
    PINODE temp=head;
    int i=0;

    if(name == NULL)
    {
        return NULL;
    }

    while(temp !=NULL)
    {

        if(strcmp(name,temp->FileName)==0)
          break;

        temp=temp->next;
    }
    return temp;
}
void InitialiseSuperBlock()
{
    int i=0;

    while(i<MAXINODE)
    {
        UFDTArr[i].ptrfiletable=NULL;
        i++;
    }

    SUPERBLOCKobj.TotalInodes=MAXINODE;
    SUPERBLOCKobj.FreeInode=MAXINODE;
}
void CreateDILB()
{
    int i=1;

    PINODE newn=NULL;

    PINODE temp=head;

    while(i<MAXINODE)
    {
    newn=(PINODE)malloc(sizeof(INODE));

    newn->LinkCount=0;
    newn->ReferenceCount=0;
    newn->FileType=0;
    newn->FileSize=0;

    newn->Buffer=NULL;
    newn->next=NULL;

    newn->InodeNumber=i;

    if(temp==NULL)
    {
        head=newn;
        temp=head;
    }
    else
    {
        temp->next=newn;
        temp=temp->next;
    }
    i++;
    }
    printf("DILB create successfully\n");

}
void man(char *name)
{
     if(name == NULL)
        return;

    if(strcmp(name,"create")==0)
    {
        printf("Description: Used to create new regular file \n");
        printf("Usage : Create File_name Permission 1(read),2(write),3(R+W))\n");
    }
    else if(strcmp(name,"write")==0)
    {
        printf("Description: Used to writedata from regular file \n");
        printf("Usage :write File_name \n enter the data that we want to write\n");
    }
    else if(strcmp(name,"read")==0)
    {
        printf("Description: Used to read data from regular file \n");
        printf("Usage :read File_name \n ");
    }

     else if(strcmp(name,"ls")==0)
    {
        printf("Description: Used to list all informtion regular file \n");
        printf("Usage :ls\n");
    }
     else if(strcmp(name,"stat")==0)
    {
        printf("Description: Used to display information of file \n");
        printf("Usage :stat file_name\n");
    }
    else if(strcmp(name,"fstat")==0)
    {
        printf("Description: Used to display information of file \n");
        printf("Usage :stat file_Discriptor\n");
    }
    else if(strcmp(name,"truncate")==0)
    {
        printf("Description: Used to remove data from file \n");
        printf("Usage :truncate file_name\n");
    }
    else if(strcmp(name,"open")==0)
    {
        printf("Description: Used to close file open file \n");
        printf("Usage :open file_name\n");
    }
    else if(strcmp(name,"closeall")==0)
    {
        printf("Description: Used to close all opened files \n");
        printf("Usage :closeall\n");
    }
    else if(strcmp(name,"close")==0)
    {
        printf("Description: Used to close  opened files \n");
        printf("Usage :close file_name\n");
    }
    else if(strcmp(name,"lseeck")==0)
    {
        printf("Description: Used to change file offset \n");
        printf("Usage :lseek file_name Change inOffset StartPoint\n");
    }
    else if(strcmp(name,"rm")==0)
    {
        printf("Description: Used to delete file \n");
        printf("Usage :rm file_name\n");
    }
    else
    {
        printf("ERROR:No manual entry aailable\n");
    }
   
}
void DisplayHelp()
{
     printf("\n1.ls:To List out all files\n");
    printf("\n2.clear:To clear console\n");
    printf("\n3.Open:To open file\n");
    printf("\n4.close:To close file\n");
    printf("\n5.closeall : To close all the files\n");
    printf("\n6.read:To read the content of files\n");
    printf("\n7.write:To write content into file\n");
    printf("\n8.stat : To display information using name\n");
    printf("\n9.fstat:To Display information of file using descriptor\n");
    printf("\n10.truncate : To remove all the data from file\n");
    printf("\n11.rm:To Delete the file\n");
    printf("\n \t Thank You!!!!\n");

}
int GetFDFromeName(char *name)
{
    int i=0;

    while(i<50)
    {
    if(UFDTArr[i].ptrfiletable != NULL)
        if(strcmp((UFDTArr[i].ptrfiletable->printnode->FileName),name)==0)
                    break;

        i++;            
   
    }
    if(i == 50)
    {
        return -1;
    }

    else
    {
        return i;
    }

}


int rm_file(char * name)
{
      int fd=0;

      fd=GetFDFromeName(name);
      if(fd == -1)
        return -1;

    (UFDTArr[fd].ptrfiletable->printnode->LinkCount)--;

    if((UFDTArr[fd].ptrfiletable->printnode->LinkCount)==0);
    {

        (UFDTArr[fd].ptrfiletable->printnode->FileType=0);

        free(UFDTArr[fd].ptrfiletable);
        printf("\n File deleted Sucessfully!!");
    }

    UFDTArr[fd].ptrfiletable=NULL;

    (SUPERBLOCKobj.FreeInode)++;

return 0;
}

int ReadFile(int fd,char *arr,int isize)
{
    int read_size=0;

    if(UFDTArr[fd].ptrfiletable == NULL)
       return -1;

    if(UFDTArr[fd].ptrfiletable->mode != READ  && UFDTArr[fd].ptrfiletable->mode !=READ+WRITE)  
        return -2;
    
   if(((UFDTArr[fd].ptrfiletable->printnode->permission) != READ) && ((UFDTArr[fd].ptrfiletable->printnode->permission) != READ+WRITE))
       return -2;

    if(UFDTArr[fd].ptrfiletable->printnode->FileType != REGULAR)
    return -4;

    read_size=(UFDTArr[fd].ptrfiletable->printnode->FileActualSize)-(UFDTArr[fd].ptrfiletable->readoffset);

    if(read_size < isize)
    {
        strncpy(arr,(UFDTArr[fd].ptrfiletable->printnode->Buffer)+(UFDTArr[fd].ptrfiletable->readoffset),read_size);

        UFDTArr[fd].ptrfiletable->readoffset=UFDTArr[fd].ptrfiletable->readoffset + read_size;
    }

    else
    {
      strncpy(arr,(UFDTArr[fd].ptrfiletable->printnode->Buffer)+(UFDTArr[fd].ptrfiletable->readoffset),isize);

        UFDTArr[fd].ptrfiletable->readoffset=UFDTArr[fd].ptrfiletable->readoffset + isize;
      
    }
    
    return isize;
}

int WriteFile(int fd,char *arr,int isize)
{
   
    if(UFDTArr[fd].ptrfiletable->mode != WRITE  && UFDTArr[fd].ptrfiletable->mode !=READ+WRITE)  
        return -1;

    if((UFDTArr[fd].ptrfiletable->printnode->FileType)!= REGULAR)
        return -2;

    if((UFDTArr[fd].ptrfiletable->writeoffset)!= MAXFILESIZE)
        return -3;
    
    strncpy((UFDTArr[fd].ptrfiletable->printnode->Buffer)+(UFDTArr[fd].ptrfiletable->writeoffset),arr,isize);

    UFDTArr[fd].ptrfiletable->writeoffset=UFDTArr[fd].ptrfiletable->writeoffset + isize;

    UFDTArr[fd].ptrfiletable->printnode->FileActualSize=UFDTArr[fd].ptrfiletable->printnode->FileActualSize + isize;

    return isize;
}
int OpenFile(char *name,int mode)
{
    int i=0;

    PINODE temp=NULL;

    if(name == NULL || mode <=0)
        return -1;

    temp=Get_Inode(name);
    if(temp == NULL)
      return -2;

    while(i<50)
    {
         if(UFDTArr[i].ptrfiletable == NULL)
             break;
        
        i++;
    }

    UFDTArr[i].ptrfiletable=(PFILETABLE)malloc(sizeof(FILETABLE));
    if(UFDTArr[i].ptrfiletable == NULL)
         return -1;

    UFDTArr[i].ptrfiletable->count =1;
    UFDTArr[i].ptrfiletable->mode=mode;

    if(mode == READ)
    {
        UFDTArr[i].ptrfiletable->readoffset=0;
       
    }
    else if(mode ==  WRITE)
    {
        UFDTArr[i].ptrfiletable->writeoffset=0;

    }
     else if(mode == READ+ WRITE)
    {
        UFDTArr[i].ptrfiletable->readoffset=0;
        UFDTArr[i].ptrfiletable->writeoffset=0;

    }
 return i;
}

int CloseByFileName(char *name)
{
    int i=0;

    i=GetFDFromeName(name);

    if(i == -1)
        return -1;

    UFDTArr[i].ptrfiletable->readoffset=0;
    UFDTArr[i].ptrfiletable->writeoffset=0;

    (UFDTArr[i].ptrfiletable->printnode->ReferenceCount)--;

    return 0;
}

void CloseAllFile()
{
    int i=0;

    while(i<50)
    {

        if(UFDTArr[i].ptrfiletable != NULL)
        {
            UFDTArr[i].ptrfiletable->readoffset=0;
            UFDTArr[i].ptrfiletable->writeoffset=0;

            (UFDTArr[i].ptrfiletable->printnode->ReferenceCount)--;

        break;
        }
        i++;
    }

}

int LseekFile(int fd,int size,int form)
{
    
   if((fd<0)|| (form>2))
     return -1;

    if(UFDTArr[fd].ptrfiletable == NULL)
      return -1;

    if((UFDTArr[fd].ptrfiletable->mode == READ) || (UFDTArr[fd].ptrfiletable->mode == READ+ WRITE))
    {
        if(form == CURRENT)
        {
            if(((UFDTArr[fd].ptrfiletable->readoffset)+size)>((UFDTArr[fd].ptrfiletable->printnode->FileActualSize)))
                return -1;

            if(((UFDTArr[fd].ptrfiletable->readoffset)+size)<0) return -1;

            (UFDTArr[fd].ptrfiletable->readoffset) =(UFDTArr[fd].ptrfiletable->readoffset)+size;

        }

        else if(form == START)
        {
          if(size >(UFDTArr[fd].ptrfiletable->printnode->FileActualSize))  return -1;
           if(size <0) return -1;

         UFDTArr[fd].ptrfiletable->readoffset=size;
        }

        else if(form == END)
        {
          if((UFDTArr[fd].ptrfiletable->printnode->FileActualSize)+size >MAXFILESIZE)  return -1;
           
           if( (UFDTArr[fd].ptrfiletable->readoffset) +size <0) return -1;
                 (UFDTArr[fd].ptrfiletable->readoffset) = (UFDTArr[fd].ptrfiletable->printnode->FileActualSize)+size;

         
        }


    }
     else if(UFDTArr[fd].ptrfiletable->mode == WRITE)
     {

        if(form == CURRENT)
        {
             if(((UFDTArr[fd].ptrfiletable->writeoffset)+size)> MAXFILESIZE)
             return -1;

             if(((UFDTArr[fd].ptrfiletable->writeoffset)+size)<0)
             return -1;

             if(((UFDTArr[fd].ptrfiletable->writeoffset)+size)> (UFDTArr[fd].ptrfiletable->printnode->FileActualSize))
                (UFDTArr[fd].ptrfiletable->printnode->FileActualSize)=(UFDTArr[fd].ptrfiletable->writeoffset)+size;

                (UFDTArr[fd].ptrfiletable->writeoffset)=(UFDTArr[fd].ptrfiletable->writeoffset)+size;
        }

        else if(form == START)
        {
            if(size >MAXFILESIZE)
               return -1;

            if(size <0)
              return -1;

            if(size >(UFDTArr[fd].ptrfiletable->printnode->FileActualSize))
              (UFDTArr[fd].ptrfiletable->printnode->FileActualSize)=size;
        }

         else if(form == END)
        {
          if((UFDTArr[fd].ptrfiletable->printnode->FileActualSize)+size >MAXFILESIZE)  return -1;
           
         if( (UFDTArr[fd].ptrfiletable->writeoffset) +size <0) return -1;
                 (UFDTArr[fd].ptrfiletable->writeoffset) = (UFDTArr[fd].ptrfiletable->printnode->FileActualSize)+size;

         
        }

     }


return 0;

}

void Is_file()
{
   int i=0;
   PINODE temp=head;

   if(SUPERBLOCKobj.FreeInode == MAXINODE)
   {
     printf("Error:There are no files\n");
     return;
   }

   printf("\n File Name\tInode number\tFile Size\tLink Count\n");
   printf("----------------------------------------------------------------\n");

   while(temp!=NULL)
   {

    if(temp->FileType != 0)
    {
        printf("%s\t\t%d\t\t%d\t\t%d\n",temp->FileName,temp->InodeNumber,temp->FileActualSize,temp->LinkCount);
    }
    temp=temp->next;
   }
   printf("------------------------------------------------------------------------\n");

}

int fstat_file(int fd)
{
    PINODE temp=head;
    int i=0;

    if(fd <0)
    return -1;

    if(UFDTArr[fd].ptrfiletable == NULL)
      return -2;

    temp=UFDTArr[fd].ptrfiletable->printnode;

    printf("\n--------Statistical Information about file-------n");
    printf("File Name:%s\n",temp->FileName);
    printf("Inode Number:%d\n",temp->InodeNumber);
    printf("File Size:%d\n",temp->FileSize);
    printf("Actuall File Size:%d\n",temp->FileActualSize);
    printf("Link count:%d\n",temp->ReferenceCount);

    if(temp->permission ==1)
        printf("File Permission:Read Only\n");

    else if(temp->permission ==2)
        printf("File Permission:Write Only\n");

    else if(temp->permission ==3)
        printf("File Permission:Read & Write Only\n");

    printf("-------------------------------------------------------\n\n");

   return 0;

}

int stat_file(char * name)
{

    PINODE temp=head;
    int i=0;

    if(name == NULL)
    return -1;

    while(temp != NULL)
    {

         if(strcmp(name,temp->FileName)==0)
           break;

        temp=temp->next;
    }

    if(temp == NULL)
      return -2;

    
    printf("\n--------Statistical Information about file-------n");
    printf("File Name:%s\n",temp->FileName);
    printf("Inode Number:%d\n",temp->InodeNumber);
    printf("File Size:%d\n",temp->FileSize);
    printf("Actuall File Size:%d\n",temp->FileActualSize);
    printf("Link count:%d\n",temp->ReferenceCount);

    if(temp->permission ==1)
        printf("File Permission:Read Only\n");

    else if(temp->permission ==2)
        printf("File Permission:Write Only\n");

    else if(temp->permission ==3)
        printf("File Permission:Read & Write Only\n");

    printf("-------------------------------------------------------\n\n");

   return 0;
}

int truncate_file(char *name)
{
    int fd =GetFDFromeName(name);

    if(fd == -1)
    if(fd == -1)
        return -1;

    memset(UFDTArr[fd].ptrfiletable->printnode->Buffer,0,1024);
    UFDTArr[fd].ptrfiletable->readoffset=0;
    UFDTArr[fd].ptrfiletable->writeoffset=0;
    UFDTArr[fd].ptrfiletable->printnode->FileActualSize=0;

   return 0;
}

int CreateFile(char *name,int permission)
{
  int i=3;

  PINODE temp=head;

  if((name == NULL)||(permission==0)||(permission>3))
    return -1;

 if(SUPERBLOCKobj.FreeInode == 0)
   return -2;

(SUPERBLOCKobj.FreeInode)--;

if(Get_Inode(name)!= NULL)
 return -3;

while(temp != NULL)
{
    if(temp->FileType == 0)
        break;
    temp=temp->next;
}

 while(i<50)
 {
     if(UFDTArr[i].ptrfiletable == NULL)
       break;
    i++;
 }
   
   UFDTArr[i].ptrfiletable=(PFILETABLE)malloc(sizeof(FILETABLE));

   UFDTArr[i].ptrfiletable->count=1;
   UFDTArr[i].ptrfiletable->mode=permission;
   UFDTArr[i].ptrfiletable->readoffset=0;
   UFDTArr[i].ptrfiletable->writeoffset=0;
   UFDTArr[i].ptrfiletable->printnode=temp;

   strcpy(UFDTArr[i].ptrfiletable->printnode->FileName,name);
    UFDTArr[i].ptrfiletable->printnode->FileType=REGULAR;
    UFDTArr[i].ptrfiletable->printnode->ReferenceCount=1;
    UFDTArr[i].ptrfiletable->printnode->LinkCount=1;
    UFDTArr[i].ptrfiletable->printnode->FileSize=MAXFILESIZE;
    UFDTArr[i].ptrfiletable->printnode->FileActualSize=0;
    UFDTArr[i].ptrfiletable->printnode->permission=permission;
    UFDTArr[i].ptrfiletable->printnode->Buffer=(char *)malloc(MAXFILESIZE);
      

    return i;
}
int main()
{
 char *ptr=NULL;
 int ret=0,fd=0,count=0;
 char command[4][80],str[80],arr[1024];

 InitialiseSuperBlock();
 CreateDILB();

 while(1)
 {

        fflush(stdin);
        strcpy(str,"");

        printf("\n Customised VFS:>");

        fgets(str,80,stdin);

        count=sscanf(str,"%s %s %s %s",command[0],command[1],command[2],command[3]);

        if(count ==1)
        {
            if(strcmp(command[0],"ls")==0)
            {
                Is_file();
            }

            else if(strcmp(command[0],"closeall")==0)
            {
                CloseAllFile();
                printf("All files are sucessfully closed\n");
                continue;
            }
            else if(strcmp(command[0],"clear")==0)
            {
                system("clear");
                continue;
            }
            else if(strcmp(command[0],"help")==0)
            {
               DisplayHelp();
               continue;
            }
            else if(strcmp(command[0],"exit")==0)
            {
                printf("Terminating the My VFS\n");
                break;
            }
            else
            {
               printf("EROR:Command not found\n");
               continue;   
            }
        }

        else if(count ==2)
        {
           if(strcmp(command[0],"stat")==0)
            {
               ret=stat_file(command[1]);

               if(ret == -1)
                        printf("ERROR:Incorrect parametr \n");
               if(ret ==-2)
                        printf("ERROR:There is no such file \n");
                        continue;
            }

           else if(strcmp(command[0],"fstat")==0)
            {
               ret=fstat_file(atoi(command[1]));

               if(ret == -1)
                        printf("ERROR:Incorrect parametr \n");
               if(ret ==-2)
                        printf("ERROR:There is no such file \n");
                        continue;
            }

           else if(strcmp(command[0],"close")==0)
            {
               ret=CloseByFileName(command[1]);

               if(ret == -1)
                        printf("ERROR:Incorrect parametr \n");
               if(ret ==-2)
                        printf("ERROR:There is no such file \n");
                        continue;
            }

            else if(strcmp(command[0],"rm")==0)
            {
               ret=rm_file(command[1]);

               if(ret == -1)
                        printf("ERROR:Incorrect parametr \n");
               if(ret ==-2)
                        printf("ERROR:There is no such file \n");
                        continue;
            }

           else if(strcmp(command[0],"man")==0)
            {
              man(command[1]);
              continue;
            }

           else if(strcmp(command[0],"write")==0)
            {
               fd=GetFDFromeName(command[1]);

               if(fd == -1)
                   {
                        printf("ERROR:Incorrect parametr \n");
                        continue;
                   }
                   printf("Enter the data:\n");
                   fgets(arr,1024,stdin);


                   ret=strlen(arr);

                if(ret ==0)
                {
                    printf("ERROr: Incorect Parameter\n");
                    continue;
                }

                ret=WriteFile(fd,arr,ret);

                if(ret == -1)
                printf("ERROR:Permission denied\n");

                if(ret ==-2)
                        printf("ERROR:There is no sufficient memory to write \n");
                        
                if(ret == -3)
                        printf("ERROR:It is not a regular file\n");

            }

            else if(strcmp(command[0],"truncate")==0)
            {
               ret=truncate_file(command[1]);

               if(ret == -1)
                        printf("ERROR:Incorrect parametr \n");
              
            }


            else
            {
                printf("\n ERROR:Command not found\n");
                continue;
            }

        }


        else if(count== 3)
        {

            if(strcmp(command[0],"create")==0)
            {
               ret=CreateFile(command[1],atoi(command[2]));

               if(ret >= 0)
                        printf("File is sucessfully created with descriptor:%d \n",ret);
               if(ret == -1)
                        printf("ERROR:Incorrect parametr \n");
               if(ret ==-2)
                        printf("ERROR:There is no such file \n");
               if(ret == -3)
                        printf("File is already exists \n");     
               if(ret == -4)
                        printf("ERROR:Memory allocation failure \n");   

                    continue;
            }

        
            if(strcmp(command[0],"open")==0)
            {
               ret=OpenFile(command[1],atoi(command[2]));

               if(ret >= 0)
                        printf("File is sucessfully opened with descriptor:%d \n",ret);
               if(ret == -1)
                        printf("ERROR:Incorrect parametr \n");
               if(ret ==-2)
                        printf("ERROR:There is no such file \n");
               if(ret == -3)
                        printf("permission denied \n");     
               

                    continue;
            }

            else if(strcmp(command[0],"read")==0)
            {
               ret=GetFDFromeName(command[1]);

               if(fd == -1)
               {
                  printf("Error:Incorrect Parameter\n");
                  continue;
               }

               ptr=(char *)malloc(sizeof(atoi(command[2]))+1);

               if(ptr == NULL)
               {

                printf("ERROR:Memory allocation Failure\n");
                continue;
               }

               ret=ReadFile(fd,ptr,atoi(command[2]));

               if(ret == 0)
                        printf("file is empty \n");
               if(ret == -1)
                        printf("ERROR:File is not exists \n");
               if(ret ==-2)
                        printf("ERROR:permission denied \n");
               if(ret == -3)
                        printf("Error:reache at end of file \n");     
               if(ret == -4)
                        printf("ERROR:It is not regular file \n");   
                if(ret>0)
                {
                    write(2,ptr,ret);
                }  

                continue;      

            }

            else
            {

                printf("\n ERROR:Command not found\n");
                continue;
            }


        }

        else if(count ==4)
        {

            if(strcmp(command[0],"lseek")==0)
            {

                fd =GetFDFromeName(command[1]);

                if(fd == -1)
                {
                  printf("ERROR:Incorrect parameter\n");
                  continue;
                
                }

                ret=LseekFile(fd,atoi(command[2]),atoi(command[3]));

                if(ret == -1)
                {
                    printf("Unable to perforn lseek\n");
                }

            }
            else
            {   
                printf("nERROR:Command not found\n");
                continue;
            }
        }

        else
        {
            printf("\n ERROR:Command not found\n");
            continue;
        }
 }

    return 0;
}