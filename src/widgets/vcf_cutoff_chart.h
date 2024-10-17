/**
 *-----------------------------------------------------------------------------
 * Copyright (c) 2023 Melbourne Instruments, Australia
 *-----------------------------------------------------------------------------
 * @file  vcf_cutoff_chart.h
 * @brief VCF Cutoff Chart class definitions.
 *-----------------------------------------------------------------------------
 */
#ifndef VCF_CUTOFF_CHART_H
#define VCF_CUTOFF_CHART_H

#include "chart.h"

// VCF Cutoff Chart class
class VcfCutoffChart : public Chart
{
	Q_OBJECT
public:
	// Constructor
	VcfCutoffChart(QWidget *parent = nullptr);
	~VcfCutoffChart();

	// Public functions
	void update_chart(float hp_filter, float lp_filter);
	void refresh_colour();
};

#endif	// VCF_CUTOFF_CHART_H
