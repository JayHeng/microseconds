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
    // PIT clock gate control ON
    CLOCK_EnableClock(kCLOCK_Pit);

    microseconds_init();
    
    microseconds_delay(1000);
    
    microseconds_set_delay(1000);
    
    while(!microseconds_is_timeout());
    
    microseconds_shutdown();

    while (1)
    {
    }
}
