#pragma once

enum class UIType {
    NonDiegetic, // not in game narrative, not in game world
    Diegetic,    // in game narrative, in game world
    Spatial,     // not in game narrative, in game world
    Meta         // in game narative, not in game world
};