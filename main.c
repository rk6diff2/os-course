// В заданном текстовом файле необходимо подсчитать количество всех различных
// слов из латинских букв, которые содержат указанное сочетание букв.
//
/* Popular file words */

#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  int fdp1[2];
  int fdp2[2];
  int fdp3[2];

  pipe(fdp1);
  pipe(fdp2);
  pipe(fdp3);

  if (argc < 3) {
    write(STDOUT_FILENO, "Usage: popular file-name pattern\n", 38);
    exit(0);
  }

  if (fork() == 0) {
    close(1);
    dup(fdp1[1]);
    close(fdp1[1]);
    close(fdp1[0]);
    execl("/bin/cat", "cat", argv[1], (char *)0);
    exit(1);
  }

  if (fork() == 0) {
    close(0);
    dup(fdp1[0]);
    close(fdp1[0]);
    close(fdp1[1]);
    close(1);
    dup(fdp2[1]);
    close(fdp2[1]);
    execl("/usr/bin/tr", "tr", "-cs", "A-Za-z", "\n", (char *)0);
    exit(2);
  }

  if (fork() == 0) {
    close(0);
    dup(fdp2[0]);
//    close(fdp2[0]);
    close(fdp2[1]);
    close(fdp1[0]);
    close(fdp1[1]);
    execl("/bin/grep", "grep", "-c", argv[2], (char *)0);
    exit(3);
  }

  close(fdp1[0]);
  close(fdp1[1]);
  close(fdp2[0]);
  close(fdp2[1]);
  close(fdp3[0]);
  close(fdp3[1]);
  while (wait(0) != (-1))
    ;
  exit(0);
}
