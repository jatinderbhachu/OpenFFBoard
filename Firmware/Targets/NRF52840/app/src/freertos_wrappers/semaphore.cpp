#include "semaphore.hpp"

namespace cpp_freertos
{

    bool Semaphore::Take(TickType_t Timeout)
    {
        return 0 == k_sem_take(&handle, K_TIMEOUT_ABS_TICKS(Timeout));
    }

    bool Semaphore::Give()
    {
        k_sem_give(&handle);
        return true;
    }

    bool Semaphore::TakeFromISR(BaseType_t *pxHigherPriorityTaskWoken)
    {
        return Take();
    }

    bool Semaphore::GiveFromISR(BaseType_t *pxHigherPriorityTaskWoken)
    {
        return Give();
    }

    Semaphore::Semaphore()
    {
    }

    Semaphore::~Semaphore()
    {
    }

    BinarySemaphore::BinarySemaphore(bool set)
    {
        k_sem_init(&handle, set, 1);
    }

    CountingSemaphore::CountingSemaphore(UBaseType_t maxCount, UBaseType_t initialCount)
    {
        k_sem_init(&handle, initialCount, maxCount);
    }
}