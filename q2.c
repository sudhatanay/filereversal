#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#define SIZE 20000 /* Buffer size */

long convert(char* s)
{
    /* Function to convert index agruments(strings) into numbers */
    off_t num = 0;
    long long length= strlen(s);

    for (int j=0;j<length;j++)
    {
        num = (num*10)+ (int)s[j] - 48;
    }

    return num;
}

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

    sprintf(newfilepath,"Assignment/2_%s",argv[1]);

    fd2 = open(newfilepath, O_CREAT|O_RDWR, S_IRUSR | S_IWUSR);

    off_t filesize=lseek(fd,0,SEEK_END);

    /* Obtaining file size and calculating number of iterations along with indices */

    char *a=argv[2];
    char *b=argv[3];

    off_t startindex=convert(a);
    off_t endindex=convert(b);

    off_t end_size=filesize-1-endindex;
    off_t mid_size=filesize-end_size-startindex;
    off_t first_size=filesize-(end_size+mid_size);

    char buffer[SIZE];

    int total_first_iterations=(first_size/SIZE);
    int total_mid_iterations=(mid_size/SIZE);
    int total_end_iterations=(end_size/SIZE);

    int total_iterations=total_end_iterations+total_first_iterations+total_mid_iterations;

    float progress;

    char outputstring[30];


    /* START OF STRING */


    lseek(fd,startindex,SEEK_SET);
    lseek(fd,-SIZE,SEEK_CUR);

    /* Reversing file chunk-by-chunk from the end of the file using loop with iterations obtained */

    for (float n=1;n<=total_first_iterations;n++)
    {

        progress=(n/total_iterations)*100;

    

        sprintf(outputstring,"\rTotal progress: %.2f%%",progress);
        write(1,outputstring,strlen(outputstring));
        fflush(stdout); //Progress Indicator

        size_t a=read(fd,buffer,SIZE);

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

    /* One last iteration if partsize % chunksize != 0 */

    if (first_size%SIZE!=0)
    {
        lseek(fd,-(first_size%SIZE),SEEK_CUR);

        memset(buffer, 0, SIZE);

        size_t a=read(fd,buffer,(first_size%SIZE));

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


    /* MIDDLE OF STRING */


    lseek(fd,startindex,SEEK_SET);

    /* Writing middle part between indices as-is into output file */

    for (float n=1;n<=total_mid_iterations;n++)
    {

        progress=((n+total_first_iterations)/total_iterations)*100;

    
        sprintf(outputstring,"\rTotal progress: %.2f%%",progress);
        write(1,outputstring,strlen(outputstring));
        fflush(stdout); //Progress Indicator

        size_t a=read(fd,buffer,SIZE);

        write(fd2,buffer,SIZE);
        lseek(fd,(n*SIZE),SEEK_CUR);
    }

    /* One last iteration if partsize % chunksize != 0 */

    if (mid_size%SIZE!=0)
    {
        lseek(fd,first_size+(total_mid_iterations*SIZE),SEEK_SET);

        memset(buffer, 0, SIZE);

        size_t a=read(fd,buffer,(mid_size%SIZE));

        write(fd2,buffer,strlen(buffer));
    }


    /* END OF STRING */


    lseek(fd,-SIZE,SEEK_END);

    /* Reversing file chunk-by-chunk from the end of the file using loop with iterations obtained */

    for (float n=1;n<=total_end_iterations;n++)
    {

        progress=((n+total_first_iterations+total_mid_iterations)/total_iterations)*100;

        sprintf(outputstring,"\rTotal progress: %.2f%%",progress);
        write(1,outputstring,strlen(outputstring));
        fflush(stdout); //Progress Indicator

        size_t a=read(fd,buffer,SIZE);

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

    /* One last iteration if partsize % chunksize != 0 */

    if (end_size%SIZE!=0)
    {
        lseek(fd,-(end_size%SIZE),SEEK_CUR);

        memset(buffer, 0, SIZE);

        size_t a=read(fd,buffer,(end_size%SIZE));

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