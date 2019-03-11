
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
     typedef union{
        struct{
        uint8_t byte;
        uint8_t byte2;
        };
        struct {
            uint8_t RDY: 1; //Ready/Busy Status: 0 Ready, 1 Busy
            uint8_t WEL: 1;// Write Enable Latch Status: 0 Not Write Enable, 1 Write Enable
            uint8_t BP0: 1;//Block Protection : 0 Entire memory array is unprotected, 1 Memory is protected
            uint8_t :    1;// Reserved Bit
            uint8_t WPP: 1;// Write Protect (WP*) Pin Status: 0 WP* is Asserted, 1 WP is not Asserted
            uint8_t EPE: 1;// Erase/Program Error : 0 operation successful, 1 error detected
            uint8_t :    1;// Reserved Bit
            uint8_t BPL: 1;// Block Protection Locked : 0 BP0 bit unlocked(default), BP0 bit locked in current state when WP* asserted.
            
            uint8_t RDY2: 1;
            uint8_t :    1;// Reserved Bit
            uint8_t :    1;// Reserved Bit
            uint8_t :    1;// Reserved Bit
            uint8_t RSTE: 1;
            uint8_t :    1;// Reserved Bit
            uint8_t :    1;// Reserved Bit
            uint8_t :    1;// Reserved Bit
        }__attribute__((packed));
    }StatusReg; //Adesto Manual
    enum Peripheral
    {   
        kSS0=0,
        kSS1=1,
        kSS2=2,
    };
    enum Master_Slave
    {
        kMaster,
        kSlave,
        
    };
    enum FrameModes 
    {
        kSPI=0 ,
        kTI=1,
        kMicrowire=2,
    };
    LabSpi(Peripheral SSPn);
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
    bool Initialize(uint8_t data_size_select, FrameModes format, uint8_t divide, Master_Slave mode);

    /**
     * Transfers a byte via SSP to an external device using the SSP data register.
     * This region must be protected by a mutex static to this class.
     *
     * @return received byte from external device via SSP data register.
     */
    uint8_t Transfer(uint8_t send);
    void chip_select();
    void chip_deselect();
    void chip_set();
    void ReadDevice();
    void ReadStatus();
    void ReadBytes();
    void WriteEnable();
    void WriteDisable();
 private:
	// Fill in as needed  
    uint8_t SpiPort;
    pinconn pc;
    StatusReg stat1;
    StatusReg stat2;
};