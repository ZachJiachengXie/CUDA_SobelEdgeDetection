#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>    /* strtok() */
#include <sys/types.h> /* open() */
#include <sys/stat.h>
#include <sys/time.h>
#include <fcntl.h>
#include <unistd.h> /* getopt() */

#include "sobel.h"
#include "readAndWrite.h"

using namespace std;

double wtime(void) 
{
    double          now_time;
    struct timeval  etstart;
    struct timezone tzp;

    if (gettimeofday(&etstart, &tzp) == -1)
        perror("Error: calling gettimeofday() not successful.\n");

    now_time = ((double)etstart.tv_sec) +              /* in seconds */
               ((double)etstart.tv_usec) / 1000000.0;  /* in microseconds */
    return now_time;
}

int main(int argc, char **argv)
{
    cout << "main Begin!\n";

    int opt;
    extern char *optarg;
    // extern int optind;
    // bool isBinaryFile;
    bool is_output_timing;
    
    char *inputFilename;    
    char *outputFilename;    
    // float threshold;
    double timing, startTime, endTime;
    int width, height, channels;
    channels = 3;
    // int numLoops;

    /* some default values */
    // threshold = 0.001;
    // numLoops = 0;
    // isBinaryFile = 0;
    is_output_timing = true;
    inputFilename = NULL;
    outputFilename = NULL;

    while ((opt = getopt(argc, argv, "o:i:n:t")) != EOF)
    {
        switch (opt)
        {
        case 'i':
            inputFilename = optarg;            
            break;
        case 'o':
            outputFilename = optarg;
            break;
        case 'b':
            // isBinaryFile = 1;
            break;
        case 't':
            // threshold = atof(optarg);
            break;
        case 'n':
            // numLoops = atoi(optarg);
            break;
        case '?': //usage(argv[0], threshold);
            break;
        default: //usage(argv[0], threshold);
            break;
        }
    }

    if (!inputFilename || !outputFilename)
    {
        if (!inputFilename) cout<<"Please input filename to read using: -i filename.jpg"<<endl;
        if (!outputFilename) cout<<"Please input filename to write using: -o filename.jpg"<<endl;
        return 1;
    }

    unsigned char *image;
    image = readImage(inputFilename, &width, &height, &channels, image);
    
    if (is_output_timing) startTime = wtime();
    
    sobel(width, height, channels, image);
    
    if (is_output_timing) 
    {
        endTime = wtime();
        timing = endTime - startTime;
    }

    writeImage(outputFilename, width, height, channels, image);
    closeImage(image);

    cout.precision(7);
    cout<<"--------------------------------------------\n";
    cout<<"||Sobel computation timing: "<< timing <<" sec||\n";
    cout<<"--------------------------------------------\n";
    return 0;
}