#ifndef TAP_H
#define TAP_H

#include <stddef.h>

/* Open the TAP device with the given interface name.
 * Returns a valid fd on success, or -1 on error. */
int tap_open(const char *ifname);

/* Read one Ethernet frame from the TAP device.
 * Returns bytes read on success, or -1 on error. */
int tap_read(int fd, void *buf, size_t maxlen);

/* Write one Ethernet frame to the TAP device.
 * Returns bytes written on success, or -1 on error. */
int tap_write(int fd, const void *buf, size_t len);

/* Close the TAP device.
 * Returns 0 on success, or -1 on error. */
int tap_close(int fd);
#endif /* TAP_H */
