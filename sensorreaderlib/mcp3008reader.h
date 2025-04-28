#ifndef MCP3008READER_H
#define MCP3008READER_H

#include <QString>

class MCP3008Reader
{
public:
    explicit MCP3008Reader(const QString &device);
    ~MCP3008Reader();

    int readChannel(int channel);  // returns value or -1 on error

private:
    int fd;
};

#endif // MCP3008READER_H
