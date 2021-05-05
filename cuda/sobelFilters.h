#include <stdio.h>
#include <stdlib.h>
#include <jpeglib.h>
#include <iostream>
#include <cmath>

using namespace std;

static int MAX_STRENGTH -200;
static int MIN_STRENGTH 2000;


/*
 Horizontal kernal on a 3x3 block:

 | 1 | 2 | 1 |
 | 0 | 0 | 0 |
 |-1 |-2 |-1 |

 The center is the current position
*/
void horizontalKernel(int width, int height, unsigned char *horizontalKernelImage, unsigned char *sourceImage);

/*
 Vertical kernal on a 3x3 block:

 |-1 | 0 | 1 |
 |-2 | 0 | 2 |
 |-1 | 0 | 1 |

 The center is the current position
*/
void verticalKernel(int width, int height, unsigned char *verticalKernelImage, unsigned char *sourceImage);

/* Calculate the normalized Eucleadian distance based on horizontal and vertical filters.
*/
void edgeDiffKernel(int width, int height, unsigned char *verticalKernelImage, unsigned char *horizontalKernelImage, unsigned float* edgeDiffImage);

