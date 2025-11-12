/**
 *  @author Enzo R. Isotton (eri@vtluug.org)
 *  
 *  Modified from sxot.c (https://codeberg.org/NRK/sxot)
 */

#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <assert.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

#include <X11/Xlib.h>

const char* const usage =
    "Usage: xprtsc [-v|-h]\n"
	"Capture a screenshot and writes it in binary ppm format to stdout.\n"
    "Future manpage will give more details. For now, read the source code.\n";
const char* const version =
    "xprtsc v0.1.1\n"
    "License: GPLv3+ (https://gnu.org/licenses/gpl.html)\n";

int full_write(int fd, uint8_t* s, size_t len) {
	while (len > 0) {
		ssize_t res = write(fd, s, len);
		if (res < 0)
			return -1;
		
        s += res; len -= (size_t)res;
	}
	return 0;
}

void write_ppm(uint8_t* data, int width, int height, bool msb_first, int fd) {
    uint8_t header[64];
    size_t header_len = sprintf(header, "P6\n%d %d\n255\n", width, height);
	full_write(fd, header, header_len);

	uint8_t* w = data;
	uint8_t* end = data + ((ssize_t)(width * height * 4));
	
    for (uint8_t* p = data; p < end; p += 4) {
		int R, G, B;
		if (msb_first) {
			R = p[1];
            G = p[2];
            B = p[3];
        } 
        else {
			R = p[2];
            G = p[1];
            B = p[0];
		}
		*w++ = R; *w++ = G; *w++ = B;
	}
	assert(w < end);
	full_write(fd, data, w - data);
}

int main(int argc, char* argv[]) {
    if (argc == 1)
        goto ARG_END;
    
    if (argc > 3) {
        fprintf(stderr, "invalid number of argument (max 2)");
        return 1;
    }

    if (argv[1][0] != '-') {
        fprintf(stderr, "invalid argument: %s\n", argv[1]);
        return 1;
    }
            
    if (!(strcmp(argv[1], "-h") && strcmp(argv[1], "--help"))) {
        fprintf(stdout, usage);
        return 0;
    }
    if (!(strcmp(argv[1], "-v") && strcmp(argv[1], "--version"))) {
        fprintf(stdout, version);
        return 0;
    }

    fprintf(stderr, "invalid flag: %s\n", argv[1]);
    return 1;

ARG_END: ;

    int fd = STDOUT_FILENO;
    
    Display* dpy = XOpenDisplay(NULL);
	if (dpy == NULL) {
		fprintf(stderr, "failed to open X Display\n");
        return 1;
	}
    
    Window root = DefaultRootWindow(dpy);
    XWindowAttributes win;
    if (!XGetWindowAttributes(dpy, root, &win)) {
    	fprintf(stderr, "XGetWindowAttributes failed\n");
        return 1;
    }
	
	XImage* im = XGetImage(
		dpy, root, win.x, win.y, win.width, win.height,
		AllPlanes, ZPixmap
	);
	if (im == NULL) {
		fprintf(stderr, "XGetImage failed\n");
        return 1;
	}
	if (im->bits_per_pixel != 32 || im->bytes_per_line != (im->width * 4) ||
	    !(im->depth == 24 || im->depth == 32))
	{
		fprintf(stderr, "unsupported XImage format\n");
        return 1;
	}

	write_ppm((uint8_t *)im->data, im->width, im->height, im->byte_order == MSBFirst, fd);

    im->f.destroy_image(im);
	XCloseDisplay(dpy);

	return 0;
}
