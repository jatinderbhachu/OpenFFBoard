#include "thread.hpp"
#include <cassert>

namespace cpp_freertos
{
    Thread::Thread(const std::string Name,
                   uint16_t StackDepth,
                   //    UBaseType_t Priority) : name(Name), stack_depth((StackDepth)), priority(Priority)
                   //   UBaseType_t Priority) : name(Name), stack_depth(K_THREAD_STACK_LEN(StackDepth)), priority(Priority)
                   UBaseType_t Priority) : name(Name), stack_depth(K_KERNEL_STACK_LEN(StackDepth * 4)), priority(57 - Priority)
    {
        k_thread_system_pool_assign(k_current_get());

        // stack_memory = k_thread_stack_alloc(stack_depth, K_USER);
        stack_memory = k_thread_stack_alloc(stack_depth, 0);
        printf("%s %s memory size %u priority %d stack_memory=%p\n", __FUNCTION__, Name.c_str(), stack_depth, priority, stack_memory);

        k_sem_init(&notify_sem, 0, UINT_MAX); // count starts at 0
    }

    Thread::Thread(uint16_t StackDepth,
                   UBaseType_t Priority) : Thread("", StackDepth, Priority)
    {
    }

    Thread::~Thread()
    {
        k_thread_stack_free(stack_memory);
    }

    void Thread::Start()
    {
        if (started)
        {
            return;
        }

        handle = (struct k_thread *)k_object_alloc(K_OBJ_THREAD);
        thread_id = k_thread_create(handle,
                                    stack_memory,
                                    stack_depth,
                                    Thread::ThreadEntryPoint,
                                    this, nullptr, nullptr,
                                    // priority, K_USER | K_INHERIT_PERMS, K_NO_WAIT);
                                    priority, 0, K_NO_WAIT);
        k_thread_name_set (thread_id, name.c_str());
        started = true;
    }

    void Thread::Cleanup()
    {
    }

    void Thread::ThreadEntryPoint(void *p1, void *p2, void *p3)
    {
        Thread *thread = static_cast<Thread *>(p1);
        thread->Run();
    }

}