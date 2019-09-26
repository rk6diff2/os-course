//
// Created by starman on 23.09.2019.
//

#include "include/utils.h"
#include <assert.h>
#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/mman.h>

const int RWXR_XR_X = S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH; // 755

status check_or_create_dir_(char * path)  {
  struct stat file_stat;
  int ret_val = stat(path, &file_stat);
  if (ret_val || !S_ISDIR(file_stat.st_mode)) {
    if (mkdir(path, RWXR_XR_X) < 0) {
      return ERROR;
    }
  }
  return OK;
}

status check_or_create_path(char* path) {
  assert(path != NULL);
  char *path_copy = path;
  char delimiter = '/';
  char *token = NULL;
  char *cur_path = (char *)malloc(strlen(path));
  if (!cur_path) {
    return ERROR;
  }

  while ((token = strchr(path_copy, delimiter)) != NULL) {
    if (strlen(token)) {
      strncpy(cur_path, path, token - path);
      cur_path[token - path] = '\0';
      if (check_or_create_dir_(cur_path)) {
        free(cur_path);
        return ERROR;
      }
      path_copy = ++token;
    }
  }

  if (check_or_create_dir_(path)) {
    free(cur_path);
    return ERROR;
  }

  free(cur_path);
  return OK;
}

status copy_file(const char *path_to, const char *file) {
  assert(path_to != NULL);

  struct stat file_stat = {};
  int ret_val = stat(file, &file_stat);
  if (!ret_val && S_ISREG(file_stat.st_mode)) {
    int fd_in = open(file, O_RDONLY);
    if (fd_in < 0) {
      return ERROR;
    }
    if (fstat(fd_in, &file_stat) < 0) {
      close(fd_in);
      return ERROR;
    }
    char *new_path = (char *)malloc(strlen(path_to) + strlen(file) + 2);
    if (!new_path) {
      return ERROR;
    }

    sprintf(new_path, "%s/%s", path_to, file);
    int fd_out = open(new_path, O_CREAT | O_RDWR | O_TRUNC, 0644);
    if (fd_out < 0) {
      close(fd_in);
      free(new_path);
      return ERROR;
    }

    if (lseek(fd_out, file_stat.st_size - 1, SEEK_SET) < 0) {
      close(fd_in);
      close(fd_out);
      free(new_path);
      return ERROR;
    }

    if (write(fd_out, "", 1) != 1) {
      close(fd_in);
      close(fd_out);
      free(new_path);
      return ERROR;
    }

    void *src = mmap(
        0,
        file_stat.st_size,
        PROT_READ,
        MAP_SHARED,
        fd_in,
        0);
    if (src == MAP_FAILED) {
      close(fd_in);
      close(fd_out);
      free(new_path);
      return ERROR;
    }

    void *dst = mmap(0,
        file_stat.st_size,
        PROT_READ | PROT_WRITE, MAP_SHARED,
        fd_out,
        0);
    if (dst == MAP_FAILED) {
      close(fd_in);
      close(fd_out);
      free(new_path);
      return ERROR;
    }

    memcpy(dst, src, file_stat.st_size);
    close(fd_in);
    close(fd_out);
    free(new_path);
    return OK;
  }
  return IS_DIR;
}

status copy_files(char *path_from, char *path_to) {
  int access_status = access(path_to, R_OK);
  if (access_status < 0) {
    perror("Directory is not readable (access denied)\n");
    return PERMISSION_DENIED;
  }

  const int BUF_SIZE = 1024;
  char dir_buff[BUF_SIZE];
  char *current_dir = getcwd(dir_buff, BUF_SIZE);
  if (!current_dir) {
    return ERROR;
  }

  DIR *dir_from = opendir(path_from);
  if (dir_from) {
    struct dirent *dir = readdir(dir_from);
    do {
      printf("%d-%lu-%d %s\n", dir->d_type, dir->d_ino, dir->d_reclen, dir->d_name);
      if (dir->d_type == DT_REG) {
        access_status = access(dir->d_name, R_OK);
        if (access_status < 0) {
          printf ("File is not readable (access denied): %s\n", dir->d_name);
//          return PERMISSION_DENIED;
        }

        status result = copy_file(path_to, dir->d_name);
        if (result) {
          return result;
        }
      }
    } while ((dir = readdir(dir_from)) != NULL);
  }
  return OK;
}
