// Shared memory server - client chat implementation made by Ignacio Delmont
// base model taken from
// http://users.cs.cf.ac.uk/Dave.Marshall/C/node27.html#SECTION002731000000000000000

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>

#define SHMSZ     100  // Size in bytes of shared memory

main()
{
    char c;
    int shmid;
    key_t key;
    char *shm, *s;

    // key creation
    key = 5678;

    // segment creation
    // IPC_CREAT tells the system to create a new memory segment for the shared memory
    // 0666 sets the access permissions of the memory segment
    if ((shmid = shmget(key, SHMSZ, IPC_CREAT | 0666)) < 0) {
        perror("shmget");
        exit(1);
    }

    // segment attaching to memory section
    if ((shm = shmat(shmid, NULL, 0)) == (char *) -1) {
        perror("shmat");
        exit(1);
    }

    // fill shared memory with empty string so it's "clean"
    *shm = "";

    // chat loop, the server should start sending messages
    // it end when either side sends the message "bye"
    while (1) {
      s = shm;
      s++; // the first byte of shm is always reserved to identify the sender
           // "-" if it's server and "*" if it's the client

      fflush(stdin); // clean stdin
      fgets(s, SHMSZ - 1, stdin); // get keyboard input
      *shm = '-';
      if (strcmp(s, "bye\n") == 0)
        break;

      // wait until the first character changes to * so the message belongs to the client
      while (*shm != '*')
          sleep(1);

      printf("%s", shm);
      if (strcmp(shm + 1, "bye\n") == 0)
        break;
    }

    exit(0);
}
