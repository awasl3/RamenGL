#include "../ramen/rgl_utils.h"
#include "../../util/mesh.cpp"

std::vector<Vertex> CreateSphere(const Vec3f& color, int tesselation = 32);

struct Sphere {
    Mesh mesh;

    Sphere(const Vec3f& color, int tesselation = 32) {
        std::vector<Vertex> verts = CreateSphere(color, tesselation);
        mesh = Mesh(verts);
    };

    void draw(Mat4f modelMat, Mat4f viewMat, Mat4f projMat) {
        mesh.activate();
        glUniformMatrix4fv(0, 1, GL_FALSE, modelMat.Data());
        glUniformMatrix4fv(1, 1, GL_FALSE, viewMat.Data());
        glUniformMatrix4fv(2, 1, GL_FALSE, projMat.Data());

        mesh.draw(GL_TRIANGLES);
    };

    void deleteBuffers() {
        mesh.deleteBuffers();
    }
};

std::vector<Vertex> CreateSphere(const Vec3f& color, int tesselation) {

    std::vector<Vertex> verts;
    const float r = 0.5f;
    float endLat = RAMEN_PI;
    float endLong = RAMEN_PI * 2.f;
    float stepLat = (endLat) / tesselation;
	float stepLong = (endLong) / tesselation;


	for (float i = 0;i < tesselation;i++) {
        float lat = i * stepLat;
        float latNext = (i + 1 == tesselation) ? endLat : (i + 1) * stepLat;
		
        for (float j = 0;j < tesselation;j++) {
			float lon = j * stepLong;
			float lonNext = (j + 1 == tesselation) ? endLong : (j + 1) * stepLong;

            auto P = [&](float lat, float lon) {
                return Vec3f(
                    r * sinf(lat) * cosf(lon),
                    r * cosf(lat),
                    r * sinf(lat) * sinf(lon)
                );
            };

            Vec3f v00 = P(lat, lon);
            Vec3f v01 = P(lat, lonNext);     // East
            Vec3f v10 = P(latNext, lon);     // South
            Vec3f v11 = P(latNext, lonNext); // South-East

            Vec3f n00 = Normalize(v00);
            Vec3f n01 = Normalize(v01);
            Vec3f n10 = Normalize(v10);
            Vec3f n11 = Normalize(v11);

            // Triangle 1
            verts.push_back({v00, n00, color});
            verts.push_back({v10, n10, color});
            verts.push_back({v01, n01, color});

           // Triangle 2
            verts.push_back({v01, n01, color});
            verts.push_back({v10, n10, color});
            verts.push_back({v11, n11, color});
       
        }
    }

    return verts;

}