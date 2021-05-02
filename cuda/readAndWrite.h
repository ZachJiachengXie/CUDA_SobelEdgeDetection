#ifndef READANDWRITE
#define READANDWRITE

unsigned char *readImage(char *inputFilename, int *width, int *height, int *channels, unsigned char *image);
void writeImage(char *outputFilename, int width, int height, int channels, unsigned char *image);
void closeImage(unsigned char *image);

#endif