/**
    Copyright Notice:
    Copyright 2021 DMTF. All rights reserved.
    License: BSD 3-Clause License. For full text see link: https://github.com/DMTF/libspdm/blob/main/LICENSE.md
**/

#include <base.h>
#include <stdlib.h>
#include <windows.h>
#include <stdio.h>
#include <process.h>
#include <time.h>
#include "hal/library/timelib.h"

LARGE_INTEGER start;


/**
  Suspends the execution of the current thread until the time-out interval elapses.

  @param[in] milliseconds     The time interval for which execution is to be suspended, in milliseconds.

**/
void libspdm_sleep(uint64_t milliseconds)
{
    Sleep((DWORD)milliseconds);
}

void libspdm_settimer(uint64_t timeout)
{
    QueryPerformanceCounter(&start);
}

boolean libspdm_checktimer(uint64_t timeout)
{
    uint64_t delta;
    LARGE_INTEGER now, hfreq;
    QueryPerformanceCounter(  &now );
    QueryPerformanceFrequency( &hfreq );
    delta = (uint64_t)((now.QuadPart - start.QuadPart) * 1000ul
                / hfreq.QuadPart);

    if(delta * 1000ul > timeout) {
        return TRUE;
    } else {
        return FALSE;
    }
}

unsigned Counter = 0;
boolean terminated;
HANDLE hThread;
uint8_t heartbeat_period;
libspdm_stop_session_func spdm_stop_session_func;


unsigned __stdcall ThreadFunc(void* context)
{
    uintn total_loop;
    uintn index;
    spdm_stop_session_t *spdm_stop_session_context;

    spdm_stop_session_context = context;
    total_loop = heartbeat_period / 10 + 1;
    printf( "QIZ: In second thread...\n" );

    index = 0;
    while ((!terminated) && (index <= total_loop)) {
        index ++;
        printf("QIZ: counter %Id\n", index);
        Sleep(10);
    }

    if(index > total_loop) {
      spdm_stop_session_func(spdm_stop_session_context->spdm_context,
                             spdm_stop_session_context->session_id,
                             spdm_stop_session_context->end_session_attributes);
    }
    printf( "QIZ: second thread ends...\n" );
    _endthreadex( 0 );
    return 0;
}


boolean init_watchdog(uint8_t seconds, spdm_stop_session_t context,
                           libspdm_stop_session_func spdm_stop_session)
{

   unsigned threadID;
   
   heartbeat_period = seconds;
   spdm_stop_session_func = spdm_stop_session;
   
   printf( "QIZ: Creating second thread... hThread %lld\n", (uint64_t)hThread);

   terminated = FALSE;

   hThread = (HANDLE)_beginthreadex( NULL, 0, &ThreadFunc, &context, 0, &threadID );

   return TRUE;
}

boolean reset_watchdog()
{
    printf("QIZ: reset_watchdog hThread %lld\n", (uint64_t)hThread);
    if(hThread != 0) {
        terminated = TRUE;
        CloseHandle( hThread );
        hThread = 0;
    }
    return TRUE;
}
