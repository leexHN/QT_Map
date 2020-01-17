//
// Created by lix on 2020/1/9.
//

#include "signals.h"

boost::signals2::signal<void(int,int)> G_SetRowColSig;
boost::signals2::signal<void(bool)> G_RunSig;
boost::signals2::signal<void(bool)> G_IsShowStackSig;
boost::signals2::signal<void(int)> G_SetDelayTimeSig;
boost::signals2::signal<void(int)> G_SetMazeGeneratorSig;