/**
 *-----------------------------------------------------------------------------
 * Copyright (c) 2023 Melbourne Instruments, Australia
 *-----------------------------------------------------------------------------
 * @file  scope_msg_thread.h
 * @brief Scope Message Thread class definitions.
 *-----------------------------------------------------------------------------
 */
#ifndef SCOPE_MSG_THREAD_H
#define SCOPE_MSG_THREAD_H

#include <atomic>
#include <QThread>
#include "sound_scope.h"

// Sound Scope Message Thread class
class SoundScopeMsgThread : public QThread
{
	Q_OBJECT
public:
    SoundScopeMsgThread(SoundScope *scope, QObject *parent);
    ~SoundScopeMsgThread();
    void run();

private:
    SoundScope *_scope;
    std::atomic<bool> _exit_msgs_thread;
};

#endif
