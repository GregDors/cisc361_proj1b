#include "mycp.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define BUFFERSIZE 1
#define MODE 0666

int checkOverwrite(char *fd);
int read_write(char *r, char *w);

extern int errno;

//void or int
int main (int argc, char *argv[]){
  int file1_exist, file2_exist;
  file1_exist = access(argv[1], F_OK);
  file2_exist = access(argv[2], F_OK);
  if (argc !=3){
    printf("wrong number of arguments used. Exiting mycp \n");
    return 0;
    //exit(0);
    //or should I use exit(0);
  }
  if(file1_exist != 0){
    printf("The first file does not exist");
    exit(1);
  }
  if((file1_exist == 0) && (file2_exist == 0)){
    if(checkOverwrite(argv[2]) == 1){
      read_write(argv[1],argv[2]);
      return 0;
    }
    else{
      printf("User has chosen not to overwrite the file: %s. Exiting mycp", argv[2]);
      return 0;
    }
  }
  else if(file1_exist == 0){
    read_write(argv[1],argv[2]);
    return 0;
  }

}

int checkOverwrite(char *fd){
  int overwrite;
  printf("Do you wish to overwrite the file: %s? (1)-Yes or (2)-No: ", fd);
  scanf("%d", &overwrite);
  if(overwrite == 1){
    return 1;
  }
  else{
    return 0;
  }
}

int read_write(char *r, char *w){
  /*
or - open the file to be read from
ow - open the file to be written into
ri - read in the first character of the file to be read from
wi - write in the first character of the file to be read from
to the file to be written into
  */
//umask(0);
//int x = stat(r, buffer);
  int or, ow, ri, wi;
int n;
struct stat buf;
  char *buffer = (char *)malloc(sizeof(char) * BUFFERSIZE);
  int x = stat(r,&buf);
//where do I use  buf.stmode
  or = open(r, O_RDONLY);
  if(or < 0){
    perror("Cannot open/read file");
    return 0;
  }
  if(x < 0){
    perror("stat error");
    return 0;
  }
  //S_IRUSR, S_IWUSR, S_IRGRP, S_IROTH used for file permissions
  ow = open(w, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH,buf.st_mode);
  if (ow < 0){
    perror("Cannot open/write file");
  }
  ri = read(or, buffer, BUFFERSIZE);
  if(ri < 0){
    perror("Cannot read in first character");
    return 0;
  }else{
    wi = write(ow, buffer, BUFFERSIZE);
    if(wi < 0){
      perror("Cannot write the first character");
      return 0;
    }else{
      while ((n = read(or, buffer, 1)) > 0){
        if((wi = write(ow, buffer, n)) != n){
          printf("write error");
        }
      }
    if (n < 0){
      printf("read error");
    }
    }
  }
  printf("Copying finished\n");
  free(buffer);
  close(or);
  close(ow);
  return 1;
}

