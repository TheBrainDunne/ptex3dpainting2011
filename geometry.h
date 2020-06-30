#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <QVector>
#include <QVector3D>
#include <stdint.h>
#include <QFile>
#include <Ptexture.h>
#include <GL/gl.h>

class Geometry
{

public:
    struct face
    {
        uint32_t vertexIndex;
        uint8_t vertexCount;
    };

    Geometry();
    ~Geometry();
    void finalize();
    void loadArrays() const;
    bool load3dModel(QString filename);
    bool loadPtex(const char *fileName);
    void draw();
    void setupTexture();
    void deleteTexture();


//private:
    GLuint *texname;
    PtexTexture *ptexture;
    QVector<face> faces;
    QVector<unsigned int> verticesIndices;
    QVector<int> verticesNormalsIndices;
    QVector<int> verticestextureIndices;
    QVector<float> vertices;
    QVector<float> vertexNormals;
    QVector<QVector3D> vertextexture;
    int loadOBJ(QFile &file);

};

#endif // GEOMETRY_H
