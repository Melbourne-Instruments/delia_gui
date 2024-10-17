/**
 *-----------------------------------------------------------------------------
 * Copyright (c) 2023 Melbourne Instruments, Australia
 *-----------------------------------------------------------------------------
 * @file  vcf_cutoff_chart.cpp
 * @brief VCF Cutoff Chart class implementation.
 *-----------------------------------------------------------------------------
 */
#include <math.h>
#include "vcf_cutoff_chart.h"
#include "gui_common.h"
#include "utils.h"

// Constants
constexpr uint VCF_CUTOFF_CHART_NUM_DATA_POINTS = 4;
constexpr float CHART_MAX_X_VALUE               = 2.0f;

//----------------------------------------------------------------------------
// VcfCutoffChart
//----------------------------------------------------------------------------
VcfCutoffChart::VcfCutoffChart(QWidget *parent) : Chart(VCF_CUTOFF_CHART_NUM_DATA_POINTS, parent)
{
    // Set the chart colour
    refresh_colour();
    hide();
}

//----------------------------------------------------------------------------
// ~VcfCutoffChart
//----------------------------------------------------------------------------
VcfCutoffChart::~VcfCutoffChart()
{
    // Nothing specific to do
}

//----------------------------------------------------------------------------
// update_chart
//----------------------------------------------------------------------------
void VcfCutoffChart::update_chart(float hp_filter, float lp_filter)
{
    QVector<QPointF> data(VCF_CUTOFF_CHART_NUM_DATA_POINTS);
    auto itr = data.begin();
    float x_offset;
    float y_offset;

    // Scale the cutoff values to a chart value
    lp_filter = std::clamp(lp_filter * CHART_MAX_X_VALUE, 0.0f, CHART_MAX_X_VALUE);
    hp_filter = std::clamp(hp_filter * CHART_MAX_X_VALUE, 0.0f, CHART_MAX_X_VALUE);

    // Calculate the HP and LP filter start points
    auto hp_x = -2.95f + (2.0f * hp_filter);
    auto lp_x = -1.05f + (2.0f * lp_filter);

    // Calculate the x and y intersection points
    auto x = ((lp_x - 1.0f) + (hp_x + 1.0f)) / 2;
    auto y = x - (hp_x + 1.0f);

    // HP filter start point
    x_offset = hp_x;
    y_offset = x_offset - (hp_x + 1.0f);
    *itr++ = QPointF(x_offset, y_offset);

    // HP filter end point
    x_offset = y < 1.0f ? x : hp_x + 2.0f;
    y_offset = x_offset - (hp_x + 1.0f);
    *itr++ = QPointF(x_offset, y_offset);

    // LP filter start point
    x_offset = y < 1.0f ? x : lp_x - 2.0f;
    y_offset = -x_offset + (lp_x - 1.0f);
    *itr++ = QPointF(x_offset, y_offset);

    // LP filter end point
    x_offset = lp_x;
    y_offset = -x_offset + (lp_x - 1.0f);
    *itr++ = QPointF(x_offset, y_offset);

    // Refresh the chart
    refresh_data(data);
}

//----------------------------------------------------------------------------
// refresh_colour
//----------------------------------------------------------------------------
void VcfCutoffChart::refresh_colour()
{
    // Refresh the chart colour
    set_colour(utils::get_system_colour());
}
