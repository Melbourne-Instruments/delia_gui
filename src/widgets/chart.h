/**
 *-----------------------------------------------------------------------------
 * Copyright (c) 2023 Melbourne Instruments, Australia
 *-----------------------------------------------------------------------------
 * @file  chart.h
 * @brief Chart class definitions.
 *-----------------------------------------------------------------------------
 */
#ifndef CHART_H
#define CHART_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QPointF>
#include "gui_common.h"

QT_FORWARD_DECLARE_CLASS(QOpenGLShaderProgram)

struct Point
{
	float x;
	float y;
};


// Chart class
class Chart : public QOpenGLWidget, protected QOpenGLFunctions
{
	Q_OBJECT
public:
	// Constructor
	explicit Chart(uint num_points, QWidget *parent = nullptr);
	~Chart();

	// Public functions
	bool shown() const;
	void show();
	void hide();
	void set_colour(QColor colour);
	void refresh_data(const QVector<QPointF>& data);

public slots:
	// Public slot functions
    void cleanup();

protected:
	// Protected functions
    void initializeGL() override;
    void paintGL() override;

private:
	// Private data
    QOpenGLVertexArrayObject _line_vao;
	QOpenGLVertexArrayObject _fill_vao;
    QOpenGLBuffer _line_vbo;
	QOpenGLBuffer _fill_vbo;
    QOpenGLShaderProgram *_program;
	int _colour_loc;
	uint _num_points;
	float *_line_vertices;
	float *_fill_vertices;
	QColor _colour;
};

#endif
