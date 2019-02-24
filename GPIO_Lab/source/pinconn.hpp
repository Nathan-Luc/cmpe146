#pragma once

#include <cstdio>

#include "L0_LowLevel/LPC40xx.h"


class pinconn
{   void inactive(uint8_t port, uint8_t pin);
    void pullup(uint8_t port, uint8_t pin);
    void pulldown(uint8_t port, uint8_t pin);
    void repeater(uint8_t port, uint8_t pin);
}