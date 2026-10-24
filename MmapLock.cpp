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


typedef  struct  {
    const  char  *  path;
    int         fd;
    size_t      size;
    void  *     addr;
}  MappingInfo;


int
lockMappedMemoty(
        const  char  *  fileName,
        MappingInfo  *  mapInfo)
{
    MappingInfo  mi;

    mi.path = fileName;
    mi.fd   = open(fileName, O_RDONLY);
    if ( mi.fd == -1 ) {
        perror("Error open file");
        return ( 2 );
    }

    struct stat sb;
    if ( fstat(mi.fd, &sb) == -1 ) {
        perror("Error fstat");
        return ( 2 );
    }

    mi.size = sb.st_size;
    if ( mi.size == 0 ) {
        fprintf(stderr, "File is empty\n");
        close(mi.fd);
        return ( 2 );
    }

    mi.addr = mmap(NULL, mi.size, PROT_READ, MAP_SHARED, mi.fd, 0);
    if ( mi.addr == MAP_FAILED ) {
        perror("Error mmap");
        close(mi.fd);
        return ( 3 );
    }
    if ( mlock(mi.addr, mi.size) != 0 ) {
        perror("Error locking memory.");
        munmap(mi.addr, mi.size);
        close(mi.fd);
        return ( 3 );
    }

    fprintf(stdout,
            "Successfully mapped and locked '%s' (%zu bytes) into memory.\n",
            mi.path, mi.size);

    mapInfo->path   = mi.path;
    mapInfo->fd     = mi.fd;
    mapInfo->size   = mi.size;
    mapInfo->addr   = mi.addr;

    return ( 0 );
}

int main(int argc, char * argv[])
{
    if ( argc < 2 ) {
        fprintf(stderr, "Usage: %s <filename(s).\n", argv[0]);
        return ( 1 );
    }

    const char * filepath = argv[1];
    MappingInfo maps[2];

    lockMappedMemoty(filepath, maps + 1);
    printf("Press Ctrl+C to exit and release the memory.\n");

    for (;;) {
        sleep(3600);
    }

    // munlock(addr, cbSize);
    // munmap (addr, cbSize);
    // close(fd);

    return ( 0 );
}
