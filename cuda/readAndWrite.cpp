#include <stdint.h>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <cmath>

#include "readAndWrite.h"

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include "stb_image.h"

using namespace std;

unsigned char *readImage(char *inputFilename, int *width, int *height, int *channels, unsigned char *image)
{
    image = stbi_load(inputFilename, width, height, channels, 3);

    cout << "Reading image: " << inputFilename << endl;
    cout << "Image info: width= " << *width << " height= " << *height << " channels= " << *channels << endl;
    /* If image is a RGB image, convert it to grayscale */
    if (*channels == 3)
    {        
        unsigned char *image2;
        image2 = (unsigned char *)malloc((*width) * (*height) * sizeof(image));
        (*channels) = 1;
        for (int i = 0; i < (*height); i++)
        {
            for (int j = 0; j < (*width); j++)
            {
                image2[i * (*width) + j] = (image[i * (*width) * 3 + j * 3] + image[i * (*width) * 3 + j * 3 + 1] + image[i * (*width) * 3 + j * 3 + 2]) / 3;
            }
        }
        free(image);
        image = image2;

        /* Output the matrix to a file. */
        // ofstream myFile("stb1.csv");
        // for (int i = 0; i < (*width) * (*height) *(*channels); ++i)
        // {
        //     myFile << +image[i] << ',';
        //     if ((i + 1) % (*width) == 0)
        //     {
        //         myFile << '\n';
        //     }
        // }
        // myFile.close();

        /* Write the temporary grayscale image. */
        // char tmpStr[] = "grayscale.jpg";
        // cout << "Writing grayscale." << endl;
        // stbi_write_jpg(tmpStr, (*width), (*height), (*channels), image, 95);

        return image;
    }
}

void writeImage(char *outputFilename, int width, int height, int channels, unsigned char *image)
{
    stbi_write_jpg(outputFilename, width, height, channels, image, 95);
    cout << "Writing image: " << outputFilename << endl;
}

void closeImage(unsigned char *image)
{
    stbi_image_free(image);

}