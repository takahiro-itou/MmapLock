#if 0
program=MmapLock;
gcc  -O2 -g  -o ${program}  ${program}.cpp
exit 0
#endif


#include    <fcntl.h>
#include    <stdio.h>
#include    <stdlib.h>
#include    <sys/mman.h>
#include    <sys/stat.h>
#include    <unistd.h>


int lockMappedMemoty(
        const char * filepath)
{
    int fd = open(filepath, O_RDONLY);
    if ( fd == -1 ) {
        perror("Error open file");
        return ( 2 );
    }

    struct stat sb;
    if ( fstat(fd, &sb) == -1 ) {
        perror("Error fstat");
        return ( 2 );
    }

    size_t  cbSize  = sb.st_size;
    if ( cbSize == 0 ) {
        fprintf(stderr, "File is empty\n");
        close(fd);
        return ( 2 );
    }

    void * addr = mmap(NULL, cbSize, PROT_READ, MAP_SHARED, fd, 0);
    if ( addr == MAP_FAILED ) {
        perror("Error mmap");
        close(fd);
        return ( 3 );
    }
    if ( mlock(addr, cbSize) != 0 ) {
        perror("Error locking memory.");
        munmap(addr, cbSize);
        close(fd);
        return ( 3 );
    }

    fprintf(stdout,
            "Successfully mapped and locked '%s' (%zu bytes) into memory.\n",
            filepath,  cbSize);
    return ( 0 );
}

int main(int argc, char * argv[])
{
    if ( argc < 2 ) {
        fprintf(stderr, "Usage: %s <filename(s).\n", argv[0]);
        return ( 1 );
    }

    const char * filepath = argv[1];

    lockMappedMemoty(filepath);
    printf("Press Ctrl+C to exit and release the memory.\n");

    for (;;) {
        sleep(3600);
    }

    // munlock(addr, cbSize);
    // munmap (addr, cbSize);
    // close(fd);

    return ( 0 );
}
