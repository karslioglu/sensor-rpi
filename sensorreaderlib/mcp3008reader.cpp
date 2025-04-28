#include "mcp3008reader.h"
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>
#include <cstring>
#include <QDebug>

MCP3008Reader::MCP3008Reader(const QString &device)
{
    fd = open(device.toStdString().c_str(), O_RDWR);
    if (fd < 0) {
        qCritical() << "Failed to open SPI device:" << device;
        return;
    }

    uint8_t mode = SPI_MODE_0;
    uint8_t bits = 8;
    uint32_t speed = 1000000;

    ioctl(fd, SPI_IOC_WR_MODE, &mode);
    ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits);
    ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
}

MCP3008Reader::~MCP3008Reader()
{
    if (fd >= 0)
        close(fd);
}

int MCP3008Reader::readChannel(int channel)
{
    if (fd < 0 || channel < 0 || channel > 7)
        return -1;

    uint8_t tx[] = { 0x01, static_cast<uint8_t>((0x08 | channel) << 4), 0x00 };
    uint8_t rx[3] = {0};

    struct spi_ioc_transfer tr{};
    tr.tx_buf = reinterpret_cast<__u64>(tx);
    tr.rx_buf = reinterpret_cast<__u64>(rx);
    tr.len = 3;
    tr.speed_hz = 1000000;
    tr.bits_per_word = 8;

    int ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
    if (ret < 1)
        return -1;

    int value = ((rx[1] & 0x03) << 8) | rx[2];
    return value;
}
