#include "tap.h"

#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/if.h>
#include <linux/if_tun.h>

int tap_open(const char *ifname)
{
    struct ifreq ifr;
    int fd;

    fd = open("/dev/net/tun", O_RDWR);
    if (fd < 0) {
        perror("tap_open: open /dev/net/tun");
        return -1;
    }

    memset(&ifr, 0, sizeof(ifr));
    ifr.ifr_flags = IFF_TAP | IFF_NO_PI;
    strncpy(ifr.ifr_name, ifname, IFNAMSIZ - 1);

    if (ioctl(fd, TUNSETIFF, &ifr) < 0) {
        perror("tap_open: ioctl TUNSETIFF");
        close(fd);
        return -1;
    }

    return fd;
}

int tap_read(int fd, void *buf, size_t maxlen)
{
    ssize_t n = read(fd, buf, maxlen);
    if (n < 0) {
        perror("tap_read");
        return -1;
    }
    return (int)n;
}

int tap_write(int fd, const void *buf, size_t len)
{
    ssize_t n = write(fd, buf, len);
    if (n < 0) {
        perror("tap_write");
        return -1;
    }
    return (int)n;
}

int tap_close(int fd)
{
    if (close(fd) < 0) {
        perror("tap_close");
        return -1;
    }
    return 0;
}