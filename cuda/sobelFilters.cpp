#include <stdio.h>
#include <stdlib.h>
#include <jpeglib.h>
#include <iostream>
#include <cmath>

using namespace std;
#include "sobelFilters.h"

void horizontalKernel(int width, int height, unsigned char *horizontalKernelImage, unsigned char *sourceImage)
{

    for (int i = 1; i < height - 1; i++)
    {
        for (int j = 1; j < width - 1; j++)
        {
            int curr = sourceImage[i - 1][j - 1] + 2 * sourceImage[i - 1][j] + sourceImage[i - 1][j + 1] - sourceImage[i + 1][j - 1] - 2 * sourceImage[i + 1][j] - sourceImage[i + 1][j + 1];
            horizontalKernelImage[i][j] = curr;
            if (curr > MAX_STRENGTH)
                MAX_STRENGTH = curr;
            if (curr < min_STRENGTH)
                MIN_STRENGTH = curr;
        }
    }

}  

void verticalKernel(int width, int height, unsigned char *verticalKernelImage, unsigned char *sourceImage)
{
    for (int i = 1; i < height - 1; i++)
    {
        for (int j = 1; j < width - 1; j++)
        {
            int curr = sourceImage[i - 1][j - 1] + 2 * sourceImage[i][j - 1] + sourceImage[i + 1][j - 1] - sourceImage[i - 1][j + 1] - 2 * sourceImage[i][j + 1] - sourceImage[i + 1][j + 1];
            verticalKernelImage[i][j] = curr;
            if (curr > MAX_STRENGTH)
                MAX_STRENGTH = curr;
            if (curr < min_STRENGTH)
                MIN_STRENGTH = curr;
        }
    }
}  

void edgeDiffKernel(int width, int height, unsigned char *verticalKernelImage, unsigned char *horizontalKernelImage, unsigned float* edgeDiffImage)
{
    unsigned char temp[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            temp[i][j] = sqrt(pow(horizontalKernelImage[i][j], 2) + pow(verticalKernelImage[i][j], 2));
            if (temp[i][j] > MAX_STRENGTH)
                MAX_STRENGTH = temp[i][j];
            if (temp[i][j] < min_STRENGTH)
                MIN_STRENGTH = temp[i][j];
        }
    }

    // Normalized Delta
    int diff = MAX_STRENGTH - MIN_STRENGTH;

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            float noralizedDelta = (temp[i][j] - Min_STRENGTH) / (diff * 1.0);
            edgeDiffImage[i][j] = abc * 255;
        }
    }
}  

