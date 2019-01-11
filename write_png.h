// Writes an RGB buffer to a PNG file
// Parameters:
//   filename     The file name
//   width        The image width
//   height       The image height
//   buffer       Raw image data starting at (0, 0), ending at (width, height)
//                A byte for each red, green, blue value, 3 bytes per pixel
//                A contiguous buffer of rows from top to bottom, where each row
//                is pixels from left to right (so 3 * width bytes per row).
//   title        PNG title metadata, if NULL, don't set.
//   compression  zlib compression level, see https://www.zlib.net/manual.html
// Returns 0 on success, nonzero on failure.
int writeImage(const char* filename, unsigned long width, unsigned long height,
               unsigned char *buffer, char* title, int compression);
