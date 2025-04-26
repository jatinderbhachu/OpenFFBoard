#pragma once
#include "mutex.hpp"
#include "semaphore.hpp"
#include "condition_variable.hpp"
#include <string.h>
#include "common.hpp"

namespace cpp_freertos
{
    class Thread
    {

    public:
        Thread(const std::string Name,
               uint16_t StackDepth,
               UBaseType_t Priority);
        Thread(uint16_t StackDepth,
               UBaseType_t Priority);
        void Start();

        virtual ~Thread();

        static inline void Yield()
        {
            k_yield();
        }

        inline void Suspend()
        {
            k_thread_suspend(thread_id);
        }

        inline void Resume()
        {
            k_thread_resume(thread_id);
        }
        inline void ResumeFromISR()
        {
            Resume();
        }

        inline void Notify()
        {
            k_sem_give(&notify_sem);
        }

        /**
         *  Notify a specific thread from ISR context.
         */
        inline void NotifyFromISR()
        {
            Notify();
        }

        inline uint32_t WaitForNotification(TickType_t Timeout = portMAX_DELAY)
        {
            unsigned int count = 0;

            if (k_sem_take(&notify_sem, K_TIMEOUT_ABS_TICKS(Timeout)) != 0)
                return 0;

            count = 1;

            while (k_sem_take(&notify_sem, K_NO_WAIT) == 0)
            {
                count++;
            }

            return count;
        }

        void inline Delay(const TickType_t Delay)
        {
            k_sleep(K_TIMEOUT_ABS_TICKS(Delay));
        }

    protected:
        virtual void Run() = 0;
        virtual void Cleanup();

    private:
        static void ThreadEntryPoint(void *p1, void *p2, void *p3);
        struct k_sem notify_sem;

        struct k_thread *handle;
        k_tid_t thread_id;
        k_thread_stack_t *stack_memory = nullptr;

        const std::string name;
        const uint16_t stack_depth;

        UBaseType_t priority;
        bool started = false;
    };

}