#include <sys/types.h>
#include <dirent.h>
#include "up_down.h"

struct chunk *ud_1_svc(chunk *argp, struct svc_req *rqstp) {
  static struct chunk result;

  if (argp->flag == 1) {
    // Get the data, dump it to a file
    FILE *fp;
    char *filename = argp->dirname;
    char prefix[10] = "uploaded_";
    strcat(prefix, filename);
    fp = fopen(prefix, "ab+");
    
    if (fp == NULL) {
      printf("Sorry, the file cannot be opened");
      exit(1);
    }
    
    fwrite(argp->sendBuff, 1, strlen(argp->sendBuff), fp);
    fclose(fp);
  } else if (argp->flag == 2) {
    // Get a request to send the file
    FILE *fp;
    fp = fopen(argp->dirname, "rb+");
    
    if (fp == NULL) {
      printf("Sorry, the file cannot be opened");
      exit(1);
    }
    
    memset(argp->sendBuff, 0, sizeof(argp->sendBuff));
    int nread = fread(argp->sendBuff, 1, 1025, fp);
    result = *argp;

    if (nread > 0 && nread == 1025) {
      // If you read successfully, then send this data
      return &result;
    } else if (ferror(fp)) {
      printf("Error reading\n");
    } else {
      result.flag = 4;
      return &result;
    }
    
    fclose(fp);
} else {
    DIR *dp;
    struct dirent *ep;
    dp = opendir("./");

    if (dp != NULL) {
      while (ep = readdir(dp)) {
        char fname[50];
        memset(fname, 0, sizeof(fname));
        strcpy(fname, ep->d_name);
        strcat(fname, "--");
        strcat(result.dirname, fname);
      }
    } else {
      perror("Couldn't open the directory");
    }
  }
  return &result;
}
