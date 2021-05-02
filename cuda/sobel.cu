#include <stdio.h>
#include <stdlib.h>
#include <jpeglib.h>
#include <iostream>
#include <cmath>
#include <fstream>

using namespace std;
#define PI 3.14159265

#include "sobel.h"

void sobel(int width, int height, int channels, unsigned char *image)
{
    int img2d[height][width];

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            img2d[i][j] = image[i * width + j];
        }
    }
    int img2dhororg[height][width];
    int img2dverorg[height][width];
    int img2dmag[height][width];

    ///horizontal
    int max = -200, min = 2000;

    for (int i = 1; i < height - 1; i++)
    {
        for (int j = 1; j < width - 1; j++)
        {
            int curr = img2d[i - 1][j - 1] + 2 * img2d[i - 1][j] + img2d[i - 1][j + 1] - img2d[i + 1][j - 1] - 2 * img2d[i + 1][j] - img2d[i + 1][j + 1];
            img2dhororg[i][j] = curr;
            if (curr > max)
                max = curr;
            if (curr < min)
                min = curr;
        }
    }

    ///vertical:
    max = -200;
    min = 2000;

    for (int i = 1; i < height - 1; i++)
    {
        for (int j = 1; j < width - 1; j++)
        {
            int curr = img2d[i - 1][j - 1] + 2 * img2d[i][j - 1] + img2d[i + 1][j - 1] - img2d[i - 1][j + 1] - 2 * img2d[i][j + 1] - img2d[i + 1][j + 1];
            img2dverorg[i][j] = curr;
            if (curr > max)
                max = curr;
            if (curr < min)
                min = curr;
        }
    }

    ///magnitude
    max = -200;
    min = 2000;

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            img2dmag[i][j] = sqrt(pow(img2dhororg[i][j], 2) + pow(img2dverorg[i][j], 2));
            if (img2dmag[i][j] > max)
                max = img2dmag[i][j];
            if (img2dmag[i][j] < min)
                min = img2dmag[i][j];
        }
    }

    int diff = max - min;

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            float abc = (img2dmag[i][j] - min) / (diff * 1.0);
            img2dmag[i][j] = abc * 255;
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i * width + j] = img2dmag[i][j];
        }
    }
}  
