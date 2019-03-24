#pragma once

#include <cstdio>
#include <iterator>

#include "L3_Application/commandline.hpp"
#include "utility/log.hpp"

namespace command
{
    inline Command Sus_Producer("Suspend Producer", "Suspend Producer Task", vTaskSuspend(vProducerTask));
    inline Command Sus_Consumer("Suspend Consumer", "Suspend Consumer Task", vTaskSuspend(vConsumerTask));
    inline Command Res_Producer("Resume Producer", "Resume Producer Task", vTaskResume(vProducerTask));
    inline Command Res_Consumer("Resume Consumer", "Resume Producer Task", vTaskResume(vConsumerTask));
}

inline void AddTaskCommands(CommandLineType<list> & command_line)
{
  command_line.AddCommand(&command::Sus_Producer);
  command_line.AddCommand(&command::Sus_Consumer);
  command_line.AddCommand(&command::Res_Consumer);
  command_line.AddCommand(&command::Res_Producer);
}