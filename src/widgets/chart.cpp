/**
 *-----------------------------------------------------------------------------
 * Copyright (c) 2023 Melbourne Instruments, Australia
 *-----------------------------------------------------------------------------
 * @file  chart.cpp
 * @brief Chart class implementation.
 *-----------------------------------------------------------------------------
 */
#include <QOpenGLShaderProgram>
#include <QPainter>
#include "chart.h"
#include "gui_common.h"
#include "utils.h"

// Constants
constexpr float LINE_ALPHA        = 1.0f;
constexpr float FILL_ALPHA        = 0.5f;
constexpr uint DEFAULT_LINE_WIDTH = 4;

// Vertex shader
static const char *_vertex_shader_source_core =
    "#version 310 es\n"
        "layout (location = 0) in vec3 aPos;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
        "}\0";

// Fragment shader
static const char *_fragment_shader_source_core =
    "#version 310 es\n"
        "precision mediump float;\n"
        "out vec4 FragColor;\n"
        "uniform vec4 system_colour;\n"
        "void main()\n"
        "{\n"
        "   FragColor = system_colour;\n"
        "}\0";

//----------------------------------------------------------------------------
// Chart
//----------------------------------------------------------------------------
Chart::Chart(uint num_points, QWidget *parent) : QOpenGLWidget(parent)
{
    // Initialise the line and fill vertex arrays
    _line_vertices = new float[num_points * 3];
    _fill_vertices = new float[num_points * 3 * 2];
    for (uint i=0; i<num_points; i++) {
        _line_vertices[(i*3)] = -1.0f + ((qreal(i) / num_points) * 2);
        _line_vertices[(i*3)+1] = 0.0f;
        _line_vertices[(i*3)+2] = 0.0f;
        _fill_vertices[(i*6)] = -1.0f + ((qreal(i) / num_points) * 2);
        _fill_vertices[(i*6)+1] = 0.0f;
        _fill_vertices[(i*6)+2] = 0.0f;
        _fill_vertices[(i*6)+3] = -1.0f + ((qreal(i) / num_points) * 2);
        _fill_vertices[(i*6)+4] = -1.0f;
        _fill_vertices[(i*6)+5] = 0.0f;                
    }

    // Initialise the rest of the class data
    _num_points = num_points;
    _program = nullptr;
}

//----------------------------------------------------------------------------
// ~Chart
//----------------------------------------------------------------------------
Chart::~Chart()
{
    // Perform any cleanup actions
    cleanup();
}

//----------------------------------------------------------------------------
// shown
//----------------------------------------------------------------------------
bool Chart::shown() const
{
    // Return if the chart is shown or not
    return isVisible();
}

//----------------------------------------------------------------------------
// show
//----------------------------------------------------------------------------
void Chart::show()
{
    // Show the chart
    setVisible(true);
}

//----------------------------------------------------------------------------
// hide
//----------------------------------------------------------------------------
void Chart::hide()
{
    // Hide the chart
    setVisible(false);
}

//----------------------------------------------------------------------------
// set_colour
//----------------------------------------------------------------------------
void Chart::set_colour(QColor colour)
{
    // Set the chart colour
    _colour = colour;
}

//----------------------------------------------------------------------------
// refresh_data
//----------------------------------------------------------------------------
void Chart::refresh_data(const QVector<QPointF>& data)
{
    // Update the line and fill vertices
    for (uint i=0; i<_num_points; i++) {
        _line_vertices[(i*3)] = data[i].x();
        _line_vertices[(i*3)+1] = data[i].y();
        _fill_vertices[(i*6)] = data[i].x();
        _fill_vertices[(i*6)+1] = data[i].y();
        _fill_vertices[(i*6)+3] = data[i].x();
        _fill_vertices[(i*6)+4] = -1.0;           
    }
  
    // Refresh the chart
    update();
}

//----------------------------------------------------------------------------
// cleanup
//----------------------------------------------------------------------------
void Chart::cleanup()
{
    // If the shader program is not defined
    if (_program == nullptr) {
        // Just delete any allocated memory
        delete [] _line_vertices;
        delete [] _fill_vertices;
    }
    else {
        // Clean up the shader program
        makeCurrent();
        delete [] _line_vertices;
        delete [] _fill_vertices;
        delete _program;
        _program = nullptr;
        doneCurrent();        
        QObject::disconnect(context(), &QOpenGLContext::aboutToBeDestroyed, this, &Chart::cleanup);
    }
}

//----------------------------------------------------------------------------
// initializeGL
//----------------------------------------------------------------------------
void Chart::initializeGL()
{
    // Make sure we handle any Open GL clean-up correctly
    connect(context(), &QOpenGLContext::aboutToBeDestroyed, this, &Chart::cleanup);

    // Intialise Open GL - including enabling blend (for alpha control), and setting
    // the clear colour
    initializeOpenGLFunctions();
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0, 0, 0, 0);

    // Create the Open GL shader program - adding our vertex and fragment processing
    _program = new QOpenGLShaderProgram;
    _program->addShaderFromSourceCode(QOpenGLShader::Vertex, _vertex_shader_source_core);
    _program->addShaderFromSourceCode(QOpenGLShader::Fragment, _fragment_shader_source_core);
    _program->bindAttributeLocation("vertex", 0);
    _program->link();
    _program->bind();
    _colour_loc = _program->uniformLocation("system_colour");
    
    // Create our line Vertex Array Object (VAO), and bind it to our line Vertex Buffer Object (VBO)
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    _line_vao.create();
    QOpenGLVertexArrayObject::Binder line_vao_Binder(&_line_vao);
    _line_vbo.create();
    _line_vbo.bind();
    _line_vbo.setUsagePattern(QOpenGLBuffer::DynamicDraw);
    _line_vbo.allocate(_line_vertices, (_num_points * 3) * sizeof(GLfloat));
    f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat),
                             nullptr);
    f->glEnableVertexAttribArray(0);
    _line_vbo.release();
    _program->release();

    // Create our fill Vertex Array Object (VAO), and bind it to our fill Vertex Buffer Object (VBO)
    _fill_vao.create();
    QOpenGLVertexArrayObject::Binder fill_vao_Binder(&_fill_vao);
    _fill_vbo.create();
    _fill_vbo.bind();
    _fill_vbo.setUsagePattern(QOpenGLBuffer::DynamicDraw);
    _fill_vbo.allocate(_fill_vertices, (_num_points * 3 * 2) * sizeof(GLfloat));
    f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat),
                             nullptr);
    f->glEnableVertexAttribArray(0);
    _fill_vbo.release();
    _program->release();

    // Set the line width
    f->glLineWidth(DEFAULT_LINE_WIDTH);
}

//----------------------------------------------------------------------------
// paintGL
//----------------------------------------------------------------------------
void Chart::paintGL()
{
    // Clear the chart
    glClear(GL_COLOR_BUFFER_BIT);

    // Update our line vertices VBO and draw it
     QOpenGLVertexArrayObject::Binder line_vao_Binder(&_line_vao);
    _program->bind();
    _line_vbo.bind();
    _line_vbo.allocate(_line_vertices, (_num_points * 3) * sizeof(GLfloat));
    _line_vbo.release();
    _program->setUniformValue(_colour_loc, QVector4D(_colour.redF(), _colour.greenF(), _colour.blueF(), LINE_ALPHA));
    glDrawArrays(GL_LINE_STRIP, 0, _num_points);
    _program->release();

    // Update our fill vertices VBO and draw it
     QOpenGLVertexArrayObject::Binder fill_vao_Binder(&_fill_vao);
    _program->bind();
    _fill_vbo.bind();
    _fill_vbo.allocate(_fill_vertices, (_num_points * 3 * 2) * sizeof(GLfloat));
    _fill_vbo.release();
    _program->setUniformValue(_colour_loc, QVector4D(_colour.redF(), _colour.greenF(), _colour.blueF(), FILL_ALPHA));
    glDrawArrays(GL_TRIANGLE_STRIP, 0, _num_points * 2);
    _program->release();
}
