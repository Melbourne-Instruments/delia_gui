/**
 *-----------------------------------------------------------------------------
 * Copyright (c) 2023 Melbourne Instruments, Australia
 *-----------------------------------------------------------------------------
 * @file  wt_scope.cpp
 * @brief Wavetable Scope class implementation.
 *-----------------------------------------------------------------------------
 */
#include "wt_scope.h"
#include "wt_file.h"
#include "gui_common.h"
#include "utils.h"

//----------------------------------------------------------------------------
// WtScope
//----------------------------------------------------------------------------
WtScope::WtScope(QWidget *parent) : Scope(WtFile::NumSamplesPerWave(), parent)
{
    // Create the WT chart timer
    _wt_timer = new QTimer(this);
    connect(_wt_timer, &QTimer::timeout, this, &WtScope::update_scope);
    
    // Set the scope colour
    refresh_colour();
    hide();
}

//----------------------------------------------------------------------------
// ~WtScope
//----------------------------------------------------------------------------
WtScope::~WtScope()
{
    // Nothing specific to do
}

//----------------------------------------------------------------------------
// load_wt_file
//----------------------------------------------------------------------------
void WtScope::load_wt_file(const std::string& file)
{
    // Stop the WT timer
    _wt_timer->stop();

    // Load the WT file
    if (_wt_file.load(file)) {
        // Start the WT timer and show the WT chart
        _wt_timer->start(WT_CHART_REFRESH_RATE);
    }
    else {
        // The WT file could not be loaded, so just display a line at 0.0
        show_zero_scope();
    }
}

//----------------------------------------------------------------------------
// unload_wt_file
//----------------------------------------------------------------------------
void WtScope::unload_wt_file()
{
    // Stop the WT timer and unload the file
    _wt_timer->stop();
    _wt_file.unload();
    clear_scope();
}

//----------------------------------------------------------------------------
// update_scope
//----------------------------------------------------------------------------
void WtScope::update_scope()
{
    QVector<QPointF> data;

    // Get the next wave samples to display
    auto samples = _wt_file.next_wave_samples();
    if (samples.size() > 0) {
        // Add the points to the data
        for (uint i=0; i<samples.size(); i++) {
            auto x = -1.0f + ((qreal(i) / samples.size()) * 2);
            data.append(QPointF(x, samples[i]));
        }

        // Refresh the scope
        refresh_data(data);
    }
}

//----------------------------------------------------------------------------
// refresh_colour
//----------------------------------------------------------------------------
void WtScope::refresh_colour()
{
    // Refresh the scope colour
    set_colour(utils::get_system_colour());
}
