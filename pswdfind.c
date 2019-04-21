#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdint.h>
#include<unistd.h>

char *filestrings(const char *filename)
{
    FILE *fp;
    char *fstr;
    int fsize;
    int i;
    fp = fopen(filename,"r");
    if(fp == NULL){
        fprintf(stderr,"file input error\n");
        exit(EXIT_FAILURE);
    }
    fseek(fp,0,SEEK_END);
    fsize = ftell(fp);
    fclose(fp);
    fp = fopen(filename,"r");
    fstr = (char *)malloc(sizeof(char)*fsize);
    for(i = 0; i < fsize; i++){ 
        fstr[i] = fgetc(fp);
    }
    fclose(fp);

    return fstr;
}

void filewrite(const char *filename,const char *fstring)
{
    FILE *fp;
    
    fp = fopen(filename,"w");
    if(fp == NULL){
        fprintf(stderr,"file output error\n");
        exit(EXIT_FAILURE);
    }
    fputs(fstring,fp);
    fclose(fp);
}

typedef struct _table
{
    int indx;
    char **str;
}table_t;

table_t strsplit(char *str,char *tgt)
{
    char *sbuff, *fbuff, **rstr;
    int indx;
    table_t t;
    indx = 0;
    fbuff = strdup(str);
    rstr = (char**)malloc(sizeof(char*));
    while( ( sbuff = strsep(&fbuff,tgt) ) != NULL ){
        rstr[indx] = sbuff;
        indx++;
        rstr = (char**)realloc(rstr,sizeof(char*)*(indx+1));
    }
    t.indx = indx-1;
    t.str = rstr;
    return t;
}



char **dictstrcat(char **ref, char **dict,  
        int reflen, int dictlen, int depth, int max)
{
    int rlen,dlen,catlen;
    int match;
    int i,j;
    int loop;
    char **buff,**rbuff;
    size_t buffsize;

    buffsize = (size_t)dictlen*sizeof(char*);
    match = 0;
    loop = 0;
    buff = (char**)malloc(buffsize);
 
    if(buff == NULL){
        fprintf(stderr,"buffer could not allocated\n");
        return NULL;
    }

    for(i = 0; i < reflen; ++i){
        rlen = strlen(ref[i]);

        for(j = 0; j < dictlen; ++j){
        
            //fprintf(stderr,"  \"\e[93m%s\e[0m\" << \"\e[93m%s\e[0m\" ",ref[i],dict[j]);
        
            dlen = strlen(dict[j]);
            catlen = rlen + dlen;
            
            if(catlen <= max){

                buff[match] = (char*)malloc(sizeof(char)*(catlen));
                strcpy(buff[match],ref[i]);
                strcat(buff[match],dict[j]);
                //fprintf(stderr,"\runder max   \e[92m%s\e[0m <-- (%ld/%d)",buff[match],strlen(buff[match]),max);
                ++match;
                buff = (char**)realloc(buff,sizeof(char*)*(match+1)); 
            }
            ++loop;
        }  
    }

    if(match == 0){
        fprintf(stderr,"--==\e[96m FINISHED \e[0m==--\n");
        fprintf(stderr,"loops - \e[95m%d\e[0m\n",loop);
        fprintf(stderr,"depth - \e[95m%d\e[0m\n",depth); 

        return NULL;
    }else{
        fprintf(stderr,"(depth)\e[92m %d \e[0m",depth);
        fprintf(stderr,": (match)\e[93m %d \e[0m\n",match);

    }

    ++depth;
    buff = dictstrcat(
            buff,dict,
            match,dictlen,
            depth,
            max);
}

void getargs(int argc, char **argv)
{
    char **dcatstr,**dict,**res,**ref,*fstr;
    char *dir;
    int len,max,i;
    table_t t; 
    
    
    if(argv[1] == NULL || argv[2] == NULL){
        fprintf(stderr,"too short argument\n");
        if(argv[2] == NULL)
            fprintf(stderr,"max length not selected\n");
        exit(1);
    }
    if (argc > 3)
        fprintf(stderr,"too many arguments\n");

    dir = argv[1];
    max = atoi(argv[2]);
    
    
    fstr = filestrings(dir);
    t = strsplit(fstr,"\n");
    dict = t.str;
    ref = t.str;  
    len = t.indx;

    dcatstr = dictstrcat(
            ref,
            dict,
            len,
            len,
            0,
            max);
    
    fprintf(stderr,"\n");
}

int main(int argc, char **argv)
{
    getargs(argc,argv);
}
