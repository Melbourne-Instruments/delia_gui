/**
 *-----------------------------------------------------------------------------
 * Copyright (c) 2023-2024 Melbourne Instruments, Australia
 *-----------------------------------------------------------------------------
 * @file  scope_msg_thread.cpp
 * @brief Scope Message Thread class implementation.
 *-----------------------------------------------------------------------------
 */
#include <mqueue.h>
#include <poll.h>
#include "sound_scope_msg_thread.h"

// Constants
constexpr char MSG_QUEUE_NAME[] = "/delia_samples_msg_queue";
constexpr uint MSG_QUEUE_SIZE   = 1;
constexpr auto POLL_TIMEOUT     = 1;

//----------------------------------------------------------------------------
// SoundScopeMsgThread
//----------------------------------------------------------------------------
SoundScopeMsgThread::SoundScopeMsgThread(SoundScope *scope, QObject *parent) :
    QThread(parent)
{
    // Initialise class variables
    _scope = scope;
    _exit_msgs_thread = false;
}

//----------------------------------------------------------------------------
// ~SoundScopeMsgThread
//----------------------------------------------------------------------------
SoundScopeMsgThread::~SoundScopeMsgThread()
{
    // Stop the thread
    _exit_msgs_thread = true;
    wait();
}

//----------------------------------------------------------------------------
// run
//----------------------------------------------------------------------------
void SoundScopeMsgThread::run()
{
    mq_attr attr;

    // Open the Samples Message Queue (create if it doesn't exist)
    std::memset(&attr, 0, sizeof(attr));
    attr.mq_maxmsg = MSG_QUEUE_SIZE;
    attr.mq_msgsize = sizeof(float) * SCOPE_SAMPLES_MSG_SIZE;
    mqd_t desc = ::mq_open(MSG_QUEUE_NAME, (O_CREAT|O_RDONLY),
                           (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH),
                           &attr);
    if (desc == (mqd_t)-1)
    {
        // Error opening the Samples Message Queue
        MSG("SoundScopeMsgThread: ERROR: Could not open the Samples message queue: " << errno);
        return;
    }

    // Run until the thread is stopped
    while(!_exit_msgs_thread)
    {
        timespec poll_time;
        float msg[SCOPE_SAMPLES_MSG_SIZE];
        std::memset(&msg, 0, sizeof(msg));

        // Wait for Sample events, timeout, or an error
        clock_gettime(CLOCK_REALTIME, &poll_time);
        poll_time.tv_sec += POLL_TIMEOUT;
        int res = ::mq_timedreceive(desc, (char *)&msg, sizeof(msg), NULL, &poll_time);
        if (res == sizeof(msg))
        {
            // Update the data
            _scope->update_scope_data(msg);
        }
        else if (res == -1)
        {
            // If not a timeout
            if (errno != ETIMEDOUT)
            {
                // An error occurred, stop processing the queue
                DEBUG_MSG("SoundScopeMsgThread: Message Queue error: " << errno);
                break;
            }
        }
    }

    // Thread exited
    DEBUG_MSG("SoundScopeMsgThread: thread: EXIT");

    // Close the Samples message queue
    ::mq_close(desc);
    //::mq_unlink(MSG_QUEUE_NAME);
}
