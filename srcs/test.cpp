#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>

int main()
{
    struct stat	sb;

    stat("test.txt", &sb);
    if (errno == ENOENT)
        std::cout << "존재 x" << std::endl;
    else
    {
        std::cout << "존재" << std::endl;
        if (S_ISDIR(sb.st_mode))
					std::cout << "diretory" << std::endl;
        std::cout << sb.st_size << std::endl;
    }
    int fd = open("test.txt", O_RDONLY);
    char buf[sb.st_size + 1];
    int k = read(fd, buf, sizeof(buf));
    buf[k] = 0;
    std::cout << k << std::endl;
    std::cout << buf << std::endl;
}
