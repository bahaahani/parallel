#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main()
{
    const int SIZE = 4096;   // Size of the shared memory
    const char *name = "OS"; // Name of the shared memory object
    const char *message_0 = "Hello, ";
    const char *message_1 = "Shared Memory!\n";

    int shm_fd; // Shared memory file descriptor
    void *ptr;  // Pointer to shared memory object

    // Create the shared memory object
    shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1)
    {
        perror("Error creating shared memory");
        return EXIT_FAILURE;
    }

    // Configure the size of the shared memory object
    ftruncate(shm_fd, SIZE);

    // Memory map the shared memory object
    ptr = mmap(0, SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (ptr == MAP_FAILED)
    {
        perror("Error mapping shared memory");
        return EXIT_FAILURE;
    }

    // Write to the shared memory object
    sprintf(ptr, "%s", message_0);
    ptr += strlen(message_0);
    sprintf(ptr, "%s", message_1);

    // Now read from the shared memory
    printf("%s", (char *)mmap(0, SIZE, PROT_READ, MAP_SHARED, shm_fd, 0));

    // Remove the shared memory object
    if (shm_unlink(name) == -1)
    {
        perror("Error removing shared memory");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
