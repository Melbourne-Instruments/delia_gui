/**
 *-----------------------------------------------------------------------------
 * Copyright (c) 2023-2024 Melbourne Instruments, Australia
 *-----------------------------------------------------------------------------
 * @file  sound_scope.h
 * @brief Sound Scope class definitions.
 *-----------------------------------------------------------------------------
 */
#ifndef SOUND_SCOPE_H
#define SOUND_SCOPE_H

#include <QTimer>
#include "gui_msg.h"
#include "scope.h"
#include "gui_common.h"

// Sound Scope class
class SoundScope : public Scope
{
    Q_OBJECT
public:
    SoundScope(SoundScopeMode& scope_mode, QWidget *parent=nullptr);
    ~SoundScope();

    void start();
    void update_scope_data(float *samples);
    void refresh_scope();
    void refresh_colour();

private:
    // Private data
    SoundScopeMode& _scope_mode;
    QVector<QPointF> _data1;
    QVector<QPointF> _data2;
    QVector<QPointF> *_data;
    QTimer _scope_refresh_timer;
    bool _scope_idle;
    float _scope_idle_threshold;
    uint _scope_idle_frame_count;

    // Private functions
    QPointF _rotate_point(float x, float y);
};

#endif
