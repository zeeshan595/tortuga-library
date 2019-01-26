#ifndef _QUEUE_FAMILY_INDICES
#define _QUEUE_FAMILY_INDICES

#include "../Core.h"

struct QueueFamilyIndices
{
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    bool isComplete()
    {
        return graphicsFamily.has_value() && presentFamily.has_value();
    }

    std::set<uint32_t> GetFamilies()
    {
        return {graphicsFamily.value(),
                presentFamily.value()};
    }
};

#endif