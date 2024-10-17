/**
 *-----------------------------------------------------------------------------
 * Copyright (c) 2023 Melbourne Instruments, Australia
 *-----------------------------------------------------------------------------
 * @file  background.h
 * @brief Background class definitions.
 *-----------------------------------------------------------------------------
 */
#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <QLabel>

// Background class
class Background : public QLabel
{
	Q_OBJECT
public:
	// Constructors
	explicit Background(QWidget *parent = nullptr);

	// Public functions
	void set_image(const std::string &path);
	void clear_image();
};

#endif
