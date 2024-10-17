/**
 *-----------------------------------------------------------------------------
 * Copyright (c) 2023 Melbourne Instruments, Australia
 *-----------------------------------------------------------------------------
 * @file  msg_popup.h
 * @brief Message Popup widget class definitions.
 *-----------------------------------------------------------------------------
 */
#ifndef INFO_POPUP_H
#define INFO_POPUP_H

#include <QTimer>
#include "moniq_label.h"

// Message Popup class
class MsgPopup: public MoniqLabel
{
	Q_OBJECT
public:
	// Constructors
	explicit MsgPopup(QWidget *parent = nullptr);

	// Public functions
	void show_msg(const QString& line_1, const QString& line_2);
	void refresh_colour();

private:
	// Private data
    MoniqLabel *_line_1;
    MoniqLabel *_line_2;
    QTimer _timer;

	// Private functions
	void _timeout();	
};

#endif
