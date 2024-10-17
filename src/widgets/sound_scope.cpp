/**
 *-----------------------------------------------------------------------------
 * Copyright (c) 2023-2024 Melbourne Instruments, Australia
 *-----------------------------------------------------------------------------
 * @file  sound_scope.cpp
 * @brief Sound Scope class implementation.
 *-----------------------------------------------------------------------------
 */

#include <cmath>
#include "sound_scope.h"
#include "gui_common.h"
#include "utils.h"
#include "timer.h"

// Constants
constexpr uint REFRESH_RATE_MS        = ((1.f / 60.f) * 1000.f);
constexpr uint SCOPE_IDLE_FRAME_COUNT = 60 * 3;
const float PI                        = std::acos(-1);
const float ROTATE_SCOPE_XY_ANGLE     = 45;
const float ROTATE_SCOPE_XY_SIN       = std::sin((ROTATE_SCOPE_XY_ANGLE * PI) / 180.f);
const float ROTATE_SCOPE_XY_COS       = std::cos((ROTATE_SCOPE_XY_ANGLE * PI) / 180.f);

//----------------------------------------------------------------------------
// SoundScope
//----------------------------------------------------------------------------
SoundScope::SoundScope(SoundScopeMode& scope_mode, QWidget *parent) : 
    Scope(SCOPE_NUM_SAMPLES, parent),
    _scope_mode(scope_mode)
{
    // Initialise class variables
    _data1.clear();
    _data2.clear();
    for (uint i=0; i<SCOPE_NUM_SAMPLES; i++) {
        float x = -1.0f + ((qreal(i) / SCOPE_NUM_SAMPLES) * 2);
        _data1.append(QPointF(x, 0.0f));
        _data2.append(QPointF(x, 0.0f));
    }     
    _data = &_data1;
    _scope_idle = false;
    _scope_idle_threshold = 0.0f;
    _scope_idle_frame_count = 0;

    // Set the initial colour
    refresh_colour();
    hide();
}

//----------------------------------------------------------------------------
// ~SoundScope
//----------------------------------------------------------------------------
SoundScope::~SoundScope()
{
    // Nothing specific to do
}

//----------------------------------------------------------------------------
// start
//----------------------------------------------------------------------------
void SoundScope::start()
{
    // Calculate the idle threshold (+/- 1px)
    _scope_idle_threshold = 1.0f / (height() / 2);

    // Start the scope refresh timer
    _scope_refresh_timer.setInterval(REFRESH_RATE_MS);
    _scope_refresh_timer.setSingleShot(false);
    QObject::connect(&_scope_refresh_timer, &QTimer::timeout, this, &SoundScope::refresh_scope);
    _scope_refresh_timer.start();
}

//----------------------------------------------------------------------------
// update_scope_data
//----------------------------------------------------------------------------
void SoundScope::update_scope_data(float *samples)
{
    // Get the alternate data to update and clear it
    QVector<QPointF>& data = (_data == &_data1) ? _data2 : _data1;
    data.clear();

    // If there is a scope mode
    if (_scope_mode != SoundScopeMode::SCOPE_MODE_OFF) {
        bool scope_idle = display_mode() == ScopeDisplayMode::BACKGROUND;

        // Add the points to the data
        for (uint i=0; i<SCOPE_NUM_SAMPLES; i++) {
            QPointF point;

            // Get the L/R samples
            auto l_sample = *samples++;
            auto r_sample = *samples++;

            // If the scope samples are idle so far
            if (scope_idle) {
                // Check if these L and R samples are no longer idle
                if ((l_sample < -(_scope_idle_threshold)) || (l_sample > _scope_idle_threshold) ||
                    (r_sample < -(_scope_idle_threshold)) || (r_sample > _scope_idle_threshold)) {
                    // No longer idle - make sure the scope is shown if it is in
                    // background
                    if (display_mode() == ScopeDisplayMode::BACKGROUND) {
                        show();
                    }
                    _scope_idle_frame_count = 0;
                    scope_idle = false;
                }
            }

            // Check the scope mode
            if (_scope_mode == SoundScopeMode::SCOPE_MODE_OSC) {
                // Oscillator - add the scope point
                qreal x = ((qreal(i) / qreal(SCOPE_NUM_SAMPLES)) * 2) - 1.0;
                qreal y = l_sample + r_sample;
                point = QPointF(x, y);
            }
            else {
                // X/Y - add the scope point (rotated)
                point = _rotate_point(l_sample, r_sample);             
            }
            data.append(point);
        }
        _scope_idle = scope_idle;
    }

    // Set the data pointer to the updated data
    _data = &data;
}

//----------------------------------------------------------------------------
// refresh_scope
//----------------------------------------------------------------------------
void SoundScope::refresh_scope()
{
    // If the scope is currently shown in the background and these samples were idle
    if (_scope_idle && shown() && (display_mode() == ScopeDisplayMode::BACKGROUND)) {
        // Increment the idle frame count, and if it exceeds the idle
        // threshold then hide the scope (don't reset the display mode when hiding)
        _scope_idle_frame_count++;
        if (_scope_idle_frame_count >= SCOPE_IDLE_FRAME_COUNT) {
            hide(false);
            _scope_idle_frame_count = 0;
        }
    }

    // Refresh the scope
    refresh_data(*_data);
}

//----------------------------------------------------------------------------
// refresh_colour
//----------------------------------------------------------------------------
void SoundScope::refresh_colour()
{
    // Refresh the scope colour
    set_colour(utils::get_system_colour());
}

//----------------------------------------------------------------------------
// _rotate_point
//----------------------------------------------------------------------------
QPointF SoundScope::_rotate_point(float x, float y)
{
    // Rotate the point
    qreal rotated_x = (x * ROTATE_SCOPE_XY_COS) - (y * ROTATE_SCOPE_XY_SIN);
    qreal rotated_y = (x * ROTATE_SCOPE_XY_SIN) + (y * ROTATE_SCOPE_XY_COS);
    return QPointF(rotated_x, rotated_y);
}
