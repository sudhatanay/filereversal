#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#define SIZE 20000

void displayfilestats(struct stat fs,char *file,char *message)
{
    /* Function to print file permissions */

    /* USER PERMISSIONS */

    if( fs.st_mode & S_IRUSR )
    {
        sprintf(message,"User has read permissions on %s: Yes\n",file);
        write(1,message,strlen(message));
    }
    else
    {
        sprintf(message,"User has read permissions on %s: No\n",file);
        write(1,message,strlen(message));
    }


    if( fs.st_mode & S_IWUSR )
    {
        sprintf(message,"User has write permissions on %s: Yes\n",file);
        write(1,message,strlen(message));
    }
    else
    {
        sprintf(message,"User has write permissions on %s: No\n",file);
        write(1,message,strlen(message));
    }


    if( fs.st_mode & S_IXUSR )
    {
        sprintf(message,"User has execute permissions on %s: Yes\n",file);
        write(1,message,strlen(message));
    }
    else
    {
        sprintf(message,"User has execute permissions on %s: No\n",file);
        write(1,message,strlen(message));
    }

    /* GROUP PERMISSIONS */

    if( fs.st_mode & S_IRGRP )
    {
        sprintf(message,"Group has read permissions on %s: Yes\n",file);
        write(1,message,strlen(message));
    }
    else
    {
        sprintf(message,"Group has read permissions on %s: No\n",file);
        write(1,message,strlen(message));
    }

    if( fs.st_mode & S_IWGRP )
    {
        sprintf(message,"Group has write permissions on %s: Yes\n",file);
        write(1,message,strlen(message));
    }
    else
    {
        sprintf(message,"Group has write permissions on %s: No\n",file);
        write(1,message,strlen(message));
    }

    if( fs.st_mode & S_IXGRP )
    {
        sprintf(message,"Group has execute permissions on %s: Yes\n",file);
        write(1,message,strlen(message));
    }
    else
    {
        sprintf(message,"Group has execute permissions on %s: No\n",file);
        write(1,message,strlen(message));
    }

    /* OTHERS PERMISSIONS */

    if( fs.st_mode & S_IROTH )
    {
        sprintf(message,"Others has read permissions on %s: Yes\n",file);
        write(1,message,strlen(message));
    }
    else
    {
        sprintf(message,"Others has read permissions on %s: No\n",file);
        write(1,message,strlen(message));
    }

    if( fs.st_mode & S_IWOTH )
    {
        sprintf(message,"Others has write permissions on %s: Yes\n",file);
        write(1,message,strlen(message));
    }
    else
    {
        sprintf(message,"Others has write permissions on %s: No\n",file);
        write(1,message,strlen(message));
    }

    if( fs.st_mode & S_IXOTH )
    {
        sprintf(message,"Others has execute permissions on %s: Yes\n",file);
        write(1,message,strlen(message));
    }
    else
    {
        sprintf(message,"Others has execute permissions on %s: No\n",file);
        write(1,message,strlen(message));
    }

    char *linebreaks="\n\n";
    write(1,linebreaks,strlen(linebreaks));
}


int main(int argc,char **argv)
{
    char* newfilepath=argv[1];
    char* oldfile=argv[2];
    char* directory=argv[3];

    struct stat fs;

    int oldstat,newstat,dirstat;

    char message[100];
    char *file="oldfile";

    /* Checking if directory has been created */

    if (access(argv[3],F_OK)==0)
    {
        sprintf(message,"Directory is created: Yes\n");
        write(1,message,strlen(message));
    }
    else
    {
        sprintf(message,"Directory is created: No\n");
        write(1,message,strlen(message));
    }

    char *linebreaks="\n\n";
    write(1,linebreaks,strlen(linebreaks));

    int fd,fd2;

    /* Opening oldfile and newfile */

    fd = open (argv[2], O_RDONLY);
    fd2 = open (newfilepath, O_RDONLY);

    if (fd==-1 && fd2==-1)
    {
        perror("Oldfile & Newfile");
        write(1,linebreaks,strlen(linebreaks));
    }
    else if (fd==-1)
    {
        perror("Oldfile");
        write(1,linebreaks,strlen(linebreaks));
    }
    else if(fd2==-1)
    {
        perror("Newfile");
        write(1,linebreaks,strlen(linebreaks));
    }
    else
    {
        /* Checking if newfile is reverse of oldfile */
        off_t filesize_old=lseek(fd,0,SEEK_END);
        off_t filesize_new=lseek(fd2,0,SEEK_END);

        int flag=0;

        if(filesize_new!=filesize_old)
        {
            sprintf(message,"Whether file contents are reversed in newfile: No\n");
            write(1,message,strlen(message));
        }
        else
        {
            char buffer[SIZE];
            char buffer2[SIZE];
            int total_iterations=(filesize_old/SIZE);
            float progress;

            char outputstring[30];
            lseek(fd,0,SEEK_SET);
            lseek(fd2,-(SIZE),SEEK_END);
            for (float n=1;n<=total_iterations;n++)
            {

                progress=(n/total_iterations)*100;

                sprintf(outputstring,"Total progress: %.2f%%\r",progress);
                write(1,outputstring,strlen(outputstring));
                fflush(stdout);


                read(fd,buffer,SIZE);
                read(fd2,buffer2,SIZE);




                for (int j=0;j<SIZE-1;j++)
                {
                    if (buffer[j]!=buffer2[SIZE-1-j])
                    {
                        flag=2;
                        break;
                    }
                }

                if (flag==2)
                {
                    break;
                }

                if(n<total_iterations)
                {
                    lseek(fd2,-(2*SIZE),SEEK_CUR);
                }
                else
                {
                    lseek(fd2,-(SIZE),SEEK_CUR);
                    break;
                }
            }

            if (filesize_old%SIZE!=0)
            {
                lseek(fd,total_iterations*SIZE,SEEK_SET);
                lseek(fd2,-(filesize_new%SIZE),SEEK_CUR);

                memset(buffer, 0, SIZE);
                memset(buffer2, 0, SIZE);

                read(fd,buffer,(filesize_old%SIZE));
                read(fd2,buffer2,(filesize_old%SIZE));

                for (int k=0;k<(filesize_new%SIZE);k++)
                {
                    if (buffer[k]!=buffer2[(filesize_new%SIZE)-1-k])
                    {
                        flag=3;
                        break;
                    }
                    else
                    {
                        continue;
                    }
                }
            };

            if (flag==0)
            {
                sprintf(message,"Whether file contents are reversed in newfile: Yes\n");
                write(1,message,strlen(message)); 
            }
            else
            {
                sprintf(message,"Whether file contents are reversed in newfile: No\n");
                write(1,message,strlen(message)); 
            }    

            write(1,linebreaks,strlen(linebreaks));
        }
    }

    /* Printing file permissions */

    oldstat=stat(oldfile,&fs);
    if (oldstat==-1)
    {
        perror("OLDFILE");
        write(1,linebreaks,strlen(linebreaks));
    }
    else
    {
        displayfilestats(fs,file,message);
    }

    dirstat=stat(directory,&fs);
    if (dirstat==-1)
    {
        perror("DIRECTORY");
        write(1,linebreaks,strlen(linebreaks));
    }
    else
    {
        file="directory";
        displayfilestats(fs,file,message);
    }
    newstat=stat(newfilepath,&fs);
    if (newstat==-1)
    {
        perror("NEWFILE");
        write(1,linebreaks,strlen(linebreaks));
    }
    else
    {
        file="newfile";
        displayfilestats(fs,file,message);
    }
}
