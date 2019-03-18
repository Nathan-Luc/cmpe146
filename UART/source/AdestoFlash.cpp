#include "AdestoFlash.hpp"
//AdestoFlash::AdestoFlash(){
    //LabSpi Hold(LabSpi::Peripheral::kSS2);
    
//}
void AdestoFlash::ReadStatus(){
    Spi.chip_select();
    Spi.Transfer(0x05);
    stat1.byte=Spi.Transfer(0xFF);
    stat2.byte=Spi.Transfer(0xFF);
    printf("Status: %x %x\n",stat1.byte, stat2.byte);
    Spi.chip_deselect();

}

void AdestoFlash::ReadDevice(){
    uint8_t data[4];
    Spi.chip_select();
    Spi.Transfer(0x9F);
    for(int i =0; i<4; i++)
    {
    data[i] = (Spi.Transfer(0xFF));
    }
    Spi.chip_deselect();
    printf("Returned data: %x %x %x %x\n", data[0], data[1], data[2], data[3]);
}
void AdestoFlash::WriteEnable(){
    Spi.chip_select();
    Spi.Transfer(0x06);
    Spi.chip_deselect();
}
void AdestoFlash::WriteDisable(){
    Spi.chip_select();
    Spi.Transfer(0x04);
    Spi.chip_deselect();
}
void AdestoFlash::ReadBytes(){
    if(stat1.RDY)
    printf("Ready/Busy Bit is busy: %x\n",stat1.RDY);
    else 
    printf("Read/Busy Bit is ready: %x\n",stat1.RDY);
    if(stat1.WEL)
    printf("Write Enable bit is Enable: %x\n", stat1.WEL);
    else 
    printf("Write Enable bit is not enabled: %x\n", stat1.WEL);
    if(stat1.BP0)
    printf("Memory is protected: %x\n",stat1.BP0);
    else 
    printf("Not protected: %x\n",stat1.BP0);
    if(stat1.WPP)   
    printf("Write Protect is not asserted: %x\n",stat1.WPP);
    else 
    printf("Write Protect asserted: %x\n",stat1.WPP);
    if(stat1.EPE)
    printf("Erase Error dectected: %x\n",stat1.EPE);
    else
    printf("Operation Successful: %x\n",stat1.EPE);
    if(stat2.RDY2)
    printf("Ready/Busy 2 is busy: %x\n",stat2.RDY2);
    else 
    printf("Ready/Busy 2 is ready: %x\n",stat2.RDY2);
    if(stat2.RSTE)
    printf("Reset command enabled: %x\n",stat2.RSTE);
    else
    printf("Reset command disabled: %x\n", stat2.RSTE);
}