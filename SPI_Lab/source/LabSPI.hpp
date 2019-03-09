/*
1)Power: In the PCONP register (Section 3.3.2.2), set bit PCSSP0 to enable SSP0 and
bit PCSSP1 to enable SSP1.
Remark: On reset, SSP interfaces 0 and 1 are enabled (PCSSP0/1 = 1), while SSP2
is disabled (PCSSP2 = 0).
20 PCSSP2 SSP2 interface power/clock control bit. 0
21 PCSSP0 SSP0 interface power/clock control bit. 0

2)Pins: Select the SSP pins and pin modes through the relevant IOCON registers
(Section 7.4.1).
p1_10 CE SetLow
P1_04 MISO Input
P1_0 SCLK Output
P1_1 MOSI OutPut

3) Initialization: There are two control registers for each of the SSP ports to be
configured: SSP0CR0 and SSP0CR1 for SSP0, SSP1CR0 and SSP1CR1 for SSP1,
SSP2CR0 and SSP2CR1 for SSP2. See Section 21.6.1 and Section 21.6.2

*/
#pragma once 
#include <cstdint>
#include "L0_LowLevel/interrupt.hpp"
#include "L0_LowLevel/LPC40xx.h"
#include "utility/log.hpp"
#include "utility/time.hpp"
#include "LabGPIO.hpp"

class LabSpi
{
 public:
    enum Peripheral
    {
        kSS0=0,
        kSS1=1,
        kSS2=2,
    };
    enum FrameModes  
    {
        kSPI,
        kTI,
        kMicrowire,
    };
   
    /**
     * 1) Powers on SPPn peripheral
     * 2) Set peripheral clock
     * 3) Sets pins for specified peripheral to MOSI, MISO, and SCK
     *
     * @param data_size_select transfer size data width; To optimize the code, look for a pattern in the datasheet
     * @param format is the code format for which synchronous serial protocol you want to use.
     * @param divide is the how much to divide the clock for SSP; take care of error cases such as the value of 0, 1, and odd numbers
     *
     * @return true if initialization was successful
     */
    bool Initialize(uint8_t data_size_select, FrameModes format, uint8_t divide);

    /**
     * Transfers a byte via SSP to an external device using the SSP data register.
     * This region must be protected by a mutex static to this class.
     *
     * @return received byte from external device via SSP data register.
     */
    uint8_t Transfer(uint8_t send);
 
 private:
	// Fill in as needed  
    
};