#include "geometry.h"


#include <QTextStream>
#include <QString>
#include <QStringList>



Geometry::Geometry()
{
    this->ptexture = 0;
    //loadPtex("/home/conor/FYP/samples/triangle/triangle.ptx");
    //load3dModel("/home/conor/FYP/samples/triangle/triangle.obj");

    loadPtex("/home/conor/FYP/samples/teapot/teapot.ptx");
    load3dModel("/home/conor/FYP/samples/teapot/teapot.obj");

    //loadPtex("/home/conor/FYP/samples/bunny/bunny.ptx");
    //load3dModel("/home/conor/FYP/samples/bunny/bunny.obj");

    //loadPtex("/home/conor/FYP/samples/holeycube/holeycube.ptx");
    //load3dModel("/home/conor/FYP/samples/holeycube/holeycube.obj");
}

Geometry::~Geometry()
{
    ptexture->release();
}

void Geometry::loadArrays() const
{
    glVertexPointer(3, GL_FLOAT, 0, vertices.constData());
    glNormalPointer(GL_FLOAT, 0, vertexNormals.constData());
}

void Geometry::finalize()
{
    // TODO: add vertex buffer uploading here

}


void Geometry::draw()
{
    Ptex::MeshType mt = ptexture->meshType();
    int numFaces = ptexture->numFaces();

    glEnableClientState( GL_VERTEX_ARRAY );
    glVertexPointer(3, GL_FLOAT,0,this->vertices.constData());


    for(int i = 0; i<numFaces; i++)
    {

        glBindTexture(GL_TEXTURE_2D,this->texname[i]);
        if(mt == Ptex::mt_quad)
        {
            glBegin(GL_QUADS);
        }
        else
        {
            glBegin(GL_TRIANGLES);
        }
        glLoadName(i);
        for(int j = 0; j < this->faces[i].vertexCount; j++)
        {
            if(j == 0)
            {
                glTexCoord2f(0,0);
            }
            if(j == 1)
            {
                glTexCoord2f(1,0);
            }
            if(j == 2)
            {
                glTexCoord2f(1,1);
            }
            if(j == 3)
            {
                glTexCoord2f(0,1);
            }
            glArrayElement(this->verticesIndices[this->faces[i].vertexIndex + j]);
        }
        glEnd();
    }
/*
    glColor3f(0.0,1.0,0.0);
    for(int i = 0; i<numFaces; i++)
    {

        glBindTexture(GL_TEXTURE_2D,this->texname[i]);
        glBegin(GL_LINE_LOOP);
        for(int j = 0; j < this->faces[i].vertexCount; j++)
        {
            if(j == 0)
            {
                glTexCoord2f(0,0);
            }
            if(j == 1)
            {
                glTexCoord2f(1,0);
            }
            if(j == 2)
            {
                glTexCoord2f(1,1);
            }
            if(j == 3)
            {
                glTexCoord2f(0,1);
            }
            glArrayElement(this->verticesIndices[this->faces[i].vertexIndex + j]);
        }
        glEnd();
    }
*/

    glDisableClientState( GL_VERTEX_ARRAY );
}

void Geometry::setupTexture()
{
    if(this->ptexture)
    {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

        int numFaces = ptexture->numFaces();
        Ptex::DataType dt = ptexture->dataType();

        texname = new GLuint[numFaces];
        for(int i = 0; i<numFaces; i++)
        {
            int level = 0;

            glGenTextures(1, texname+i);

            glBindTexture(GL_TEXTURE_2D,*(texname+i));
            Ptex::BorderMode ubm = ptexture->uBorderMode();
            if(ubm == Ptex::m_black)
            {
                glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
            }
            if(ubm == Ptex::m_clamp)
            {
                glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
            }
            if(ubm == Ptex::m_periodic)
            {
                glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_MIRRORED_REPEAT);
            }
            Ptex::BorderMode vbm = ptexture->vBorderMode();
            if(vbm == Ptex::m_black)
            {
                glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
            }
            if(vbm == Ptex::m_clamp)
            {
                glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
            }
            if(vbm == Ptex::m_periodic)
            {
                glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_MIRRORED_REPEAT);
            }
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_NEAREST);
            PtexFaceData *fd = ptexture->getData(i);

            Ptex::Res res = fd->res();
            int width = res.u();
            int height = res.v();
            if(dt == Ptex::dt_float)
            {
                glTexImage2D(GL_TEXTURE_2D, level, GL_RGBA,width,height,0,GL_RGBA,GL_FLOAT,fd->getData());
            }
            if(dt == Ptex::dt_uint8)
            {
               glTexImage2D(GL_TEXTURE_2D, level, GL_RGBA,width,height,0,GL_RGBA,GL_UNSIGNED_BYTE,fd->getData());
            }
            if(dt == Ptex::dt_uint16)
            {
                glTexImage2D(GL_TEXTURE_2D, level, GL_RGBA,width,height,0,GL_RGBA,GL_UNSIGNED_INT,fd->getData());
            }
            fd->release();
            /*
            if(ptexture->hasMipMaps())
            {
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_NEAREST_MIPMAP_NEAREST);
                level++;
                PtexFaceData *pfd = ptexture->getData(i);
                Ptex::Res res = pfd->res();
                res.ulog2--;
                res.vlog2--;
                pfd->release();
                while (res.ulog2 > 0 || res.vlog2 > 0)
                {

                    PtexFaceData* fdMM = ptexture->getData(i, res);
                    if (!fdMM) break;
                    width = res.u();
                    height = res.v();
                    bool isconstant = fdMM->isConstant();
                    if(isconstant)
                    {
                        width = height = 1;
                    }
                    if(dt == Ptex::dt_float)
                    {
                        glTexImage2D(GL_TEXTURE_2D, level, GL_RGBA,width,height,0,GL_RGBA,GL_FLOAT,fdMM->getData());
                    }
                    if(dt == Ptex::dt_uint8)
                    {
                        glTexImage2D(GL_TEXTURE_2D, level, GL_RGBA,width,height,0,GL_RGBA,GL_UNSIGNED_BYTE,fdMM->getData());
                    }
                    if(dt == Ptex::dt_uint16)
                    {
                        glTexImage2D(GL_TEXTURE_2D, level, GL_RGBA,width,height,0,GL_RGBA,GL_UNSIGNED_INT,fdMM->getData());
                    }
                    fdMM->release();
                    if (isconstant) break;
                    if (res.ulog2) res.ulog2--;
                    if (res.vlog2) res.vlog2--;
                    level++;
                }
            }*/

        }
    }
}

void Geometry::deleteTexture()
{
    int numFaces = ptexture->numFaces();

    for(int i = 0; i<numFaces; i++)
    {
        glDeleteTextures(1, texname+i);
    }
    delete texname;
}


bool Geometry::loadPtex(const char *fileName)
{
    Ptex::String error;
    this->ptexture = PtexTexture::open(fileName,error,0);
     if(this->faces.count() != this->ptexture->numFaces())
         return false;


return true;
}

int Geometry::loadOBJ(QFile &file)
{

    QTextStream textIn(&file);


    uint64_t lineNo = 0;
    QString line;
    QStringList tokens;

    while(!textIn.atEnd())
    {

        line = textIn.readLine();
        lineNo++;
        tokens = line.split(' ',QString::SkipEmptyParts);


        if(tokens[0] == "v")
        {
            float temp[3];
            int length = tokens.length();
            if(length < 4)
            {
                //cerr << "not enough co ordinates given to the vertex at " << lineNo << endl;
                return -3;
            }
            if(length > 5)
            {
                //cerr << "too many co ordinates given to the vertex at " << lineNo << endl;
                return -4;
            }

            //I are only concerned with the x y z co ordinates as I am not interested in curves so the w co ordinate is ingored
            bool ok[3];

            temp[0] = tokens[1].toFloat(ok + 0);
            temp[1] = tokens[2].toFloat(ok + 1);
            temp[2] = tokens[3].toFloat(ok + 2);

            if(!ok[0] || !ok[1] || !ok[2])
            {
                //cerr << "could't convert co ordinates given from string to float" << lineNo << endl;
                return -2;
            }

            vertices.append(temp[0]);
            vertices.append(temp[1]);
            vertices.append(temp[2]);
        }
        if(tokens[0] == "vn")
        {
            float temp[3];
            int length = tokens.length();
            if(length < 4)
            {
                //cerr << "not enough co ordinates given to the vertex normal at " << lineNo << endl;
                return -5;
            }
            if(length > 4)
            {
                //cerr << "too many co ordinates given to the vertex normal at " << lineNo << endl;
                return -6;
            }

            bool ok[3];

            temp[0] = tokens[1].toFloat(ok + 0);
            temp[1] = tokens[2].toFloat(ok + 1);
            temp[2] = tokens[3].toFloat(ok + 2);

            if(!ok[0] || !ok[1] || !ok[2])
            {
                //cerr << "could't convert co ordinates given from string to float" << lineNo << endl;
                return -2;
            }

            vertexNormals.append(temp[0]);
            vertexNormals.append(temp[1]);
            vertexNormals.append(temp[2]);
        }

        if(tokens[0] == "vt")
        {

        }
        if(tokens[0] == "f")
        {


            //for splitting a token(vert offset)
            QStringList V_VT_VN_offsets;
            //the length/num of offstes for a vert should be the same throught the face
            uint8_t  V_VT_VN_offsets_tLenght = 0;


            //needed to get index of offset with minus numbers
            int vetrexListSize = Geometry::vertices.size();
            //int vetrexNormalListSize = Geometry::vertexNormals.size();
            //int vetrexTextureListSize = Geometry::vertextexture.size();

            //default offset is Zero
            //if remains zero there is an error
            int offsetV = 0;
            int offsetVT = 0;
            int offsetVN = 0;

            //check if face has enough entries to make at least triangle
            int length = tokens.length();
            if(length < 4)
            {
                //cerr << "not enough vertex given to the face at " << lineNo << endl;
                return -5;
            }


            //
            static int nextIndex = 0;
            face f;
            f.vertexCount = length - 1;
            f.vertexIndex = nextIndex;
            nextIndex += f.vertexCount;

            verticesIndices.resize(f.vertexIndex + f.vertexCount);


            //split all entries into the 3 types of vertex offset
            for(int i = 0; i< tokens.length()-1; i++)
            {
                V_VT_VN_offsets = tokens[i + 1].split('/');

                /*
                if(V_VT_VN_offsets_tLenght = 0 && V_VT_VN_offsets_tLenght != V_VT_VN_offsets.length())
                {
                    //cerr << " at lineNo " << lineNo << endl;
                    return-8;
                }
                else
                {*/
                    V_VT_VN_offsets_tLenght = V_VT_VN_offsets.length();
                //}



                bool ok;
                offsetV = V_VT_VN_offsets[0].toInt(&ok);
                if(V_VT_VN_offsets.length() > 1)
                {
                    if(!V_VT_VN_offsets[1].isEmpty())
                    {
                        offsetVT = V_VT_VN_offsets[1].toInt(&ok);
                    }
                    offsetVN = V_VT_VN_offsets[2].toInt(&ok);
                }



                //Convert Vertex offsets to indices
                if(offsetV > 0)
                {
                    verticesIndices[f.vertexIndex + i] = offsetV - 1;
                }
                if(offsetV < 0)
                {
                    verticesIndices[f.vertexIndex + i] = vetrexListSize + offsetV;
                }
                if(offsetV == 0)
                {
                    //cerr << "error" << endl;
                    return -7;
                }
                /*
                //Convert Vertex texture offsets to indices
                if(V_VT_VN_offsets.length() > 1)
                {
                    if(!V_VT_VN_offsets[1].isEmpty())
                    {
                        if(offsetVT > 0)
                        {
                            temp.vt[i] =vetrexTexture.at(offsetVT - 1);
                        }
                        if(offsetVT < 0)
                        {
                            temp.vt[i] =vetrexTexture.at(vetrexTextureListSize + offsetVT);
                        }
                        if(offsetVT == 0)
                        {
                            cerr << "error" << endl;
                            return -7;
                        }
                    }

                    //Convert Vertex normal offsets to indices
                    if(offsetVN > 0)
                    {
                        temp.vn[i] =vetrexNormal.at(offsetVN - 1);
                    }
                    if(offsetVN < 0)
                    {
                        temp.vn[i] = vetrexNormal.at(vetrexNormalListSize + offsetVN);
                    }
                    if(offsetVN == 0)
                    {
                        cerr << "error" << endl;
                        return -7;
                    }
                }*/

            }

            faces.push_back(f);

        }
    }

    return 1;
}

bool Geometry::load3dModel(QString filename)
{

    int returnValue = 1;

    QFile file(filename);
    if(!file.open(QIODevice::ReadOnly))
    {
        //cerr << "Cannot open file for reading: " << endl;
        return -1;
    }

    returnValue = this->loadOBJ(file);

    file.close();
    return returnValue == 1;
}
