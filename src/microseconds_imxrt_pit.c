/*
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*
 * @file microseconds_cortexm_systick.c
 * @brief Microseconds PIT timer driver source file
 *
 * Notes: The driver configure PIT as lifetime timer
 */
#include "microseconds.h"
#include "assert.h"
#include "fsl_device_registers.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#if defined(PIT0)
#define PIT PIT0
#elif defined(PIT1)
#define PIT PIT1
#endif
   
/*******************************************************************************
 * Variables
 ******************************************************************************/


/*******************************************************************************
 * Prototypes
 ******************************************************************************/
extern uint32_t get_pit_clock(void);

/*******************************************************************************
 * Code
 ******************************************************************************/

void microseconds_timer_init(void)
{
    // Turn on PIT: MDIS = 0, FRZ = 0
    PIT->MCR = 0x00;

    // Set up timer 1 to max value
    PIT->CHANNEL[1].LDVAL = 0xFFFFFFFF;          // setup timer 1 for maximum counting period
    PIT->CHANNEL[1].TCTRL = 0;                   // Disable timer 1 interrupts
    PIT->CHANNEL[1].TFLG = 1;                    // clear the timer 1 flag
    PIT->CHANNEL[1].TCTRL |= PIT_TCTRL_CHN_MASK; // chain timer 1 to timer 0
    PIT->CHANNEL[1].TCTRL |= PIT_TCTRL_TEN_MASK; // start timer 1

    // Set up timer 0 to max value
    PIT->CHANNEL[0].LDVAL = 0xFFFFFFFF;         // setup timer 0 for maximum counting period
    PIT->CHANNEL[0].TFLG = 1;                   // clear the timer 0 flag
    PIT->CHANNEL[0].TCTRL = PIT_TCTRL_TEN_MASK; // start timer 0
}

void microseconds_timer_deinit(void)
{
    // Turn off PIT: MDIS = 1, FRZ = 0
    for (int32_t i = 3; i >= 0; i--)
    {
        PIT->CHANNEL[i].TCTRL = 0; // stop timer
        PIT->CHANNEL[i].LDVAL = 0; // clear load value
    }

    PIT->MCR |= PIT_MCR_MDIS_MASK;
}

uint32_t microseconds_get_clock(void)
{
    return get_pit_clock();
}

//! @brief Read back the running tick count
uint64_t microseconds_get_ticks(void)
{
    uint64_t valueH;
    volatile uint32_t valueL;

#if defined(FSL_FEATURE_PIT_HAS_LIFETIME_TIMER) && (FSL_FEATURE_PIT_HAS_LIFETIME_TIMER == 1)
    // Note: first read LTMR64H and then LTMR64L. LTMR64H will have the value
    //  of CVAL1 at the time of the first access, LTMR64L will have the value of CVAL0 at the
    //  time of the first access, therefore the application does not need to worry about carry-over
    //  effects of the running counter.
    valueH = PIT->LTMR64H;
    valueL = PIT->LTMR64L;
#else
    // Make sure that there are no rollover of valueL.
    // Because the valueL always decreases, so, if the formal valueL is greater than
    // current value, that means the valueH is updated during read valueL.
    // In this case, we need to re-update valueH and valueL.
    do
    {
        valueL = PIT->CHANNEL[0].CVAL;
        valueH = PIT->CHANNEL[1].CVAL;
    } while (valueL < PIT->CHANNEL[0].CVAL);
#endif // FSL_FEATURE_PIT_HAS_LIFETIME_TIMER

    // Invert to turn into an up counter
    return ~((valueH << 32) | valueL);
}

