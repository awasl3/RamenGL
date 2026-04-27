#include <glad/glad.h>

#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include <vector>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_impl_sdl3.h>

#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>
#include <physfs.h>

#include "../ramen/ramen.h"
#include "../ramen/rgl_camera.h"
#include "../ramen/rgl_defines.h"
#include "../ramen/rgl_filesystem.h"
#include "../ramen/rgl_image.h"
#include "../ramen/rgl_math.h"
#include "../ramen/rgl_model.h"
#include "../ramen/rgl_shader.h"
#include "../ramen/rgl_utils.h"


#include "../util/mesh.h"
#include "geometry/cube.h"
#include "geometry/cylinder.h"
#include "geometry/sphere.h"

int main(int argc, char** argv)
{
    Filesystem* pFS = Filesystem::Init(argc, argv, "assets");

    Ramen* pRamen = Ramen::Instance();
    pRamen->Init("GUI", 800, 600);

    /* Load shaders. */
    Shader shader{};
    if ( !shader.Load("shaders/task03.vert", "shaders/task03.frag") )
    {
        fprintf(stderr, "Could not load shader.\n");
    }

    /* Create camera */
    Camera camera(Vec3f{ 0.0f, 2.0f, 3.0f });
    camera.RotateAroundSide(-10.0f);

    /* Model mat*/
    Mat4f modelMat = Mat4f::Identity();

    /* Use coordinate system as a dummy model so you see how VBO, VAOs work again. */
    const Vertex* data = Utils::CoordSystemRHZU();
    size_t size = 6;
    std::vector<Vertex> vec(data, data + size);
    Mesh coordMesh = Mesh(vec);
    /* All controls write into this. Geometry is rebuilt from it each frame. */
    enum Geometry { GEO_SPHERE = 0, GEO_CUBE = 1, GEO_CYLINDER = 2 };
    struct ImGuiState
    {
        int   selectedGeometry = GEO_SPHERE;
        float color[3]         = { 1.f, 0.f, 0.f };
        bool  useNormAsColor   = false;
        bool  drawNorms        = false;
        int   tesselation = 8;
        float lightPos[3] = { 2.f, 2.f, 2.f};
    };
    ImGuiState state;
    const char* geometryNames[] = { "Sphere", "Cube", "Cylinder" };

    /* Some global GL states */
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);
    // glDisable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    /* Main loop */
    bool isRunning = true;
    SDL_GL_SetSwapInterval(1); /* 1 = VSync enabled; 0 = VSync disabled */
    while ( isRunning )
    {

        SDL_Event e;
        while ( SDL_PollEvent(&e) )
        {
            ImGui_ImplSDL3_ProcessEvent(&e);
            pRamen->ProcessInputEvent(e);

            if ( e.type == SDL_EVENT_QUIT )
            {
                isRunning = false;
            }

            if ( e.type == SDL_EVENT_KEY_DOWN )
            {
                switch ( e.key.key )
                {
                case SDLK_ESCAPE:
                {
                    isRunning = false;
                }
                break;

                default:
                {
                }
                }
            }
        }

        /* Query new frame dimensions */
        int windowWidth, windowHeight;
        SDL_GetWindowSize(pRamen->GetWindow(), &windowWidth, &windowHeight);

        /* Adjust viewport and perspective projection accordingly. */
        glViewport(0, 0, windowWidth, windowHeight);

        /* View mat */
        Mat4f viewMat = LookAt(
            camera.GetPosition(), camera.GetPosition() + camera.GetForward(), camera.GetUp()); // Mat4f::Identity();

        /* Projection mat */
        float aspect  = (float)windowWidth / (float)windowHeight;
        Mat4f projMat = PerspectiveProjection(TO_RAD(60.0f), aspect, 0.01f, 500.0f);

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();
        ImGui::Begin("Controls");
        if(ImGui::CollapsingHeader("Scene controls", ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::Combo("Geometry", &state.selectedGeometry, geometryNames, IM_ARRAYSIZE(geometryNames));
        ImGui::ColorEdit3("Color", state.color);
        ImGui::Checkbox("Use norm as color", &state.useNormAsColor);
         ImGui::Checkbox("Draw norms", &state.drawNorms);
        ImGui::SliderInt("Tesselation", &state.tesselation, 3, 64);
            ImGui::InputFloat3("Light position", state.lightPos);
        }
        ImGui::End();
        /* ImGUI Rendering */
        ImGui::Render();

        /* Rendering */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
        glUniform3f(3, state.lightPos[0], state.lightPos[1], state.lightPos[2]);
        shader.Use();
        coordMesh.activate();
        glUniformMatrix4fv(0, 1, GL_FALSE, modelMat.Data());
        glUniformMatrix4fv(1, 1, GL_FALSE, viewMat.Data());
        glUniformMatrix4fv(2, 1, GL_FALSE, projMat.Data());
        coordMesh.draw(GL_LINES);

        /* Build only the selected geometry from current ImGui state, draw it, free it. */
        Vec3f c(state.color[0], state.color[1], state.color[2]);
        switch ( state.selectedGeometry )
        {
        case GEO_SPHERE:
        {
            Sphere s(c, state.tesselation, state.useNormAsColor, state.drawNorms);
            s.draw(modelMat, viewMat, projMat);
            s.deleteBuffers();
            break;
        }
        case GEO_CUBE:
        {
            Cube cube(c, state.useNormAsColor, state.drawNorms);
            cube.draw(modelMat, viewMat, projMat);
            cube.deleteBuffers();
            break;
        }
        case GEO_CYLINDER:
        {
            Cylinder cyl(c, state.tesselation, state.useNormAsColor, state.drawNorms);
            cyl.draw(modelMat, viewMat, projMat);
            cyl.deleteBuffers();
            break;
        }
        }

        

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        SDL_GL_SwapWindow(pRamen->GetWindow());
    }

    /* GL Resources shutdown. */
    shader.Delete();
    coordMesh.deleteBuffers();

    /* Ramen Shutdown */
    pRamen->Shutdown();

    /* Filesystem deinit */
    PHYSFS_deinit();

    return 0;
}
