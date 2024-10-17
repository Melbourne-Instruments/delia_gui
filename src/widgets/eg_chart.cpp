/**
 *-----------------------------------------------------------------------------
 * Copyright (c) 2023-2024 Melbourne Instruments, Australia
 *-----------------------------------------------------------------------------
 * @file  eg_chart.cpp
 * @brief Envelope Generator (EG) Chart class implementation.
 *-----------------------------------------------------------------------------
 */
#include <math.h>
#include "eg_chart.h"
#include "gui_common.h"
#include "utils.h"

// Constants
constexpr uint NUM_CURVE_DATA_POINTS      = 20;
constexpr uint ADSR_CHART_NUM_DATA_POINTS = ((NUM_CURVE_DATA_POINTS * 3) + 4);
constexpr float CHART_MAX_X_VALUE         = 2.0f;
constexpr float CHART_MAX_Y_VALUE         = 2.0f;
constexpr float ATTACK_EXP_CONSTANT       = -2.0f;
constexpr float DECAY_EXP_CONSTANT        = 5.0f;
constexpr float RELEASE_EXP_CONSTANT      = 5.0f;

//----------------------------------------------------------------------------
// EgChart
//----------------------------------------------------------------------------
EgChart::EgChart(QWidget *parent) : Chart(ADSR_CHART_NUM_DATA_POINTS, parent)
{
    // Set the chart colour
    refresh_colour();
    hide();
}

//----------------------------------------------------------------------------
// ~EgChart
//----------------------------------------------------------------------------
EgChart::~EgChart()
{
    // Nothing specific to do
}

//----------------------------------------------------------------------------
// update_chart
//----------------------------------------------------------------------------
void EgChart::update_chart(float attack, float decay, float sustain, float release, float level)
{
    QVector<QPointF> data(ADSR_CHART_NUM_DATA_POINTS);
    auto itr = data.begin();
    float x_offset;
    float exp_x;
    constexpr float EXP_X_INC = 1.0f / NUM_CURVE_DATA_POINTS;
    constexpr float PLOT_X_INC = 2.0f / NUM_CURVE_DATA_POINTS;

    // Scale the ADSR envelope values to a chart value
    level = std::clamp(level * CHART_MAX_Y_VALUE, 0.0f, CHART_MAX_Y_VALUE);
    attack = std::clamp(attack * CHART_MAX_X_VALUE, 0.0f, CHART_MAX_X_VALUE);
    decay = std::clamp(decay * CHART_MAX_X_VALUE, 0.0f, CHART_MAX_X_VALUE);
    sustain = std::clamp(sustain * level, 0.0f, CHART_MAX_Y_VALUE);
    release = std::clamp(release * CHART_MAX_X_VALUE, 0.0f, CHART_MAX_X_VALUE);

    // Calculate the sustain duration
    float sustain_dur = (2.0f / 3) - (decay / 3);

    // Centre the ADSR envelope
    x_offset = -1.0f + (2.0f - (attack / 3) - (decay / 3) - sustain_dur - (release / 3)) / 2;

    // Create the plot data points

    // Draw the Attack from 0 to the max level
    // If the attack is greater than zero
    if (attack > 0) {
        // Plot the attack curve
        float exp_x = 0;
        auto plot_x = x_offset;
        auto plot_x_inc = (attack / 3) / (float)NUM_CURVE_DATA_POINTS;
        for (uint i=0; i<NUM_CURVE_DATA_POINTS; i++ ) {
            float val = _exp_curve_fn(exp_x, ATTACK_EXP_CONSTANT);
            *itr++ = QPointF(plot_x, -1.0f + (val * level));
            exp_x += EXP_X_INC;
            plot_x += plot_x_inc;
        }
    }
    else {
        // No attack, so just draw a vertical line to the max level
        // Add all data points as we need to have the same number of points for the chart
        *itr++ = QPointF(x_offset, -1.0f);
        for (uint i=0; i<(NUM_CURVE_DATA_POINTS-1); i++ ) {
            *itr++ = QPointF(x_offset, -1.0f + level);
        }
    }
    x_offset += attack / 3;
    *itr++ = QPointF(x_offset, -1.0f + level);   

    // Draw the Decay from the Attack to the Sustain level
    // If the Sustain is less than the maximum level
    if (sustain < level) {
        // If a decay has been specified
        if (decay > 0) {
            // Plot the Decay curve from the end of the Attack above
            float exp_x = -1.0f;
            auto plot_x = x_offset;
            auto plot_x_inc = (decay / 3) / (float)NUM_CURVE_DATA_POINTS;
            for (uint i=0; i<NUM_CURVE_DATA_POINTS; i++ ) {
                float val = _exp_curve_fn(exp_x, DECAY_EXP_CONSTANT);
                *itr++ = QPointF(plot_x, -1.0f + ((level - sustain) * val) + sustain);
                exp_x += EXP_X_INC;
                plot_x += plot_x_inc;
            }
        }
        else {
            // No Decay, so just draw a vertical line to zero
            // Add all data points as we need to have the same number of points for the chart
            for (uint i=0; i<NUM_CURVE_DATA_POINTS; i++ ) {
                *itr++ = QPointF(x_offset, -1.0f + sustain);
            }
        }
    }
    else {
        // The Sustain value is max, so just draw a horizontal line based on the Decay value
        // Add all data points as we need to have the same number of points for the chart
        auto plot_x_inc = (decay / 3) / (float)NUM_CURVE_DATA_POINTS;
        auto plot_x = x_offset;
        for (uint i=0; i<NUM_CURVE_DATA_POINTS; i++ ) {
            *itr++ = QPointF(plot_x, -1.0 + level);
            plot_x += plot_x_inc;
        }
    }
    x_offset += decay / 3;
    *itr++ = QPointF(x_offset, -1.0f + sustain);

    // Draw the Sustain
    x_offset += sustain_dur;
    *itr++ = QPointF(x_offset, -1.0f + sustain);

    // Draw the Release from the Sustain to 0
    // If the Release is greater than zero
    if (release > 0) {
        // Plot the Release curve from the end of the Sustain above
        float exp_x = -1.0f;
        auto plot_x = x_offset;
        auto plot_x_inc = (release / 3) / (float)NUM_CURVE_DATA_POINTS;
        for (uint i=0; i<NUM_CURVE_DATA_POINTS; i++ ) {
            float val = _exp_curve_fn(exp_x, DECAY_EXP_CONSTANT);
            *itr++ = QPointF(plot_x, -1.0f + (sustain * val));
            exp_x += EXP_X_INC;
            plot_x += plot_x_inc;
        }
    }
    else {
        // No Release, so just draw a vertical line to -1.0
        // Add all data points as we need to have the same number of points for the chart
        for (uint i=0; i<NUM_CURVE_DATA_POINTS; i++ ) {
            *itr++ = QPointF(x_offset, -1.0f);
        }
    }
    x_offset += release / 3;
    *itr++ = QPointF(x_offset, -1.0f);

    // Refresh the chart
    refresh_data(data);
}

//----------------------------------------------------------------------------
// refresh_colour
//----------------------------------------------------------------------------
void EgChart::refresh_colour()
{
    // Refresh the chart colour
    set_colour(utils::get_system_colour());
}

//----------------------------------------------------------------------------
// _exp_curve_fn
//----------------------------------------------------------------------------
inline float EgChart::_exp_curve_fn(float x, float exp_constant)
{
    // Return the calculated data point for the exp curve
    return (1.0f - std::exp(std::abs(x)*exp_constant)) / (1.0f - std::exp(exp_constant));    
}
