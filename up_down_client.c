#include "up_down.h"

char *get_file_name_from_path(char *filepath, char *delimiter) {
  char *token = strtok(filepath, delimiter);
  char *actual_file = NULL;
  while (token != NULL) {
    actual_file = token;
    token = strtok(NULL, delimiter);
  }
  return actual_file;
}

void updown_1(char *host, char *command, char *filepath) {
  CLIENT *clnt;
  struct chunk *result_1;
  chunk ud_1_arg;

#ifndef DEBUG
  clnt = clnt_create(host, UPDOWN, UPDOWN_1, "udp");
  if (clnt == NULL) {
    clnt_pcreateerror(host);
    exit(1);
  }
#endif /* DEBUG */

  FILE *fp;

  if (strcmp(command, "-upload") == 0) {
    ud_1_arg.flag = 1;
    fp = fopen(filepath, "rb");
    if (fp == NULL) {
      printf("Sorry the file cannot be opened");
      exit(1);
    }
    char *actual_file = get_file_name_from_path(filepath, "/");
    strcpy(ud_1_arg.dirname, actual_file);
    while (1) {
      memset(ud_1_arg.sendBuff, 0, sizeof(ud_1_arg.sendBuff));
      int nread = fread(ud_1_arg.sendBuff, 1, 2049, fp);
      if (nread > 0) {
        result_1 = ud_1(&ud_1_arg, clnt);
      }
      if (nread < 2049) {
        if (ferror(fp)) printf("Error reading\n");
        break;
      }
    }
    fclose(fp);
  } else if (strcmp(command, "-download") == 0) {
    ud_1_arg.flag = 2;
    strcpy(ud_1_arg.dirname, filepath);
    char *actual_file = get_file_name_from_path(filepath, "/");
    fp = fopen(actual_file, "ab+");
    if (fp == NULL) {
      printf("Sorry the file cannot be opened");
      exit(1);
    }
    do {
      result_1 = ud_1(&ud_1_arg, clnt);
      fwrite(result_1->sendBuff, 1, strlen(result_1->sendBuff), fp);
    } while (result_1->flag != 4);
    fclose(fp);
  } else if (strcmp(command, "-getdir") == 0) {
    result_1 = ud_1(&ud_1_arg, clnt);
    char *token = strtok(result_1->dirname, "--");
    while (token != NULL) {
      printf(" %s\n", token);
      token = strtok(NULL, "--");
    }
  } else {
    printf("Enter a valid command !!!");
  }

  if (result_1 == (struct chunk *)NULL) {
    clnt_perror(clnt, "call failed");
  }

#ifndef DEBUG
  clnt_destroy(clnt);
#endif /* DEBUG */
}

int main(int argc, char *argv[]) {
  char *host, *command, *path = NULL;

  if (argc < 3) {
    printf("Usage: %s server_host -getdir\n%s server_host -upload | -download path_of_the_file\n", argv[0], argv[0]);
    exit(1);
  }

  host = argv[1];
  command = argv[2];

  if (argc > 3) path = argv[3];

  updown_1(host, command, path);
  exit(0);
}