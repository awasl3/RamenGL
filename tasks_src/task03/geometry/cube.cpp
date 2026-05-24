#include "cube.h"
#include "../util/vertex.h"

Cube::Cube(const Vec3f& color, bool useNormAsColor, bool drawNorms)
: drawNorms(drawNorms)
{
    auto [cubeVerts, normVerts] = CreateCuboid(color, useNormAsColor, drawNorms);
    cubeMesh = Mesh(cubeVerts);
    normMesh = Mesh(normVerts); 
    
}

void Cube::draw(Mat4f modelMat, Mat4f viewMat, Mat4f projMat)
{
    cubeMesh.activate();
    glUniformMatrix4fv(0, 1, GL_FALSE, modelMat.Data());
    glUniformMatrix4fv(1, 1, GL_FALSE, viewMat.Data());
    glUniformMatrix4fv(2, 1, GL_FALSE, projMat.Data());

    cubeMesh.draw();

    if(drawNorms) {
        normMesh.activate();
        glUniformMatrix4fv(0, 1, GL_FALSE, modelMat.Data());
        glUniformMatrix4fv(1, 1, GL_FALSE, viewMat.Data());
        glUniformMatrix4fv(2, 1, GL_FALSE, projMat.Data());

        normMesh.draw(GL_LINES);
    }
}

void Cube::deleteBuffers()
{
    cubeMesh.deleteBuffers();
    normMesh.deleteBuffers();
}

std::tuple<std::vector<Vertex>, std::vector<Vertex>> CreateCuboid(const Vec3f& color, bool useNormAsColor, bool drawNorms)
{
    std::vector<Vertex> cubeVerts;
    std::vector<Vertex> normVerts;
    Vec3f useColor = color;
    auto addFace = [&](Vec3f p0, Vec3f p1, Vec3f p2, Vec3f p3, Vec3f norm) {
        CreateVertices(cubeVerts, normVerts, p0, norm, color, useNormAsColor, drawNorms);
        CreateVertices(cubeVerts, normVerts, p1, norm, color, useNormAsColor, drawNorms);
        CreateVertices(cubeVerts, normVerts, p2, norm, color, useNormAsColor, drawNorms);

        CreateVertices(cubeVerts, normVerts, p2, norm, color, useNormAsColor, drawNorms);
        CreateVertices(cubeVerts, normVerts, p1, norm, color, useNormAsColor, drawNorms);
        CreateVertices(cubeVerts, normVerts, p3, norm, color, useNormAsColor, drawNorms);
        
    };

    Vec3f dicePoints[8] = {
        Vec3f{ -0.5f, -0.5f, -0.5f }, // - - -
        Vec3f{ -0.5f, -0.5f,  0.5f }, // - - +
        Vec3f{ -0.5f,  0.5f, -0.5f }, // - + -
        Vec3f{ -0.5f,  0.5f,  0.5f }, // - + +
        Vec3f{  0.5f, -0.5f, -0.5f }, // + - -
        Vec3f{  0.5f, -0.5f,  0.5f }, // + - +
        Vec3f{  0.5f,  0.5f, -0.5f }, // + + -
        Vec3f{  0.5f,  0.5f,  0.5f }, // + + +
    };

    addFace(dicePoints[0], dicePoints[1], dicePoints[2], dicePoints[3], Vec3f(-1.f,  0.f,  0.f)); // Left
    addFace(dicePoints[5], dicePoints[4], dicePoints[7], dicePoints[6], Vec3f( 1.f,  0.f,  0.f)); // Right
    addFace(dicePoints[1], dicePoints[0], dicePoints[5], dicePoints[4], Vec3f( 0.f, -1.f,  0.f)); // Bottom
    addFace(dicePoints[2], dicePoints[3], dicePoints[6], dicePoints[7], Vec3f( 0.f,  1.f,  0.f)); // Top
    addFace(dicePoints[0], dicePoints[2], dicePoints[4], dicePoints[6], Vec3f( 0.f,  0.f, -1.f)); // Back
    addFace(dicePoints[3], dicePoints[1], dicePoints[7], dicePoints[5], Vec3f( 0.f,  0.f,  1.f)); // Front

    return {cubeVerts, normVerts};
}
