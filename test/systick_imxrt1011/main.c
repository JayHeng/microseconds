/*
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "microseconds.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/


/*******************************************************************************
 * Variables
 ******************************************************************************/


/*******************************************************************************
 * Prototypes
 ******************************************************************************/
extern void SystemCoreClockUpdate (void);

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Main function
 */
int main(void)
{
    SystemCoreClockUpdate();

    microseconds_init();
    // Delay 5s
    microseconds_delay(5000000);
    // Set delay time to 5s
    microseconds_set_delay(5000000);
    // wait 5s timeout
    while(!microseconds_is_timeout());
    
    microseconds_shutdown();

    while (1)
    {
    }
}
