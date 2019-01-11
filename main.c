#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "perlin.h"
#include "write_png.h"

void create_image(unsigned long frame, unsigned long width,
                  unsigned long height, unsigned char *buffer);

int main(int argc, char *argv[]) {
  unsigned long width = 640;
  unsigned long height = 480;
  unsigned long frame = 0;

  // PNG compression level. Uses zlib, see deflate_init in
  // https://www.zlib.net/manual.html
  //
  // #define Z_NO_COMPRESSION         0
  // #define Z_BEST_SPEED             1
  // #define Z_BEST_COMPRESSION       9
  // #define Z_DEFAULT_COMPRESSION  (-1)
  //
  // Empirically, this kind of image yields the following for 3840x2160 frame 0:
  //
  // Compression level, seconds, PNG file size
  // 0 0.858 24925706
  // 1 1.056 7070220
  // 2 1.106 6784025
  // 3 1.327 6508929
  // 4 1.310 5811411
  // 5 1.614 5668109
  // 6 2.509 5521967
  // 7 3.544 5384009
  // 8 11.808 5127623
  // 9 21.975 5008784
  unsigned long compression = 1;

  // TODO: sanity check input
  if (argc >= 2) {
    frame = strtoul(argv[1], NULL, 0);
  }

  if (argc >= 4) {
    width = strtoul(argv[2], NULL, 0);
    height = strtoul(argv[3], NULL, 0);
  }

  if (argc >= 5) {
    compression = strtoul(argv[4], NULL, 0);
    if (compression > 9) {
      compression = 9;
    }
  }

  printf("Creating frame %05lu at %lux%lu, compression level %lu...",
      frame, width, height, compression);
  fflush(stdout);

  unsigned char *buffer = malloc(width * height * 3);
  if (buffer == NULL) {
    fputs("malloc failed\n", stderr);
    exit(1);
  }

  perlin_init();
  create_image(frame, width, height, buffer);
  perlin_done();

  char filename[64];
  snprintf(filename, sizeof filename, "image%05lu.png", frame);
  int r = writeImage(filename, width, height, buffer, filename, (int)compression);
  if (r != 0) {
    puts("failed.");
  } else {
    puts("done.");
  }
  free(buffer);
}

void create_image(unsigned long frame, unsigned long width,
                  unsigned long height, unsigned char *buffer) {
  double r, g, b;
  double scaling_factor = 4.0;
  double r_div = 73.0 * scaling_factor;
  double g_div = 47.0 * scaling_factor;
  double b_div = 89.0 * scaling_factor;
  // "time", larger = slower movement.
  double t_div = 80.0;

  double f = frame / t_div;

  for (unsigned long y = 0; y < height; y++) {
    for (unsigned long x = 0; x < width; x++) {
      r = floor(perlin(x / r_div, y / r_div, f) * 255.0);
      g = floor(perlin(x / g_div, y / g_div, f) * 255.0);
      b = floor(perlin(x / b_div, y / b_div, f) * 255.0);
      *buffer++ = (unsigned char)r;
      *buffer++ = (unsigned char)g;
      *buffer++ = (unsigned char)b;
    }
  }
}
