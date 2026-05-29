#include "entityIdHolder.h"
#include <asserts.h>

std::uint64_t EntityIdHolder::getEntityIDAndIncrement()
{
    std::uint64_t id = idCounter;
    idCounter++;

    permaAssertComment(id < UINT64_MAX - 1, "We ran out of ids somehow.....This should impossiable. You somehow did it good job.");

    return id;
}
