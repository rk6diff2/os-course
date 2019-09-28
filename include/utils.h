//
// Created by starman on 23.09.2019.
//

#ifndef LAB2_UTILS_H_
#define LAB2_UTILS_H_

typedef enum {
  NOT_FOUND = -4,
  PERMISSION_DENIED,
  UNKNOWN_FLAG,
  ERROR,
  OK,
  IS_DIR,
  IS_FILE,
} status;

/**
 * Copying files from path_form dir to path_to dir
 * @param {char*} path_from - path to departure dir
 * @param {char*} path_to - path to destination dir
 * @return {status} stat
 */
status copy_files(char *path_from, char *path_to);

/**
 * Check is path exist and if it not then create it
 * @param {char*} path - path to dir
 * @return {status} stat
 */
status check_or_create_path(char *path);

/**
 * Copy file from current dir to path_to dir
 * @param {char*} path_to - the path to destination dir
 * @param {char*} file - the file that you need to copy
 * @return {status} stat
 */
status copy_file(const char *path_to, const char *file);

#endif // LAB2_UTILS_H_
