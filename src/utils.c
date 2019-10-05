//
// Created by starman on 23.09.2019.
//

#include "include/utils.h"
#include "include/memory.h"
#include <assert.h>
#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdbool.h>

// access rights
const int RWXR_XR_X = S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH; // 755

/**
 * Check path to dir and create directories if it is not exist
 * @param {const char * const} path - path to dir
 * @return {status} - status of function work
 */
status check_or_create_dir_(const char * const path) {
  assert(path != NULL);
  struct stat file_stat;
  int ret_val = stat(path, &file_stat);
  if (ret_val || !S_ISDIR(file_stat.st_mode)) {
    if (mkdir(path, RWXR_XR_X) < 0) {
      return ERROR;
    }
  }
  return OK;
}

/**
 *
 * @param path
 * @return
 */
status check_path_on_current_(const char * const path) {
  const int BUF_SIZE = 1024;
  char dir_buff[BUF_SIZE];
  char *current_dir = getcwd(dir_buff, BUF_SIZE);
  if (!current_dir) {
    return ERROR;
  }
  chdir(path);
  char user_dir[BUF_SIZE];
  char *user_ptr = getcwd(user_dir, BUF_SIZE);
  chdir(current_dir);
  if (!user_ptr) {
    return ERROR;
  }
  if (strcmp(current_dir, user_ptr) == 0) {
    return IS_CURRENT_DIR;
  }
  return OK;
}

status check_or_create_path(char *path) {
  assert(path != NULL);
  char *cur_path = (char *)new(strlen(path));
  if (!cur_path) {
    return ERROR;
  }

  const char delimiter = '/';
  char *path_copy = path;
  char *token = strchr(path_copy, delimiter);
  while (token != NULL) {
    if (strlen(token)) {
      strncpy(cur_path, path, token - path);
      cur_path[token - path] = '\0';
      if (check_or_create_dir_(cur_path)) {
        delete(cur_path);
        return ERROR;
      }
      path_copy = ++token;
      token = strchr(path_copy, delimiter);
    }
  }

  if (check_or_create_dir_(path)) {
    delete(cur_path);
    return ERROR;
  }
  delete(cur_path);
  status result = check_path_on_current_(path);
  return result;
}

status copy_file(const char *path_to, const char *file) {
  assert(path_to != NULL && file != NULL);
  struct stat file_stat = {};
  int ret_val = stat(file, &file_stat);
  if (!ret_val && S_ISREG(file_stat.st_mode)) {
    int fd_in = open(file, O_RDONLY);
    if (fd_in < 0) {
      return ERROR;
    }

    char *new_path = (char *)new(strlen(path_to) + strlen(file) + 2);
    if (!new_path) {
      return ERROR;
    }
    sprintf(new_path, "%s/%s", path_to, file);

    int fd_out = open(new_path, O_CREAT | O_RDWR | O_TRUNC, 0644);
    if (fd_out < 0) {
      close(fd_in);
      delete(new_path);
      return ERROR;
    }

    bool error = false;
    if (lseek(fd_out, file_stat.st_size - 1, SEEK_SET) < 0) {
      error = true;
      goto err_handler;
    }

    if (write(fd_out, "", 1) != 1) {
      error = true;
      goto err_handler;
    }

    void *src = mmap(0, file_stat.st_size, PROT_READ, MAP_SHARED, fd_in, 0);
    if (src == MAP_FAILED) {
      error = true;
      goto err_handler;
    }

    void *dst = mmap(0, file_stat.st_size, PROT_READ | PROT_WRITE, MAP_SHARED,
                     fd_out, 0);
    if (dst == MAP_FAILED) {
      error = true;
      goto err_handler;
    }

    memcpy(dst, src, file_stat.st_size);

  err_handler:
    close(fd_in);
    close(fd_out);
    delete(new_path);
    return error ? ERROR : OK;
  }
  return IS_DIR;
}

status copy_files(char *path_from, char *path_to) {
  assert(path_from != NULL && path_to != NULL);

  int access_status = access(path_to, R_OK);
  if (access_status < 0) {
    perror("Directory is not readable (access denied)\n");
    return PERMISSION_DENIED;
  }

  DIR *dir_from = opendir(path_from);
  if (dir_from) {
    struct dirent *dir = readdir(dir_from);
    do {
      printf("%d-%lu-%d %s\n", dir->d_type, dir->d_ino, dir->d_reclen,
             dir->d_name);
      if (dir->d_type == DT_REG) {
        access_status = access(dir->d_name, R_OK);
        if (access_status < 0) {
          fprintf(stderr, "File is not readable (access denied): %s\n",
                  dir->d_name);
          //          return PERMISSION_DENIED;
        }

        status result = copy_file(path_to, dir->d_name);
        if (result) {
          return result;
        }
      }
      dir = readdir(dir_from);
    } while (dir != NULL);
  }
  return OK;
}
