#include "../ramen/rgl_utils.h"
#include "../../util/mesh.cpp"

std::vector<Vertex> CreateCuboid(const Vec3f& color);

struct Cube {
    Mesh mesh;

    Cube(const Vec3f& color) {
        std::vector<Vertex> verts = CreateCuboid(color);
        mesh = Mesh(verts);
    };

    void draw(Mat4f modelMat, Mat4f viewMat, Mat4f projMat) {
        mesh.activate();
        glUniformMatrix4fv(0, 1, GL_FALSE, modelMat.Data());
        glUniformMatrix4fv(1, 1, GL_FALSE, viewMat.Data());
        glUniformMatrix4fv(2, 1, GL_FALSE, projMat.Data());

        mesh.draw();
    };

    void deleteBuffers() {
        mesh.deleteBuffers();
    }
};

std::vector<Vertex> CreateCuboid(const Vec3f& color) {

    std::vector<Vertex> verts;

    auto addFace = [&](Vec3f p0, Vec3f p1, Vec3f p2, Vec3f p3, Vec3f norm ) {
        verts.push_back(Vertex{.position = p0, .normal=norm, .color=color });
        verts.push_back(Vertex{.position = p1, .normal=norm, .color=color });
        verts.push_back(Vertex{.position = p2, .normal=norm, .color=color });

        verts.push_back(Vertex{.position = p2, .normal=norm, .color=color });
        verts.push_back(Vertex{.position = p1, .normal=norm, .color=color });
        verts.push_back(Vertex{.position = p3, .normal=norm, .color=color });

    };

    Vec3f dicePoints[8] = {
        Vec3f{-0.5f, -0.5f, -0.5f}, // - - -
        Vec3f{-0.5f, -0.5f,  0.5f}, // - - +
        Vec3f{-0.5f,  0.5f, -0.5f}, // - + -
        Vec3f{-0.5f,  0.5f,  0.5f}, // - + +
        Vec3f{ 0.5f, -0.5f, -0.5f}, // + - -
        Vec3f{ 0.5f, -0.5f,  0.5f}, // + - +
        Vec3f{ 0.5f,  0.5f, -0.5f}, // + + -
        Vec3f{ 0.5f,  0.5f,  0.5f}, // + + +
    };

    addFace(dicePoints[0], dicePoints[1], dicePoints[2], dicePoints[3], Vec3f(-1.f,  0.f,  0.f)); // Left
    addFace(dicePoints[5], dicePoints[4], dicePoints[7], dicePoints[6], Vec3f( 1.f,  0.f,  0.f)); // Right
    addFace(dicePoints[1], dicePoints[0], dicePoints[5], dicePoints[4], Vec3f( 0.f, -1.f,  0.f)); // Bottom
    addFace(dicePoints[2], dicePoints[3], dicePoints[6], dicePoints[7], Vec3f( 0.f,  1.f,  0.f)); // Top
    addFace(dicePoints[0], dicePoints[2], dicePoints[4], dicePoints[6], Vec3f( 0.f,  0.f, -1.f)); // Back
    addFace(dicePoints[3], dicePoints[1], dicePoints[7], dicePoints[5], Vec3f( 0.f,  0.f, -1.f)); // Front
    
    return verts;

}