#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>
#include "defs.h"
#include "clog.h"

FILE *fp = NULL;

int main(){
  const char* fileName = "clog.txt";
  fp = fopen(fileName,"a");
  if(fp == NULL){
    //error opening file
    return -1;
  }


  int answer2[4] = {3,11,12,13};
  int answer[1] = {-1};
  int answer4[7] = {6,1,2,3,4,5,6};
  int answer3[1] = {-5};
  writeinLog(answer);
  writeinLog(answer2);
  writeinLog(answer3);
  writeinLog(answer4);
  return 0;
}

void writeinLog(int answer[]) {
  if(answer[0] > 0)
  {
    int i;
    int n_seats = answer[0];
    for(i=1;i<=n_seats;i++){
      fprintf(fp,"%.5d %.2d.%.2d \n",getpid(),answer[i],n_seats);
      fflush(fp);
    }

  }
  else {
    int errorNo = answer[0];
    switch(errorNo){
      case MAX:
      writeError("MAX");
      break;

      case NST:
      writeError("NST");;
      break;

      case IID:
      writeError("IID");
      break;

      case ERR:
      writeError("ERR");
      break;

      case NAV:
      writeError("NAV");
      break;

      case FUL:
      writeError("FUL");
      break;


    }
  }
}

void writeError(char err[]){
  fprintf(fp,"%.5d %.3s \n",getpid(),err);
  fflush(fp);
}
