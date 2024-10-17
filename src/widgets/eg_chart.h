/**
 *-----------------------------------------------------------------------------
 * Copyright (c) 2023 Melbourne Instruments, Australia
 *-----------------------------------------------------------------------------
 * @file  eg_chart.h
 * @brief Envelope Generator (EG) Chart class definitions.
 *-----------------------------------------------------------------------------
 */
#ifndef EG_CHART_H
#define EG_CHART_H

#include "chart.h"

// EG Chart class
class EgChart : public Chart
{
	Q_OBJECT
public:
	// Constructor
	EgChart(QWidget *parent = nullptr);
	~EgChart();

	// Public functions
	void update_chart(float attack, float decay, float sustain, float release, float level);
	void refresh_colour();

private:
	// Private functions
	inline float _exp_curve_fn(float x, float exp_constant);
};

#endif
