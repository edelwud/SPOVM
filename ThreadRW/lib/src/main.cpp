#include <aio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <iostream>

using namespace std;

extern "C" bool ReadFileAsync(char path[BUFSIZ], char buffer[BUFSIZ]) {
    int fd = open(path, O_RDWR, S_IRUSR | S_IWUSR);
    if (fd == 0) {
        cout << "Can't open file" << endl;
        return false;
    }

    aiocb callback{ fd, LIO_READ, 0, buffer, BUFSIZ };
    if (aio_read(&callback) == -1) {
        close(fd);
        cout << "Can't read file" << endl;
        return false;
    }
    int err;
    while ((err = aio_error(&callback)) == EINPROGRESS);
    
    int ret = aio_return(&callback);
    err = aio_error(&callback);
    
    if (err != 0) {
        close(fd);
        cout << "End error: " << err << endl;
        return false;
    }

    return true;    
}

extern "C" bool WriteFileAsync(char path[BUFSIZ], char buffer[BUFSIZ]) {
    int fd = open(path, O_RDWR, S_IRUSR | S_IWUSR);
    if (fd == 0) {
        cout << "Can't open file" << endl;
        return false;
    }

    aiocb callback{ fd, LIO_WRITE, 0, buffer, strlen(buffer) };
    if (aio_write(&callback) == -1) {
        close(fd);
        cout << "Can't read file" << endl;
        return false;
    }
    int err;
    while ((err = aio_error(&callback)) == EINPROGRESS);
    
    int ret = aio_return(&callback);
    err = aio_error(&callback);
    
    if (err != 0) {
        close(fd);
        cout << "End error: " << err << endl;
        return false;
    }

    return true;    
}