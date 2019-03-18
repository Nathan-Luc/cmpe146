#pragma once
#include "LabSPI.hpp"

class AdestoFlash
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
    AdestoFlash();
    void ReadDevice();
    void ReadStatus();
    void ReadBytes();
    void WriteEnable();
    void WriteDisable();
    private:
    LabSpi Spi;
    StatusReg stat1;
    StatusReg stat2;
};