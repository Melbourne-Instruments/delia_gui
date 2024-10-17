/**
 *-----------------------------------------------------------------------------
 * Copyright (c) 2023-2024 Melbourne Instruments, Australia
 *-----------------------------------------------------------------------------
 * @file  msg_box.h
 * @brief Message Box widget class definitions.
 *-----------------------------------------------------------------------------
 */
#ifndef MSG_BOX_H
#define MSG_BOX_H

#include <QTimer>
#include "moniq_label.h"

// Message Box class
class MsgBox: public MoniqLabel
{
	Q_OBJECT
public:
	// Constructors
	explicit MsgBox(QWidget *parent = nullptr);

    // Overriden functions
    void setGeometry(int x, int y, int w, int h);
    void hide();

	// Public functions
	void show_msg(const QString& line_1, const QString& line_2, const QString& line_3, bool show_hourglass);
	void refresh_colour();

private:
	// Private data
    MoniqLabel *_border;
    MoniqLabel *_line_1;
    MoniqLabel *_line_2;
    MoniqLabel *_line_3;
    MoniqLabel *_hourglass;
    uint _hourglass_pixmap_index = 0;
    std::vector<QPixmap> _hourglass_pixmaps;
    QTimer *_hourglass_timer;

	// Private functions
	void _update_hourglass();	
};

#endif
