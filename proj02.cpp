#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include <cstdlib>
#include <cmath>
#define _USE_MATH_DEFINES
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// print debugging messages?
#ifndef DEBUG
#define DEBUG false
#endif

// Global variables
int NowYear;  // 2023 - 2028
int NowMonth; // 0 - 11

float NowPrecip;   // inches of rain per month
float NowTemp;     // temperature this month
float NowHeight;   // rye grass height in inches
int NowNumRabbits; // number of rabbits in the current population

omp_lock_t Lock;
volatile int NumInThreadTeam;
volatile int NumAtBarrier;
volatile int NumGone;

// Constant variables
const float RYEGRASS_GROWS_PER_MONTH = 20.0;
const float ONE_RABBITS_EATS_PER_MONTH = 1.0;

const float AVG_PRECIP_PER_MONTH = 12.0; // average
const float AMP_PRECIP_PER_MONTH = 4.0;  // plus or minus
const float RANDOM_PRECIP = 2.0;         // plus or minus noise

const float AVG_TEMP = 60.0;    // average
const float AMP_TEMP = 20.0;    // plus or minus
const float RANDOM_TEMP = 10.0; // plus or minus noise

const float MIDTEMP = 60.0;
const float MIDPRECIP = 14.0;

// Random function
float Ranf(float low, float high)
{
    float r = (float)rand(); // 0 - RAND_MAX

    return (low + r * (high - low) / (float)RAND_MAX);
}

float x = Ranf(-1.f, 1.f);

// Barrier function template
void InitBarrier(int n)
{
    NumInThreadTeam = n;
    NumAtBarrier = 0;
    omp_init_lock(&Lock);
}

// Have the calling thread wait here until all the other threads catch up:

void WaitBarrier()
{
    omp_set_lock(&Lock);
    {
        NumAtBarrier++;
        if (NumAtBarrier == NumInThreadTeam)
        {
            NumGone = 0;
            NumAtBarrier = 0;
            // let all other threads get back to what they were doing
            // before this one unlocks, knowing that they might immediately
            // call WaitBarrier( ) again:
            while (NumGone != NumInThreadTeam - 1)
                ;
            omp_unset_lock(&Lock);
            return;
        }
    }
    omp_unset_lock(&Lock);

    while (NumAtBarrier != 0)
        ; // this waits for the nth thread to arrive

#pragma omp atomic
    NumGone++; // this flags how many threads have returned
}

void Rabbits()
{
    while (NowYear < 2029)
    {
        // compute a temporary next-value for this quantity
        // based on the current state of the simulation:
        //. . .

        // DoneComputing barrier:
        WaitBarrier();
        //. . .

        // DoneAssigning barrier:
        WaitBarrier();
        //. . .

        // DonePrinting barrier:
        WaitBarrier();
        //. . .
    }
}

void RyeGrass()
{
    while (NowYear < 2029)
    {
        // compute a temporary next-value for this quantity
        // based on the current state of the simulation:
        //. . .

        // DoneComputing barrier:
        WaitBarrier();

        //. . .

        // DoneAssigning barrier:
        WaitBarrier();

        //. . .

        // DonePrinting barrier:
        WaitBarrier();

        //. . .
    }
}

void Watcher()
{
    while (NowYear < 2029)
    {
        // compute a temporary next-value for this quantity
        // based on the current state of the simulation:
        //. . .

        // DoneComputing barrier:
        WaitBarrier();

        //. . .

        // DoneAssigning barrier:
        WaitBarrier();

        //. . .

        // DonePrinting barrier:
        WaitBarrier();

        //. . .
    }
}

void MyAgent()
{
    while (NowYear < 2029)
    {
        // compute a temporary next-value for this quantity
        // based on the current state of the simulation:
        //...

        // DoneComputing barrier:
        WaitBarrier();

        //...

        // DoneAssigning barrier:
        WaitBarrier();

        //...

        // DonePrinting barrier:
        WaitBarrier();

        //...
    }
}

float Sqr(float x)
{
    return x * x;
}

int main(int argc, char *argv[])
{
#ifdef _OPENMP
    // fprintf( stderr, "OpenMP is supported -- version = %d\n", _OPENMP );
#else
    fprintf(stderr, "No OpenMP support!\n");
    return 1;
#endif
    // Sets up Temp and Precipitation
    float ang = (30. * (float)NowMonth + 15.) * (M_PI / 180.);

    float temp = AVG_TEMP - AMP_TEMP * cos(ang);
    NowTemp = temp + Ranf(-RANDOM_TEMP, RANDOM_TEMP);

    float precip = AVG_PRECIP_PER_MONTH + AMP_PRECIP_PER_MONTH * sin(ang);
    NowPrecip = precip + Ranf(-RANDOM_PRECIP, RANDOM_PRECIP);
    if (NowPrecip < 0.)
        NowPrecip = 0.;

    // Starting date and time:
    NowMonth = 0;
    NowYear = 2023;

    // Starting state:
    NowNumRabbits = 1;
    NowHeight = 5.;
    omp_set_num_threads(4); // same as # of sections
    InitBarrier(4);

#pragma omp parallel sections
    {
#pragma omp section
        {
            Rabbits();
        }

#pragma omp section
        {
            RyeGrass();
        }

#pragma omp section
        {
            Watcher();
        }

#pragma omp section
        {
            MyAgent(); // your own
        }
    } // implied barrier -- all functions must return in order
      // to allow any of them to get past here
}
