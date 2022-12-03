#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#define SIZE 20000 /* Buffer size */


int main(int argc,char **argv)
{
    /* Open/Create source and destination files along with directory */

    int fd,fd2,n=1; 
    fd = open (argv[1], O_RDONLY);

    if (fd==-1)
    {
        perror("Error");
        return 1;
    }

    mkdir("Assignment",S_IRWXU);

    char newfilepath[1000];

    sprintf(newfilepath,"Assignment/1_%s",argv[1]);

    fd2 = open(newfilepath, O_CREAT|O_RDWR, S_IRUSR | S_IWUSR);

    /* Obtaining file size and calculating number of iterations */

    off_t filesize=lseek(fd,0,SEEK_END);

    char buffer[SIZE];

    int total_iterations=(filesize/SIZE);
    float progress;

    char outputstring[30];

    if(total_iterations==0)
    {
        sprintf(outputstring,"\rTotal progress: 100.00%%");
        write(1,outputstring,strlen(outputstring));
        fflush(stdout);
    }

    lseek(fd,-SIZE,SEEK_END);

    /* Reversing file chunk-by-chunk from the end of the file using loop with iterations obtained */

    for (float n=1;n<=total_iterations;n++)
    {

        progress=(n/total_iterations)*100;

        sprintf(outputstring,"\rTotal progress: %.2f%%",progress);
        write(1,outputstring,strlen(outputstring));
        fflush(stdout); //Progress Indicator


        read(fd,buffer,SIZE);

        int reversecount=0;
        char swap;

        while (reversecount<(SIZE/2))
        {
            swap=buffer[reversecount];
            buffer[reversecount]=buffer[(SIZE-1)-reversecount];
            buffer[(SIZE-1)-reversecount]=swap;
            reversecount++;
        }

        write(fd2,buffer,SIZE);

        if(n<total_iterations)
        {
            lseek(fd,-(2*SIZE),SEEK_CUR);
        }
        else
        {
            lseek(fd,-(SIZE),SEEK_CUR);
            break;
        }
    }

    /* One last iteration if filesize % chunksize != 0 */

        if (filesize%SIZE!=0)
        {
            lseek(fd,-(filesize%SIZE),SEEK_CUR);

            memset(buffer, 0, SIZE);

            read(fd,buffer,(filesize%SIZE));

            int reversecount=0;
            char swap;

            int k= (strlen(buffer))/2;

            while (reversecount<k)
            {
                swap=buffer[reversecount];
                buffer[reversecount]=buffer[(strlen(buffer)-1)-reversecount];
                buffer[(strlen(buffer)-1)-reversecount]=swap;
                reversecount++;
            }

            write(fd2,buffer,strlen(buffer));
        }
    return 0;
}