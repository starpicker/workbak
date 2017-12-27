#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>

// gcc crcf_tab_check.c -o crcf_tab_check
//
int main(int argc, char** argv)
{
    const char* filename = argv[1];

    struct stat st;
    stat(filename, &st);
    int size = st.st_size;
    char* buff = (char*)malloc(size+1);
    FILE* fp = fopen(filename, "r");

    fread(buff, size, 1, fp);
    buff[size] = '\0';
    
    if(strstr(buff, "\r\n") || strstr(buff, "\t"))
    {
        printf("%s is dos format or has tab\n", filename);
    }

    fclose(fp);
    free(buff);
    
    return 0;
}

