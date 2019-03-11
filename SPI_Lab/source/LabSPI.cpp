#include "LabSPI.hpp"
inline static LPC_SSP_TypeDef * SSP[3] = {LPC_SSP0, LPC_SSP1, LPC_SSP2};

LabSpi::LabSpi(Peripheral SSPn){
    SpiPort = SSPn;
}
bool LabSpi::Initialize(uint8_t data_size_select, FrameModes format, uint8_t divide, Master_Slave mode)
{
  // Initialize SSP peripheral to master
  switch(SpiPort)
  {
    case 0:
      LPC_SC->PCONP |= (1<<21);
      chip_deselect();
      chip_set();
      break;
    case 1:
      LPC_SC->PCONP |= (1<<10);
      chip_deselect();
      chip_set();
      break;
    case 2: 
      //SSP 2
      LPC_SC -> PCONP |= (1<<20);
      chip_deselect();
      chip_set();
      break;
    
  }
  if( mode == kMaster){
  SSP[SpiPort]->CR0 &= ~(0xFFFF<<0);//Clear first 16 bits
  SSP[SpiPort]->CR0 |= (data_size_select-1); // set bit transfer size
  SSP[SpiPort]->CR0 |= (format << 4); // select frame format
  SSP[SpiPort]->CPSR |= (divide); // Clock Prescale Register defaulted to 2
  SSP[SpiPort]->CR0 |= (1 << 8); // Serial Clock Rate
  SSP[SpiPort]->CR1 &= ~(0b111 << 0); // Clears CR1 and set to master and normal operation
  SSP[SpiPort]->CR1 |= (0b1<<1); // Enable SSP controller
  return true;
  }
  else if (mode == kSlave)
  {
  SSP[SpiPort]->CR0 &= ~(0xFFFF<<0);//Clear first 16 bits
  SSP[SpiPort]->CR0 |= (data_size_select-1); // set bit transfer size
  SSP[SpiPort]->CR0 |= (format << 4); // select frame format
  SSP[SpiPort]->CR0 |= (1<<8);// Serial Clock Rate
  SSP[SpiPort]->CPSR |= (divide); // Clock Prescale Register defaulted to 2
  SSP[SpiPort]->CR1 &= ~(0b111 << 0); // Clears CR1 and sets normal operation
  SSP[SpiPort]->CR1 |= (0b11<<1); // Enable SSP controller and set to slave mode
  return true;
  }
  else 
      return false;
}
void LabSpi::chip_select()
{
    switch(SpiPort)
    {
    case 0:
    {
      LabGPIO CE0(0,16); //CE 
      LabGPIO MCE0(0,1);// Mirror
      CE0.SetAsOutput();
      MCE0.SetAsOutput();
      CE0.SetLow();
      MCE0.SetLow();
      printf("P0_1 is the CE mirror pin\n");
      break;
    }
    case 1:
    {
     LabGPIO CE1(0,6); //CE 
      LabGPIO MCE1(0,25);//Mirror
      CE1.SetAsOutput();
      MCE1.SetAsOutput();
      CE1.SetLow();
      MCE1.SetLow();
      printf("P0_25 is the CE mirror pin\n");
      break;
    }
    case 2: 
    {
      LabGPIO CE2(1,10); //CE 
      LabGPIO MCE2(0,6); // Mirror
      CE2.SetAsOutput();
      MCE2.SetAsOutput();
      CE2.SetLow();
      MCE2.SetLow();
      printf("P4_28 is the CE mirror pin\n");
      break;
    }
    }
    }
    

void LabSpi::chip_deselect(){
    switch(SpiPort)
    {
      
    case 0:
    {
      LabGPIO CE0(0,16); //CE 
      LabGPIO MCE0(0,1);// Mirror
      CE0.SetAsOutput();
      MCE0.SetAsOutput();
      CE0.SetHigh();
      MCE0.SetHigh();
     
      break;
    }
  
    case 1:
    {
      
      LabGPIO CE1(0,6); //CE 
      LabGPIO MCE1(0,25);//Mirror
      CE1.SetAsOutput();
      MCE1.SetAsOutput();
      CE1.SetHigh();
      MCE1.SetHigh();

      break;
    }
  
    case 2: 
    {  //SSP 2
     
      LabGPIO CE2(1,10); //CE 
      LabGPIO MCE2(0,6); // Mirror
      CE2.SetAsOutput();
      MCE2.SetAsOutput();
      CE2.SetHigh();
      MCE2.SetHigh();
      break;
    }
    }  
    }
void LabSpi::chip_set(){
    switch(SpiPort){
    case 0:
    pc.ssp_clk(0,15);
    pc.ssp_miso(0,17);
    pc.ssp_mosi(0,18);
    break;
    case 1:
    pc.ssp_clk(0,7);
    pc.ssp_miso(0,8);
    pc.ssp_mosi(0,9);
    break;
    case 2:
    pc.ssp_clk(1,0);
    pc.ssp_miso(1,4);
    pc.ssp_mosi(1,1);
    break;
    }
}
uint8_t LabSpi::Transfer(uint8_t send){
    if(SSP[SpiPort]->SR & (1<<1))
    {
        SSP[SpiPort]->DR = send;
    }
    else return 0xFF;
    while(SSP[SpiPort]->SR & (1<<4)){}
    if(SSP[SpiPort]->SR &(1<<2))
    {
        return SSP[SpiPort]->DR;
    }
    
    }
void LabSpi::ReadStatus(){
    chip_select();
    Transfer(0x05);
    stat1.byte=Transfer(0xFF);
    stat2.byte=Transfer(0xFF);
    printf("Status: %x %x\n",stat1.byte, stat2.byte);
    chip_deselect();

}

void LabSpi::ReadDevice(){
    uint8_t data[4];
    chip_select();
    Transfer(0x9F);
    for(int i =0; i<4; i++)
    {
    data[i] = (Transfer(0xFF));
    }
    chip_deselect();
    printf("Returned data: %x %x %x %x\n", data[0], data[1], data[2], data[3]);
}
void LabSpi::WriteEnable(){
    chip_select();
    Transfer(0x06);
    chip_deselect();
}
void LabSpi::WriteDisable(){
    chip_select();
    Transfer(0x04);
    chip_deselect();
}
void LabSpi::ReadBytes(){
    if(stat1.RDY)
    printf("Ready/Busy Bit is busy\n");
    else 
    printf("Read/Busy Bit is ready\n");
    if(stat1.WEL)
    printf("Write Enable bit is Enable\n");
    else 
    printf("Write Enable bit is not enabled\n");
    if(stat1.BP0)
    printf("Memory is protected\n");
    else 
    printf("Not protected\n");
    if(stat1.WPP)   
    printf("Write Protect is not asserted\n");
    else 
    printf("Write Protect asserted\n");
    if(stat1.EPE)
    printf("Erase Error dectected\n");
    else
    printf("Operation Successful\n");
    if(stat2.RDY2)
    printf("Ready/Busy 2 is busy\n");
    else 
    printf("Ready/Busy 2 is ready\n");
    if(stat2.RSTE)
    printf("Reset command enabled\n");
    else
    printf("Reset command disabled\n");
}