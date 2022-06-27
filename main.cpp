#include <X11/Xlib.h>
#include <bits/stdc++.h>
#include <fcntl.h>
#include <linux/uinput.h>
#include <sys/stat.h>

using namespace std;

const int CPS = 15;
const int Rand = 10000; // 10ms

// fast pseudorandom numbers https://www.shadertoy.com/view/ttc3zr
uint murmurHash(uint src)
{
    const uint M = 0x5bd1e995u;
    uint h = 1190494759u;
    src *= M;
    src ^= src >> 24u;
    src *= M;
    h *= M;
    h ^= src;
    h ^= h >> 13u;
    h *= M;
    h ^= h >> 15u;
    return h;
}

void emit(int fd, int type, int code, int val)
{
    struct input_event ie;

    memset(&ie, 0, sizeof(struct input_event));

    ie.type = type;
    ie.code = code;
    ie.value = val;
    /* timestamp values below are ignored */
    ie.time.tv_sec = 0;
    ie.time.tv_usec = 0;

    write(fd, &ie, sizeof(ie));
}

void click(int fd)
{
    emit(fd, EV_KEY, BTN_LEFT, 1);
    emit(fd, EV_SYN, SYN_REPORT, 0);

    emit(fd, EV_KEY, BTN_LEFT, 0);
    emit(fd, EV_SYN, SYN_REPORT, 0);
}

int main()
{
    Display *display = XOpenDisplay(0);

    struct uinput_setup usetup;

    int fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);

    ioctl(fd, UI_SET_EVBIT, EV_KEY);
    ioctl(fd, UI_SET_KEYBIT, BTN_LEFT);

    ioctl(fd, UI_SET_EVBIT, EV_REL);
    ioctl(fd, UI_SET_RELBIT, REL_X);
    ioctl(fd, UI_SET_RELBIT, REL_Y);

    memset(&usetup, 0, sizeof(usetup));
    usetup.id.bustype = BUS_USB;
    usetup.id.vendor = 69;   /* sample vendor */
    usetup.id.product = 420; /* sample product */
    strcpy(usetup.name, "SpamBoi");

    ioctl(fd, UI_DEV_SETUP, &usetup);
    ioctl(fd, UI_DEV_CREATE);

    int t = 0;
    /* event loop */
    while (1)
    {
        char keys[32];
        XQueryKeymap(display, keys);

        // check if f6 pressed
        if (keys[9] & 1)
        {
            // click(fd);
            cerr << "Clicked!\n";
        }

        int r = murmurHash(t++) % (2 * Rand) - Rand;

        usleep((1000000 / CPS) + r);
    }

    sleep(1);

    ioctl(fd, UI_DEV_DESTROY);
    close(fd);

    return 0;
}