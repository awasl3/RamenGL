#include "../ramen/rgl_utils.h"
#include "../../util/mesh.cpp"
#include <array> 

std::array<std::vector<Vertex>, 3> CreateCylinder(const Vec3f& color, int slices = 32);

struct Cylinder {
    Mesh topMesh;
    Mesh bottomMesh;
    Mesh sideMesh;

    Cylinder(const Vec3f& color, int slices = 32) {
        std::printf("Hello\n");
        std::array<std::vector<Vertex>, 3> verts = CreateCylinder(color, slices);
        topMesh = Mesh(verts[0]);
        bottomMesh = Mesh(verts[1]);
        sideMesh = Mesh(verts[2]);
    };

    void draw(Mat4f modelMat, Mat4f viewMat, Mat4f projMat) {
        topMesh.activate();
        glUniformMatrix4fv(0, 1, GL_FALSE, modelMat.Data());
        glUniformMatrix4fv(1, 1, GL_FALSE, viewMat.Data());
        glUniformMatrix4fv(2, 1, GL_FALSE, projMat.Data());

        topMesh.draw(GL_TRIANGLE_FAN);

        bottomMesh.activate();
        glUniformMatrix4fv(0, 1, GL_FALSE, modelMat.Data());
        glUniformMatrix4fv(1, 1, GL_FALSE, viewMat.Data());
        glUniformMatrix4fv(2, 1, GL_FALSE, projMat.Data());

        bottomMesh.draw(GL_TRIANGLE_FAN);

        sideMesh.activate();
        glUniformMatrix4fv(0, 1, GL_FALSE, modelMat.Data());
        glUniformMatrix4fv(1, 1, GL_FALSE, viewMat.Data());
        glUniformMatrix4fv(2, 1, GL_FALSE, projMat.Data());

        sideMesh.draw(GL_TRIANGLES);
    };

    void deleteBuffers() {
        topMesh.deleteBuffers();
        bottomMesh.deleteBuffers();
        sideMesh.deleteBuffers();
    }
};

std::array<std::vector<Vertex>, 3> CreateCylinder(const Vec3f& color, int slices) {
    std::printf("Hello\n");
    std::array<std::vector<Vertex>, 3> verts;

    const float r = 0.5f;
    const float h = 0.5f;


    verts[0].push_back({Vec3f(0.f, h, 0.f), Vec3f(0.f, 1.f, 0.f), color});
    verts[1].push_back({Vec3f(0.f, -h, 0.f), Vec3f(0.f, -1.f, 0.f), color});
    for(int i = 0; i <= slices; i++ ) {
        float theta = 2.0f * RAMEN_PI * float(i) / float(slices);
        float theta0 = 2.0f * RAMEN_PI * float(slices - i) / float(slices);
        float x = r * sinf(theta);
        float z = r * cosf(theta);
        float x0 = r * sinf(theta0);
        float z0 = r * cosf(theta0);

        verts[0].push_back({Vec3f(x, h, z), Vec3f(0.f, 1.f, 0.f), color});
        verts[1].push_back({Vec3f(x0, -h, z0), Vec3f(0.f, -1.f, 0.f), color});
    }

    for (int i = 0; i < slices; i++) {
        float theta = 2.0f * RAMEN_PI * float(i) / float(slices);
        float thetaNext = 2.0f * RAMEN_PI * float(i + 1) / float(slices);

        float x  = r * sinf(theta);
        float z  = r * cosf(theta);
        float x2 = r * sinf(thetaNext);
        float z2 = r * cosf(thetaNext);

        Vec3f n1 = Vec3f(sinf(theta), 0.f, cosf(theta));
        Vec3f n2 = Vec3f(sinf(thetaNext), 0.f, cosf(thetaNext));

        Vec3f top1(x,  h, z);
        Vec3f bot1(x, -h, z);
        Vec3f top2(x2,  h, z2);
        Vec3f bot2(x2, -h, z2);

        // Triangle 1
        verts[2].push_back({top1, n1, color});
        verts[2].push_back({bot1, n1, color});
        verts[2].push_back({top2, n2, color});

        // Triangle 2
        verts[2].push_back({top2, n2, color});
        verts[2].push_back({bot1, n1, color});
        verts[2].push_back({bot2, n2, color});
    }

    return verts;
}