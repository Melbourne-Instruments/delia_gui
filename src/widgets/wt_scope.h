/**
 *-----------------------------------------------------------------------------
 * Copyright (c) 2023 Melbourne Instruments, Australia
 *-----------------------------------------------------------------------------
 * @file  wt_scope.h
 * @brief Wavetable Scope class definitions.
 *-----------------------------------------------------------------------------
 */
#ifndef WT_SCOPE_H
#define WT_SCOPE_H

#include <QTimer>
#include "scope.h"
#include "wt_file.h"

// Wavetable Scope class
class WtScope : public Scope
{
	Q_OBJECT
public:
	// Constructor
	WtScope(QWidget *parent=nullptr);
	~WtScope();

	// Public functions
	void load_wt_file(const std::string& file);
	void unload_wt_file();
	void update_scope();
	void refresh_colour();

private:
	// Private data
	WtFile _wt_file;
	QTimer *_wt_timer;
};

#endif
