//
//  main.cpp
//  CorePartition
//
//  Created by GUSTAVO CAMPOS on 14/07/2019.
//  Copyright © 2019 GUSTAVO CAMPOS. All rights reserved.
//


#include "CorePartition.h"
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <sys/time.h>




long getMiliseconds()
{
    struct timeval tp;
    gettimeofday(&tp, NULL);
    
    return tp.tv_sec * 1000 + tp.tv_usec / 1000; //get current timestamp in milliseconds
}

void sleepUseconds(uint32_t nTime)
{
    usleep ((useconds_t) nTime);
}


void Sleep (uint64_t nSleep)
{
    uint64_t nMomentum =  getMiliseconds();
    
    do {
        //sleepUseconds (100000);
        yield();
    } while ((getMiliseconds() - nMomentum) < nSleep);
}



void Thread1 ()
{
    unsigned int nValue = 100;
    
    while (1)
    {
        printf (">> %lu:  Value: [%u] - ScructSize: [%zu] - Memory: [%zu]\n", CorePartition_GetPartitionID(), nValue++, CorePartition_GetThreadStructSize(), CorePartition_GetPartitionAllocatedMemorySize());
        yield (); //Sleep (10);
    }
}


void Thread2 ()
{
    unsigned int nValue = 200;
    
    //setCoreNice(10);
    
    while (1)
    {
        printf ("** %lu:  Value: [%u]\n", CorePartition_GetPartitionID(), nValue++);
        
        yield(); //Sleep (10);
    }
}



void Thread3 ()
{
    unsigned int nValue = 2340000;
    
    //setCoreNice(200);
    
    while (1)
    {
        printf ("## %lu:  Value: [%u]\n", CorePartition_GetPartitionID(), nValue++);
        
        yield(); //Sleep (10);
        
    }
}


static void sleepMSTicks (uint64_t nSleepTime)
{
    printf ("Sleep: %llu\n", nSleepTime);
    
    usleep ((useconds_t) nSleepTime * 1000);
}

static uint64_t getMsTicks(void)
{
    struct timeval tp;
    gettimeofday(&tp, NULL);
    
    return tp.tv_sec * 1000 + tp.tv_usec / 1000; //get current timestamp in milliseconds
}

int main(int argc, const char * argv[])
{
    uint64_t nValue = 0x00000000000000ffLL;
    int nCount = 0;
    
    for (nCount=0; nCount < 8; nCount++)
    {
        printf ("Byte: %u, value: [%u]\n", nCount, ((uint8_t*) &nValue)[nCount] );
    }
    
    _exit (0);
    
    
    CorePartition_Start(3);
    
    CorePartition_SetCurrentTimeInterface(getMsTicks);
    CorePartition_SetSleepTimeInterface (sleepMSTicks);
    
    CreatePartition(Thread1, 256, 3000);
    CreatePartition(Thread2, 256, 1000);
    CreatePartition(Thread3, 256, 2000);
    
    join();
    
    
    return 0;
}
