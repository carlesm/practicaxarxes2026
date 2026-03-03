#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <signal.h>
#include <sys/select.h>
#include <sys/time.h>

#include "tap.h"


#define KEEPALIVE_INTERVAL_SEC 10
#define MAX_FRAME_SIZE         65535


/* Configuration parsed from command-line arguments */
typedef struct {
    const char *tap_if;
    const char *server_ip;
    int         port;
    int         client_id;
    char        password[9]; /* 8 chars + NUL */
} vpn_config_t;

static void print_usage(const char *prog)
{
    fprintf(stderr,
        "Usage: %s --tap <interface> --server <ip> --port <port>\n"
        "           --id <number> --password <string>\n"
        "\n"
        "Options:\n"
        "  --tap      <interface>  TAP interface name (e.g. tap0)\n"
        "  --server   <ip>         Server IPv4 address\n"
        "  --port     <port>       UDP port (1-65535)\n"
        "  --id       <number>     Client ID (0-65535)\n"
        "  --password <string>     8-character alphanumeric password [A-Za-z0-9]\n"
        "  --help                  Print this usage and exit\n",
        prog);
}

/**
 * Validate a password string.
 *
 * Returns 0 if the password is valid, or -1 if it is not.
 *
 * A password is considered valid if it is exactly 8 characters
 * long and only contains alphanumeric characters (A-Za-z0-9).
 */
static int validate_password(const char *pw)
{
    int i;
    if (strlen(pw) != 8) {
        return -1;
    }
    for (i = 0; i < 8; i++) {
        if (!isalnum((unsigned char)pw[i])) {
            return -1;
        }
    }
    return 0;
}


/**
 * Parse command-line arguments into a vpn_config_t structure.
 *
 * Returns 1 on success, 0 on parsing error (an error message and/or
 * usage is printed to stderr), and -1 if "--help" was requested.  The
 * cfg structure is zeroed before parsing so callers do not need to
 * initialize it.
 *
 * Return codes:
 *   1  success
 *   0  parsing error (an error message and/or usage is printed to stderr)
 *  -1  "--help" was requested (usage printed, caller should exit 0)
 */
static int parse_args(int argc, char *argv[], vpn_config_t *cfg)
{
    int i;
    int has_tap = 0, has_server = 0, has_port = 0, has_id = 0, has_password = 0;

    memset(cfg, 0, sizeof(*cfg));

    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--help") == 0) {
            print_usage(argv[0]);
            return -1; /* help requested */

        } else if (strcmp(argv[i], "--tap") == 0) {
            if (i + 1 >= argc) {
                fprintf(stderr, "Error: --tap requires an argument\n");
                print_usage(argv[0]);
                return 0;
            }
            cfg->tap_if = argv[++i];
            has_tap = 1;

        } else if (strcmp(argv[i], "--server") == 0) {
            if (i + 1 >= argc) {
                fprintf(stderr, "Error: --server requires an argument\n");
                print_usage(argv[0]);
                return 0;
            }
            cfg->server_ip = argv[++i];
            has_server = 1;

        } else if (strcmp(argv[i], "--port") == 0) {
            if (i + 1 >= argc) {
                fprintf(stderr, "Error: --port requires an argument\n");
                print_usage(argv[0]);
                return 0;
            }
            {
                char *end;
                long val = strtol(argv[++i], &end, 10);
                if (*end != '\0' || val < 1 || val > 65535) {
                    fprintf(stderr, "Error: --port must be in range 1-65535\n");
                    print_usage(argv[0]);
                    return 0;
                }
                cfg->port = (int)val;
            }
            has_port = 1;

        } else if (strcmp(argv[i], "--id") == 0) {
            if (i + 1 >= argc) {
                fprintf(stderr, "Error: --id requires an argument\n");
                print_usage(argv[0]);
                return 0;
            }
            {
                char *end;
                long val = strtol(argv[++i], &end, 10);
                if (*end != '\0' || val < 0 || val > 65535) {
                    fprintf(stderr, "Error: --id must be in range 0-65535\n");
                    print_usage(argv[0]);
                    return 0;
                }
                cfg->client_id = (int)val;
            }
            has_id = 1;

        } else if (strcmp(argv[i], "--password") == 0) {
            if (i + 1 >= argc) {
                fprintf(stderr, "Error: --password requires an argument\n");
                print_usage(argv[0]);
                return 0;
            }
            {
                const char *pw = argv[++i];
                if (validate_password(pw) != 0) {
                    fprintf(stderr,
                        "Error: --password must be exactly 8 alphanumeric characters [A-Za-z0-9]\n");
                    return 0;
                }
                memcpy(cfg->password, pw, 8);
                cfg->password[8] = '\0';
            }
            has_password = 1;

        } else {
            fprintf(stderr, "Error: unknown argument: %s\n", argv[i]);
            print_usage(argv[0]);
            return 0;
        }
    }

    if (!has_tap || !has_server || !has_port || !has_id || !has_password) {
        fprintf(stderr, "Error: missing required arguments\n");
        print_usage(argv[0]);
        return 0;
    }

    return 0;
}

int main(int argc, char *argv[])
{
    vpn_config_t cfg;

    int ret = parse_args(argc, argv, &cfg);
    if (ret <= 0) {
        /* ret == 0 -> parsing error exit -1, ret < 0 -> help requested exit 0*/
        return (ret < 0) ? 0 : -1;
    }

    /* Open TAP device, you should handle this out of main*/
    int tap_fd = tap_open(cfg.tap_if);
    if (tap_fd < 0) {
        fprintf(stderr, "Error: could not open TAP device %s\n", cfg.tap_if);
        return 1;
    }
    close(tap_fd);

    // TODO: Start client run loop, which should handle everything after this point, including:
    // - Sending keepalive packets to the server every KEEPALIVE_INTERVAL_SEC seconds
    // - Reading frames from the TAP device and sending them to the server
    // - Receiving packets from the server and writing them to the TAP device
    // You should implement this in a separate function (e.g. client_run) 
    // and keep code clean and tidy. 
    // client_run(&cfg, tap_fd);

    return 0;
}

