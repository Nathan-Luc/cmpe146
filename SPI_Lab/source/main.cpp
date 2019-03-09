#include <cstdint> 
#include "L0_LowLevel/LPC40xx.h"
#include <project_config.hpp>
#include "L0_LowLevel/interrupt.hpp"
#include <cstdint>
#include <iterator>
#include "utility/log.hpp"
#include "utility/time.hpp"
#include "LabGPIO.hpp"
  LabGPIO P1_10(1,10); //CE
  LabGPIO P1_04(1,4); // MISO
  LabGPIO P1_00 (1,0); //SCLK
  LabGPIO P1_01(1,1); //MOSI
void SpiInitialize(void)
{
  // Initialize SSP peripheral 2
  LPC_SC -> PCONP |= (1<<20);
  P1_10.SetLow();
  P1_04.SetAsInput();
  P1_00.SetAsOutput();
  P1_01.SetAsOutput();
  LPC_SSP2->CR0 &= ~(0xFF<<0);
  LPC_SSP2->CR0 |= (15);
  LPC_SSP2->CR1 &= ~(0b111 << 0);
  LPC_SSP2->CR1 |= (0b1<<1);
}

uint8_t SpiTransfer(uint8_t data_out)
{
  // Send data_out and retrieve returned byte
    if(LPC_SSP2->SR & (1<<1))
    {
        LPC_SSP2->DR = data_out;
    }
    else return 0xFF;
    while(LPC_SSP2->SR & (1<<4)){}
    if(LPC_SSP2->SR &(1<<2))
    {
        return LPC_SSP2->DR;
    }
    
}

// WARNING: This is just a sample; you will have to fill in plenty of your own code as per requirements
void ReadSignature()
{
  uint8_t data[2];
  
  // The simplest test is to try to read the signature of the Adesto flash and print it out
  //AdestoSelect();
  P1_10.SetLow();
    // TODO: Find the correct commands you need to send to the Adesto flash device 
    //       to retrieve its device signature.
    SpiTransfer(0x9F);
    data[0] = SpiTransfer(0x40); 
    data[1] = SpiTransfer(0x00);
  //AdestoDeselect();
  P1_10.SetHigh();
  
  printf("Returned data: %x %x\n", data[0], data[1]);
}


int main() 
{ 
  LOG_INFO("Start");
  SpiInitialize();
  ReadSignature();
  
}
