/**
 *-----------------------------------------------------------------------------
 * Copyright (c) 2023 Melbourne Instruments, Australia
 *-----------------------------------------------------------------------------
 * @file  scope.h
 * @brief Scope class definitions.
 *-----------------------------------------------------------------------------
 */
#ifndef SCOPE_H
#define SCOPE_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QPointF>
#include "gui_common.h"

QT_FORWARD_DECLARE_CLASS(QOpenGLShaderProgram)

// Scope Display Mode
enum class ScopeDisplayMode
{
	FOREGROUND,
	BACKGROUND
};

// Scope class
class Scope : public QOpenGLWidget, protected QOpenGLFunctions
{
	Q_OBJECT
public:
	// Constructor
	explicit Scope(uint num_samples, QWidget *parent = nullptr);
	~Scope();

	// Public functions
	bool shown() const;
	ScopeDisplayMode display_mode() const;
	void show();
	void show(ScopeDisplayMode display_mode);
	void hide(bool reset_display_mode=true);
	void set_colour(QColor colour);
	void set_pen_width(uint width);
	void refresh_data(const QVector<QPointF>& data);
	void show_zero_scope();
	void clear_scope();

public slots:
	// Public slot functions
    void cleanup();

protected:
	// Protected functions
    void initializeGL() override;
    void paintGL() override;

private:
	// Private data
    QOpenGLVertexArrayObject _vao;
    QOpenGLBuffer _vbo;
    QOpenGLShaderProgram *_program;
	int _colour_loc;
	uint _num_samples;
	float *_vertices;
	QColor _colour;
	float _alpha;
	uint _pen_width;
};

#endif
