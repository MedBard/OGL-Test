#ifndef OBJECT3D_H
#define OBJECT3D_H
#include <QOpenGLBuffer>
#include <QMatrix4x4>
#include <QVector2D>

class QOpenGLTexture;
class QOpenGLFunctions;
class QOpenGLShaderProgram;

struct VertexData
{
    VertexData()
    {
    }
    VertexData(QVector3D p, QVector2D t, QVector3D n) :
        position(p), texCoord(t), normal(n)
    {
    }
    QVector3D position;
    QVector2D texCoord;
    QVector3D normal;
};


class Object3D
{
public:
    Object3D();
    ~Object3D();
    Object3D(const QVector<VertexData> &vertData,
             const QVector<GLuint> &indexes, const QImage &texture);
    void init(const QVector<VertexData> &vertData,
              const QVector<GLuint> &indexes, const QImage &texture);
    void draw(QOpenGLShaderProgram * program, QOpenGLFunctions *functions);
    void translate(const QVector3D &translateVector);

private:
    QOpenGLBuffer m_vertexBuffer;
    QOpenGLBuffer m_indexBuffer;
    QMatrix4x4 m_modelMatrix;
    QOpenGLTexture *m_texture;
};

#endif // OBJECT3D_H
