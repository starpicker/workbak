#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>

// gcc crcf_tab_convert.c -o crcf_tab_convert
//

void replace_string(char *result, char *source, char* s1, char *s2)
{
    char *q=NULL;
    char *p=NULL;
   
    p=source;
    while((q=strstr(p, s1))!=NULL)
    {
        strncpy(result, p, q-p);
        result[q-p]= '\0';//very important, must attention!
        strcat(result, s2);
        strcat(result, q+strlen(s1));
        strcpy(p,result);
    }
    strcpy(result, p);    
}

int main(int argc, char** argv)
{
    const char* filename = argv[1];

    struct stat st;
    stat(filename, &st);
    int size = st.st_size;
    
    char* buff = (char*)malloc(size+1);
    char* buff_result1 = (char*)malloc(2*size+1);
    char* buff_result2 = (char*)malloc(4*size+1);
    FILE* fp = fopen(filename, "r");
    FILE* fp2 = 0;
        
    fread(buff, size, 1, fp);
    buff[size] = '\0';
    
    if(strstr(buff, "\r\n") || strstr(buff, "\t"))
    {
        printf("%s is dos format or has tab\n", filename);
    }
    else
    {
        goto EXIT;
    }
    
#define TEMPNAME "tempXYZ123"
    mkdir(TEMPNAME, S_IRWXU);

    replace_string(buff_result1, buff, "\r\n", "\r");
    replace_string(buff_result2, buff_result1, "\t", "    ");

    char* p = strstr(filename, "/");
    char name[256];
    char result_path[256];
    if(p)
    {
        sprintf(result_path, "%s%s", TEMPNAME, p);
        strcpy(name, p+1);
        char* start = p;
        
        while(p = strstr(p+1, "/"))
        {
            char cmd[256];
            char temp =  name[p-start];
            name[p-start] = '\0';
            sprintf(cmd, "mkdir -p %s/%s", TEMPNAME, name);
            system(cmd);
            name[p-start] = temp;
            //printf("cmd is %s\n", cmd);
        }
    }
    else
    {
        sprintf(result_path, "%s/%s", TEMPNAME, filename);
    }
   
    printf("saved to result_path: %s\n", result_path);
    
    fp2 = fopen(result_path, "w");
    fwrite(buff_result2, strlen(buff_result2), 1, fp2);

EXIT: 
    fclose(fp);
    if(fp2)fclose(fp2);
    free(buff);
    free(buff_result1);
    free(buff_result2);
}

