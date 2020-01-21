//
// Created by lix on 2020/1/9.
//

#pragma once

#include <boost/signals2.hpp>

extern boost::signals2::signal<void(int,int)> G_SetRowColSig;
extern boost::signals2::signal<void(bool)> G_RunSig;
extern boost::signals2::signal<void(bool)> G_IsShowStackSig;
extern boost::signals2::signal<void(int)> G_SetDelayTimeSig;
extern boost::signals2::signal<void(int)> G_SetMazeGeneratorSig;
extern boost::signals2::signal<void(bool)>G_IsAnimation;