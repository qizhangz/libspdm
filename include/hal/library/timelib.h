/**
    Copyright Notice:
    Copyright 2021 DMTF. All rights reserved.
    License: BSD 3-Clause License. For full text see link: https://github.com/DMTF/libspdm/blob/main/LICENSE.md
**/

#ifndef __TIME_LIB_H__
#define __TIME_LIB_H__

/**
  Suspends the execution of the current thread until the time-out interval elapses.

  @param[in] milliseconds     The time interval for which execution is to be suspended, in milliseconds.

**/

typedef return_status (*libspdm_stop_session_func)(IN void *spdm_context,
                               IN uint32_t session_id,
                               IN uint8_t end_session_attributes);

typedef struct {
    void *spdm_context;
    uint32_t session_id;
    uint8_t end_session_attributes;
} spdm_stop_session_t;

void libspdm_sleep(uint64_t milliseconds);

void libspdm_settimer(uint64_t timeout);

boolean libspdm_checktimer(uint64_t timeout);

boolean init_watchdog(uint8_t seconds, spdm_stop_session_t context,
                           libspdm_stop_session_func spdm_stop_session);

boolean reset_watchdog();


#endif // __TIME_LIB_H__
