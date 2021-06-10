/*
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "microseconds.h"
#include "fsl_clock.h"
#include "fsl_device_registers.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/


/*******************************************************************************
 * Variables
 ******************************************************************************/


/*******************************************************************************
 * Prototypes
 ******************************************************************************/


/*******************************************************************************
 * Code
 ******************************************************************************/

//! @brief Get PIT clock value
uint32_t get_pit_clock(void)
{
    // Get PIT clock source
    uint32_t ahbBusDivider = ((CCM->CBCDR & CCM_CBCDR_IPG_PODF_MASK) >> CCM_CBCDR_IPG_PODF_SHIFT) + 1;
    uint32_t periphDivider = ((CCM->CSCMR1 & CCM_CSCMR1_PERCLK_PODF_MASK) >> CCM_CSCMR1_PERCLK_PODF_SHIFT) + 1;
    return SystemCoreClock / ahbBusDivider / periphDivider;
}

/*!
 * @brief Main function
 */
int main(void)
{
    SystemCoreClockUpdate();

    // PIT clock gate control ON
    CLOCK_EnableClock(kCLOCK_Pit);

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
