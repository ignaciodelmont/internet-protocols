// Shared memory server - client chat implementation made by Ignacio Delmont
// base model taken from
// http://users.cs.cf.ac.uk/Dave.Marshall/C/node27.html#SECTION002731000000000000000

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>

#define SHMSZ     100

main()
{
    int shmid;
    key_t key;
    char *shm, *s;

    // key creation
    key = 5678;

    // segment location
    if ((shmid = shmget(key, SHMSZ, 0666)) < 0) {
        perror("shmget");
        exit(1);
    }

    // segment attaching
    if ((shm = shmat(shmid, NULL, 0)) == (char *) -1) {
        perror("shmat");
        exit(1);
    }

    t_nodo arr = [port, shm]

    // chat loop, the server should start sending messages
    // it end when either side sends the message "bye"
    while (1) {
      s = shm;
      printf("%s", s);
      if (strcmp(s+1, "bye\n") == 0)
        break;

      s = shm + 1;
      fflush(stdin);
      fgets(s, SHMSZ - 1, stdin);
      *shm = '*';
      if (strcmp(s, "bye\n") == 0)
        break;

      while (*shm != '-')
          sleep(1);
    }
    exit(0);
}
