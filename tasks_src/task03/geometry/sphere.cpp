#include "sphere.h"

#include "../util/vertex.h"

Sphere::Sphere(const Vec3f& color, int tesselation, bool useNormAsColor, bool drawNorms)
: drawNorms(drawNorms)
{
    auto [sphereVerts, normVerts] = CreateSphere(color, tesselation, useNormAsColor, drawNorms);
    sphereMesh = Mesh(sphereVerts);
    normMesh = Mesh(normVerts); 
}

void Sphere::draw(Mat4f modelMat, Mat4f viewMat, Mat4f projMat)
{
    sphereMesh.activate();
    glUniformMatrix4fv(0, 1, GL_FALSE, modelMat.Data());
    glUniformMatrix4fv(1, 1, GL_FALSE, viewMat.Data());
    glUniformMatrix4fv(2, 1, GL_FALSE, projMat.Data());

    sphereMesh.draw(GL_TRIANGLES);

     if(drawNorms) {
        normMesh.activate();
        glUniformMatrix4fv(0, 1, GL_FALSE, modelMat.Data());
        glUniformMatrix4fv(1, 1, GL_FALSE, viewMat.Data());
        glUniformMatrix4fv(2, 1, GL_FALSE, projMat.Data());

        normMesh.draw(GL_LINES);
    }
}

void Sphere::deleteBuffers()
{
    sphereMesh.deleteBuffers();
    normMesh.deleteBuffers();
}

std::tuple<std::vector<Vertex>, std::vector<Vertex>> CreateSphere(const Vec3f& color, int tesselation, bool useNormAsColor, bool drawNorms)
{
    std::vector<Vertex> sphereVerts;
    std::vector<Vertex> normVerts;
    const float r       = 0.5f;
    float       endLat  = RAMEN_PI;
    float       endLong = RAMEN_PI * 2.f;
    float       stepLat  = (endLat) / tesselation;
    float       stepLong = (endLong) / tesselation;
    for (float i = 0; i < tesselation; i++)
    {
        float lat     = i * stepLat;
        float latNext = (i + 1 == tesselation) ? endLat : (i + 1) * stepLat;

        for (float j = 0; j < tesselation; j++)
        {
            float lon     = j * stepLong;
            float lonNext = (j + 1 == tesselation) ? endLong : (j + 1) * stepLong;

            auto P = [&](float lat, float lon) {
                return Vec3f(
                    r * sinf(lat) * cosf(lon),
                    r * cosf(lat),
                    r * sinf(lat) * sinf(lon)
                );
            };

            Vec3f v00 = P(lat,     lon);
            Vec3f v01 = P(lat,     lonNext); // East
            Vec3f v10 = P(latNext, lon);     // South
            Vec3f v11 = P(latNext, lonNext); // South-East

            Vec3f n00 = Normalize(v00);
            Vec3f n01 = Normalize(v01);
            Vec3f n10 = Normalize(v10);
            Vec3f n11 = Normalize(v11);

            // Triangle 1
            CreateVertices(sphereVerts, normVerts, v00, n00, color, useNormAsColor, drawNorms);
            CreateVertices(sphereVerts, normVerts, v01, n01, color, useNormAsColor, drawNorms);
            CreateVertices(sphereVerts, normVerts, v10, n10, color, useNormAsColor, drawNorms);

            // Triangle 2
            CreateVertices(sphereVerts, normVerts, v01, n01, color, useNormAsColor, drawNorms);
            CreateVertices(sphereVerts, normVerts, v11, n11, color, useNormAsColor, drawNorms);
            CreateVertices(sphereVerts, normVerts, v10, n10, color, useNormAsColor, drawNorms);
        
        }
    }

    return {sphereVerts, normVerts};
}
