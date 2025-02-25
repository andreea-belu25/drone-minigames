#pragma once

#include "components/simple_scene.h"
#include "lab_m1/lab5/lab_camera.h"
#include "MeshRenderer.h"

#include <vector>
#include <random>

using namespace std;

namespace m1
{
    class Tema2 : public gfxc::SimpleScene
    {
     public:
         struct ViewportArea
         {
            ViewportArea() : x(0), y(0), width(1), height(1) {}
            ViewportArea(int x, int y, int width, int height)
                : x(x), y(y), width(width), height(height) {}
            int x;
            int y;
            int width;
            int height;
         };

        Tema2();
        ~Tema2();

        void Init() override;

     private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void RenderSimpleMesh(Mesh * mesh, Shader * shader, const glm::mat4 & modelMatrix);

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

        void RenderDrone(const std::string& name, glm::vec3 position);
        void SaveObjectPositions();
        void RenderObjects();
        void RenderScene();
        void RenderMinimap();
        void RenderClock();
        void RenderCheckpoint();

        float GetTerrainHeight(float x, float z);
        bool CollisonWithBuildings();
        bool CollisionWithTrees();

     protected:
        implemented::Camera *myCamera;
        glm::mat4 projectionMatrix, perspectiveProjection, modelMatrix;
        bool renderCameraTarget;

        GLfloat fov = RADIANS(60);
        bool perspectiveMode = true;
        float aspectRatio;

        float terrainWidth;
        float terrainHeight;
      
        float droneRotationOY = 0.0f;  //  current rotation angle
        const float droneTranslationSpeed = 5.0f;
        float droneRotationSpeed = 150.0f;

        float armLength = 2.0f;
        float armOffset = 0.9f;  //  distance from center to cube
        float verticalOffset = 0.2f;  //  height offset for components
        float eliceHeight = 0.12f;  //  height offset for elices
        float eliceRotationAngle = 0;

        vector<Mesh*> trees;
        vector<Mesh*> buildings;
        vector<Mesh*> clouds;

        vector<glm::vec3> treePositions;
        vector<glm::vec3> buildingPositions;
        vector<glm::vec3> cloudPositions;
        vector<glm::vec3> checkpointPositions;

        float droneRadius = 1.0f;
        bool minimapRender = false, clockRender = false;
        bool thirdPerson = false;

        //  variables for timer race
        float timePassed = 0;
        float clockLength = 2.f;
    };
}   // namespace m1
