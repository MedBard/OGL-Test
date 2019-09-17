#include "object3d.h"
#include <QOpenGLTexture>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>

Object3D::Object3D() : m_indexBuffer(QOpenGLBuffer::IndexBuffer),
    m_texture(nullptr)
{

}

Object3D::~Object3D()
{
    if(m_vertexBuffer.isCreated())
        m_vertexBuffer.destroy();
    if(m_indexBuffer.isCreated())
        m_indexBuffer.destroy();
    if(m_texture != nullptr){
        if(m_texture->isCreated())
            m_texture->destroy();
    }
}

Object3D::Object3D(const QVector<VertexData> &vertData,
                   const QVector<GLuint> &indexes, const QImage &texture) :
      m_indexBuffer(QOpenGLBuffer::IndexBuffer), m_texture(nullptr)
{
    init(vertData, indexes, texture);
}

void Object3D::init(const QVector<VertexData> &vertData, const QVector<GLuint> &indexes, const QImage &texture)
{
    if(m_vertexBuffer.isCreated())
            m_vertexBuffer.destroy();
        if(m_indexBuffer.isCreated())
            m_indexBuffer.destroy();
        if(m_texture != nullptr){
            if(m_texture->isCreated()){
                delete m_texture;
                m_texture = nullptr;
            }
        }

    m_vertexBuffer.create();
    m_vertexBuffer.bind();
    m_vertexBuffer.allocate(vertData.constData(), vertData.size() *
                            static_cast<int>(sizeof(VertexData)));
    m_vertexBuffer.release();

    m_indexBuffer.create();
    m_indexBuffer.bind();
    m_indexBuffer.allocate(indexes.constData(),
                           indexes.size() * static_cast<int>(sizeof(GLuint)));
    m_indexBuffer.release();

    m_texture = new QOpenGLTexture(texture.mirrored());
    m_texture->setMinificationFilter(QOpenGLTexture::Nearest);
    m_texture->setMagnificationFilter(QOpenGLTexture::Linear);
    m_texture->setWrapMode(QOpenGLTexture::Repeat);

    m_modelMatrix.setToIdentity();
}

void Object3D::draw(QOpenGLShaderProgram *program, QOpenGLFunctions *functions)
{

    if(!m_vertexBuffer.isCreated() || !m_indexBuffer.isCreated())
        return;

    m_texture->bind(0);
    program->setUniformValue("u_texture", 0);
    program->setUniformValue("u_modelMatrix", m_modelMatrix);

    m_vertexBuffer.bind();
    int offset = 0;

    int vertLoc = program->attributeLocation("a_position");
    program->enableAttributeArray(vertLoc);
    program->setAttributeBuffer(vertLoc, GL_FLOAT, offset,
                                 3, sizeof (VertexData));

    offset += sizeof (QVector3D);

    int texLoc = program->attributeLocation("a_texcoord");
    program->enableAttributeArray(texLoc);
    program->setAttributeBuffer(texLoc, GL_FLOAT, offset,
                                 2, sizeof (VertexData));

    offset += sizeof (QVector2D);

    int normalLoc = program->attributeLocation("a_normal");
    program->enableAttributeArray(normalLoc);
    program->setAttributeBuffer(normalLoc, GL_FLOAT, offset,
                                 3, sizeof (VertexData));

    m_indexBuffer.bind();
    functions->glDrawElements(GL_TRIANGLES, m_indexBuffer.size(),
                   GL_UNSIGNED_INT, nullptr);

    m_vertexBuffer.release();
    m_indexBuffer.release();
    m_texture->release();
}

void Object3D::translate(const QVector3D &translateVector)
{
    m_modelMatrix.translate(translateVector);
}
