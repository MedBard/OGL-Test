#include "OGLWidget.h"
#include "object3d.h"

#include <QMouseEvent>
#include <QOpenGLContext>


Widget::Widget(QWidget *parent)
    : QOpenGLWidget(parent)
{
}

Widget::~Widget()
{
}

void Widget::initializeGL()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glEnable(GL_DEPTH_TEST); // depth buffer
    glEnable(GL_CULL_FACE); // cutting behind

    initShaders();
    initCube(2.0f);
    m_objects[0]->translate(QVector3D(-1.2f, 0.0f, 0.0f));
    initCube(2.0f);
    m_objects[1]->translate(QVector3D(1.2f, 0.0f, 0.0f));
}

void Widget::resizeGL(int w, int h)
{
    float aspect = w / static_cast<float>(h);
    m_projectionMatrix.setToIdentity();
    m_projectionMatrix.perspective(45, aspect, 0.1f, 10.0f);
}

void Widget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    QMatrix4x4 viewMatrix;
    viewMatrix.setToIdentity();
    viewMatrix.translate(0.0f, 0.0f, -5.0f); //back off camera
    viewMatrix.rotate(m_rotation);

    m_program.bind();
    m_program.setUniformValue("u_projectionMatrix", m_projectionMatrix);
    m_program.setUniformValue("u_viewMatrix", viewMatrix);// camera matrix

    m_program.setUniformValue("u_lightPosition",
                              QVector4D(0.0f, 0.0f, 0.0f, 1.0f));
    m_program.setUniformValue("u_lightPower", 5.0f);
    for (int i = 0; i < m_objects.size(); ++i) {

         m_objects[i]->draw(&m_program, context()->functions());
    }
}

void Widget::mousePressEvent(QMouseEvent *event)
{
    if(event->buttons() == Qt::LeftButton){
        m_mousePosition = QVector2D(event->localPos());
    }
    event->accept();
}

void Widget::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() != Qt::LeftButton)
        return;
    QVector2D diff = QVector2D (event->localPos()) - m_mousePosition;
    m_mousePosition = QVector2D(event->localPos());

    float angle = diff.length() / 2.0f;
    QVector3D axis = QVector3D(diff.y(), diff.x(), 0.0f);
    m_rotation = QQuaternion::fromAxisAndAngle(axis, angle) * m_rotation;

    update();
}

void Widget::initShaders()
{
    if(!m_program.addShaderFromSourceFile(QOpenGLShader::Vertex,
                                          ":/vshader.vsh"))
        close();
    if(!m_program.addShaderFromSourceFile(QOpenGLShader::Fragment,
                                          ":/fshader.fsh"))
        close();
    if(!m_program.link())
        close();
}

void Widget::initCube(float width)
{
    float w_div_2 = width / 2.0f;
    QVector<VertexData> vertexes;

    vertexes.append(VertexData(QVector3D(-w_div_2, w_div_2, w_div_2),
                               QVector2D(0.0f, 1.0f),
                               QVector3D(0.0f, 0.0f, 1.0f)));
    vertexes.append(VertexData(QVector3D(-w_div_2, -w_div_2, w_div_2),
                               QVector2D(0.0f, 0.0f),
                               QVector3D(0.0f, 0.0f, 1.0f)));
    vertexes.append(VertexData(QVector3D(w_div_2, w_div_2, w_div_2),
                               QVector2D(1.0f, 1.0f),
                               QVector3D(0.0f, 0.0f, 1.0f)));
    vertexes.append(VertexData(QVector3D(w_div_2, -w_div_2, w_div_2),
                               QVector2D(1.0f, 0.0f),
                               QVector3D(0.0f, 0.0f, 1.0f)));

    vertexes.append(VertexData(QVector3D(w_div_2, w_div_2, w_div_2),
                               QVector2D(0.0f, 1.0f),
                               QVector3D(1.0f, 0.0f, 0.0f)));
    vertexes.append(VertexData(QVector3D(w_div_2, -w_div_2, w_div_2),
                               QVector2D(0.0f, 0.0f),
                               QVector3D(1.0f, 0.0f, 0.0f)));
    vertexes.append(VertexData(QVector3D(w_div_2, w_div_2, -w_div_2),
                               QVector2D(1.0f, 1.0f),
                               QVector3D(1.0f, 0.0f, 0.0f)));
    vertexes.append(VertexData(QVector3D(w_div_2, -w_div_2, -w_div_2),
                               QVector2D(1.0f, 0.0f),
                               QVector3D(1.0f, 0.0f, 0.0f)));

    vertexes.append(VertexData(QVector3D(w_div_2, w_div_2, w_div_2),
                               QVector2D(0.0f, 1.0f),
                               QVector3D(0.0f, 1.0f, 0.0f)));
    vertexes.append(VertexData(QVector3D(w_div_2, w_div_2, -w_div_2),
                               QVector2D(0.0f, 0.0f),
                               QVector3D(0.0f, 1.0f, 0.0f)));
    vertexes.append(VertexData(QVector3D(-w_div_2, w_div_2, w_div_2),
                               QVector2D(1.0f, 1.0f),
                               QVector3D(0.0f, 1.0f, 0.0f)));
    vertexes.append(VertexData(QVector3D(-w_div_2, w_div_2, -w_div_2),
                               QVector2D(1.0f, 0.0f),
                               QVector3D(0.0f, 1.0f, 0.0f)));

    vertexes.append(VertexData(QVector3D(w_div_2, w_div_2, -w_div_2),
                               QVector2D(0.0f, 1.0f),
                               QVector3D(0.0f, 0.0f, -1.0f)));
    vertexes.append(VertexData(QVector3D(w_div_2, -w_div_2, -w_div_2),
                               QVector2D(0.0f, 0.0f),
                               QVector3D(0.0f, 0.0f, -1.0f)));
    vertexes.append(VertexData(QVector3D(-w_div_2, w_div_2, -w_div_2),
                               QVector2D(1.0f, 1.0f),
                               QVector3D(0.0f, 0.0f, -1.0f)));
    vertexes.append(VertexData(QVector3D(-w_div_2, -w_div_2, -w_div_2),
                               QVector2D(1.0f, 0.0f),
                               QVector3D(0.0f, 0.0f, -1.0f)));

    vertexes.append(VertexData(QVector3D(-w_div_2, w_div_2, w_div_2),
                               QVector2D(0.0f, 1.0f),
                               QVector3D(-1.0f, 0.0f, 0.0f)));
    vertexes.append(VertexData(QVector3D(-w_div_2, w_div_2, -w_div_2),
                               QVector2D(0.0f, 0.0f),
                               QVector3D(-1.0f, 0.0f, 0.0f)));
    vertexes.append(VertexData(QVector3D(-w_div_2, -w_div_2, w_div_2),
                               QVector2D(1.0f, 1.0f),
                               QVector3D(-1.0f, 0.0f, 0.0f)));
    vertexes.append(VertexData(QVector3D(-w_div_2, -w_div_2, -w_div_2),
                               QVector2D(1.0f, 0.0f),
                               QVector3D(-1.0f, 0.0f, 0.0f)));

    vertexes.append(VertexData(QVector3D(-w_div_2, -w_div_2, w_div_2),
                               QVector2D(0.0f, 1.0f),
                               QVector3D(0.0f, -1.0f, 0.0f)));
    vertexes.append(VertexData(QVector3D(-w_div_2, -w_div_2, -w_div_2),
                               QVector2D(0.0f, 0.0f),
                               QVector3D(0.0f, -1.0f, 0.0f)));
    vertexes.append(VertexData(QVector3D(w_div_2, -w_div_2, w_div_2),
                               QVector2D(1.0f, 1.0f),
                               QVector3D(0.0f, -1.0f, 0.0f)));
    vertexes.append(VertexData(QVector3D(w_div_2, -w_div_2, -w_div_2),
                               QVector2D(1.0f, 0.0f),
                               QVector3D(0.0f, -1.0f, 0.0f)));
    QVector<GLuint> indexes;
    for (GLuint i = 0; i < 24; i += 4)
    {
        indexes.append(i + 0);
        indexes.append(i + 1);
        indexes.append(i + 2);
        indexes.append(i + 2);
        indexes.append(i + 1);
        indexes.append(i + 3);
    }

    m_objects.append(new Object3D(vertexes, indexes, QImage(":/cube.png")));
}

