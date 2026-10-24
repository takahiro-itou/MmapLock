#if 0
program=MmapLock;
gcc  -O2 -g  -o ${program}  ${program}.cpp
exit 0
#endif


#include    <fcntl.h>
#include    <stdio.h>
#include    <stdlib.h>
#include    <sys/stat.h>


int main(int argc, char * argv[])
{
    if ( argc < 2 ) {
        fprintf(stderr, "Usage: %s <filename(s).\n", argv[0]);
        return ( 1 );
    }

    const char * filepath = argv[1];
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

    return ( 0 );
}
