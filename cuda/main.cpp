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
    
    // char *filename;    
    // float threshold;
    double timing, startTime, endTime;
    int numLoops;

    /* some default values */
    // threshold = 0.001;
    numLoops = 0;
    // isBinaryFile = 0;
    is_output_timing = true;
    // filename = NULL;

    while ((opt = getopt(argc, argv, "p:i:n:t:abdo")) != EOF)
    {
        switch (opt)
        {
        case 'i':
            // filename = optarg;
            cout << "option i=" << optarg << '\n';
            break;
        case 'b':
            // isBinaryFile = 1;
            break;
        case 't':
            // threshold = atof(optarg);
            break;
        case 'n':
            numLoops = atoi(optarg);
            break;
        case 'o':
            is_output_timing = true;
            break;
        case '?': //usage(argv[0], threshold);
            break;
        default: //usage(argv[0], threshold);
            break;
        }
    }

    if (is_output_timing) startTime = wtime();
    sobel(numLoops);
    if (is_output_timing) 
    {
        endTime = wtime();
        timing = endTime - startTime;
    }
    cout.precision(7);
    cout<<"--------------------------------------------\n";
    cout<<"||Sobel computation timing: "<< timing <<" sec||\n";
    cout<<"--------------------------------------------\n";
    return 0;
}