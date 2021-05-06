#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <cuda.h>
#include <cuda_runtime.h>

#include "sobel.h"

#define GRIDVAL 20.0 
#define BLOCK_WIDTH 32
#define PI 3.14159265

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

/************************************************************************************************
* void sobel_gpu(const byte*, byte*, uint, uint);
* - This function runs on the GPU, it works on a 2D grid giving the current x, y pair being worked
* - on, the const byte* is the original image being processed and the second byte* is the image
* - being created using the sobel filter. This function runs through a given x, y pair and uses 
* - a sobel filter to find whether or not the current pixel is an edge, the more of an edge it is
* - the higher the value returned will be
* 
* Inputs: const byte* orig : the original image being evaluated
*                byte* cpu : the image being created using the sobel filter
*               uint width : the width of the image
*              uint height : the height of the image
* 
***********************************************************************************************/
__global__ void sobel_gpu(
    const unsigned char* orig, 
    unsigned char* cpu, 
    const unsigned int width, 
    const unsigned int height) 
{
    int x = threadIdx.x + blockIdx.x * blockDim.x;
    int y = threadIdx.y + blockIdx.y * blockDim.y;
    float dx, dy;
    if( x > 0 && y > 0 && x < width-1 && y < height-1) {
        dx = (-1* orig[(y-1)*width + (x-1)]) + (-2*orig[y*width+(x-1)]) + (-1*orig[(y+1)*width+(x-1)]) +
                (    orig[(y-1)*width + (x+1)]) + ( 2*orig[y*width+(x+1)]) + (   orig[(y+1)*width+(x+1)]);
        dy = (    orig[(y-1)*width + (x-1)]) + ( 2*orig[(y-1)*width+x]) + (   orig[(y-1)*width+(x+1)]) +
                (-1* orig[(y+1)*width + (x-1)]) + (-2*orig[(y+1)*width+x]) + (-1*orig[(y+1)*width+(x+1)]);
        cpu[y*width + x] = sqrt( (dx*dx) + (dy*dy) );
    }
}

void sobel_optimized(int width, int height, int channels, unsigned char *image)
{
    /** Finally, we use the GPU to parallelize it further **/
    /** Allocate space in the GPU for our original img, new img, and dimensions **/
    unsigned char *gpu_orig, *gpu_sobel;
    cudaMalloc( (void**)&gpu_orig, (width * height));
    cudaMalloc( (void**)&gpu_sobel, (width * height));
    /** Transfer over the memory from host to device and memset the sobel array to 0s **/
    cudaMemcpy(gpu_orig, image, (width * height), cudaMemcpyHostToDevice);
    cudaMemset(gpu_sobel, 0, (width * height));

    /** set up the dim3's for the gpu to use as arguments (threads per block & num of blocks)**/
    dim3 threadsPerBlock(GRIDVAL, GRIDVAL, 1);
    dim3 numBlocks(ceil(width/GRIDVAL), ceil(height/GRIDVAL), 1);

    /** Run the sobel filter using the CPU **/
    //  c = std::chrono::system_clock::now();
    sobel_gpu<<<numBlocks, threadsPerBlock>>>(gpu_orig, gpu_sobel, width, height);
    cudaError_t cudaerror = cudaDeviceSynchronize(); // waits for completion, returns error code
    if ( cudaerror != cudaSuccess ) fprintf( stderr, "Cuda failed to synchronize: %s\n", cudaGetErrorName( cudaerror ) ); // if error, output error
    //  std::chrono::duration<double> time_gpu = std::chrono::system_clock::now() - c;
    /** Copy data back to CPU from GPU **/
    cudaMemcpy(image, gpu_sobel, (width*height), cudaMemcpyDeviceToHost);


    /** Free any memory leftover.. gpuImig, cpuImg, and ompImg get their pixels free'd while writing **/
    cudaFree(gpu_orig); cudaFree(gpu_sobel);
}  
