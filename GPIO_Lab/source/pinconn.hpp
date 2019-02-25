#pragma once

#include <cstdint>
#include "L0_LowLevel/LPC40xx.h"



class pinconn
{  
public:
    void pc_inactive(uint8_t port, uint8_t pin);
    void pc_pullup(uint8_t port, uint8_t pin);
    void pc_pulldown(uint8_t port, uint8_t pin);
    void pc_repeater(uint8_t port, uint8_t pin);
    
};
