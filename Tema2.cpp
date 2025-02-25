#include <vector>
#include <iostream>
#include <random>

#include "Tema2.h"
#include "MeshRenderer.h"
#include "lab_m1/lab5/lab_camera.h"
#include "components/simple_scene.h"

using namespace std;
using namespace m1;

Tema2::Tema2() {
}

Tema2::~Tema2() {
}

//  get terrain height considering how it is created in vertex shader
float Tema2::GetTerrainHeight(float x, float z) {
    float frequency = 0.15f;
    
    glm::vec2 pos(x * frequency, z * frequency);
    glm::vec2 i = glm::floor(pos);
    glm::vec2 f = pos - i;
    glm::vec2 u = f * f * (3.0f - 2.0f * f);

    float a = glm::fract(sin(glm::dot(i, glm::vec2(12.9898f, 78.233f))) * 43758.5453123f);
    float b = glm::fract(sin(glm::dot(i + glm::vec2(1.0f, 0.0f), glm::vec2(12.9898f, 78.233f))) * 43758.5453123f);
    float c = glm::fract(sin(glm::dot(i + glm::vec2(0.0f, 1.0f), glm::vec2(12.9898f, 78.233f))) * 43758.5453123f);
    float d = glm::fract(sin(glm::dot(i + glm::vec2(1.0f, 1.0f), glm::vec2(12.9898f, 78.233f))) * 43758.5453123f);
    
    float noise = glm::mix(a, b, u.x) + (c - a) * u.y * (1.0f - u.x) + (d - b) * u.x * u.y;

    return noise * 0.5f;
}

void Tema2::SaveObjectPositions() {
    random_device rd;
    mt19937 gen(rd());

    //  generate random positions for objects
    uniform_real_distribution<float> posDistX(-terrainWidth / 2 + 5.0f, terrainWidth / 2 - 5.0f);
    uniform_real_distribution<float> posDistZ(-terrainHeight / 2 + 5.0f, terrainHeight / 2 - 5.0f);
    
    //  generate random heights for clouds
    uniform_real_distribution<float> cloudHeight(20.0f, 40.0f);

    float minDistance = 5.0f;

    /*  distribute buildings on terrain, check if there are no collisions with existing objects 
        and save their positions
    */    
    for (int i = 0; i < buildings.size(); i++) {
        bool validPosition = false;
        glm::vec3 newPosition;
        float height = 4.0f;

        while (!validPosition) {
            float x = posDistX(gen);
            float z = posDistZ(gen);
            float terrainHeight = GetTerrainHeight(x, z);
            float buildingY = terrainHeight + height / 2;
            
            newPosition = glm::vec3(x, buildingY, z);
            validPosition = true;

            for (const auto& pos: buildingPositions) {
                if (glm::distance(newPosition, pos) < minDistance) {
                    validPosition = false;
                    break;
                }
            }
        }

        buildingPositions.push_back(newPosition);
    }

    //  the same logic for distributing trees on the terrain
    for (int i = 0; i < trees.size(); i++) {
        bool validPosition = false;
        glm::vec3 newPosition;
        float height = 2.0f;

        while (!validPosition) {
            float x = posDistX(gen);
            float z = posDistZ(gen);
            float terrainHeight = GetTerrainHeight(x, z);

            newPosition = glm::vec3(x, terrainHeight, z);
            validPosition = true;

            for (const auto& pos : buildingPositions) {
                if (glm::distance(newPosition, pos) < minDistance) {
                    validPosition = false;
                    break;
                }

            }

            if (validPosition) {
                for (const auto& pos: treePositions) {
                    if (glm::distance(newPosition, pos) < minDistance) {
                        validPosition = false;
                        break;
                    }
                }
            }
        }

        treePositions.push_back(newPosition);
    }

    //  the same logic for checkpoints
    for (int i = 0; i < 6; i++) {  // generate 6 checkpoints
        bool validPosition = false;
        glm::vec3 newPosition;
        float checkpointRadius = 5.0f;

        while (!validPosition) {
            float x = posDistX(gen);
            float z = posDistZ(gen);
            float y = 12.0f;
            
            newPosition = glm::vec3(x, y, z);
            validPosition = true;

            for (const auto& pos : buildingPositions) {
                if (glm::distance(glm::vec2(newPosition.x, newPosition.z), 
                                glm::vec2(pos.x, pos.z)) < (checkpointRadius + 4.0f)) {
                    validPosition = false;
                    break;
                }
            }

            if (validPosition) {
                for (const auto& pos : treePositions) {
                    if (glm::distance(glm::vec2(newPosition.x, newPosition.z), 
                                    glm::vec2(pos.x, pos.z)) < (checkpointRadius + 2.5f)) {
                        validPosition = false;
                        break;
                    }
                }
            }

            if (validPosition) {
                for (const auto& pos : checkpointPositions) {
                    if (glm::distance(glm::vec2(newPosition.x, newPosition.z), 
                                    glm::vec2(pos.x, pos.z)) < (checkpointRadius * 3)) {
                        validPosition = false;
                        break;
                    }
                }
            }
        }

        checkpointPositions.push_back(newPosition);
    }

    //  distribute clouds on the sky without collisions
    const int spheresPerCloud = 3;
    float sphereOffset = 1.5f;  //  controls how much spheres overlap
    float cloudSpacing = 15.0f;

    for (int i = 0; i < clouds.size() / spheresPerCloud; i++) {
        bool validPosition = false;
        glm::vec3 centerPosition;

        while (!validPosition) {
            centerPosition = glm::vec3(posDistX(gen), cloudHeight(gen), posDistZ(gen));
            validPosition = true;
            
            // check distance from other cloud centers
            for (const auto& pos : cloudPositions) {
                if (glm::distance(centerPosition, pos) < cloudSpacing) {
                    validPosition = false;
                    break;
                }
            }
        }

        cloudPositions.push_back(centerPosition + glm::vec3(-sphereOffset, 0, 0));     
        cloudPositions.push_back(centerPosition);
        cloudPositions.push_back(centerPosition + glm::vec3(sphereOffset, 0, 0));
    }
}

void Tema2::Init() {
    //  init camera
    renderCameraTarget = false;
    aspectRatio = window->props.aspectRatio;
    myCamera = new implemented::Camera();
    myCamera->Set(glm::vec3(0, 2, 3.5f), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));
    myCamera->TranslateUpward(15.0f);
    projectionMatrix = glm::perspective(fov, aspectRatio, 0.01f, 200.0f);
    glm::ivec2 resolution = window->GetResolution();

    //  initialize terrain
    terrainWidth = resolution.x / 10.0f;
    terrainHeight = resolution.y / 10.0f;

    Mesh *terrainMesh = MeshRenderer::GenerateTerrain(terrainHeight, terrainWidth, resolution.y / 20,
                                                      resolution.x / 20);
    meshes["terrain"] = terrainMesh;

    Shader *shader = new Shader("TerrainShader");
    shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
    shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
    shader->CreateAndLink();
    shaders[shader->GetName()] = shader;

    //  initialize drone components
    vector<Mesh*> droneMeshes = MeshRenderer::CreateDrone("drone", 2.0f, 0.2f, 0.2f, 0.75f, 0.05f);
    meshes["drone_arm"] = droneMeshes[0];
    meshes["drone_cube"] = droneMeshes[1];
    meshes["drone_elice"] = droneMeshes[2];

    //  generate and save objects and their positions
    trees = MeshRenderer::GenerateRandomTrees(2.0f);
    buildings = MeshRenderer::GenerateRandomBuildings(4.0f);
    clouds = MeshRenderer::GenerateRandomClouds(15, 3.0f, 10.0f);

    SaveObjectPositions();

    //  generate the squares for checkpoint
    meshes["squareOutside"] = MeshRenderer::CreateOutsideSquare("squareOutside", 10.0f, glm::vec3(1, 0, 0));
    meshes["squareInner"] = MeshRenderer::CreateInnerSquare("squareInner", 8.0f, glm::vec3(1, 0, 1));

    //  generator the arrow for orientating the drone
    meshes["indicator"] = MeshRenderer::CreateTriangle("indicator", .5f);
}

void Tema2::FrameStart() {
    glClearColor(137 / 255.0f, 207 / 255.0f, 240 / 255.0f, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    glViewport(0, 0, resolution.x, resolution.y);
}

void Tema2::RenderCheckpoint() {
    //  render the 6 created checkpoints
    for (const auto& position : checkpointPositions) {
        glLineWidth(5.0f);  //  make the line for outside square thicker
        glm::mat4 squareOutsideModel = glm::mat4(1);
        squareOutsideModel = glm::translate(squareOutsideModel, position);
        RenderSimpleMesh(meshes["squareOutside"], shaders["VertexColor"], squareOutsideModel);

        glLineWidth(3.0f);
        glm::mat4 squareInnerModel = glm::mat4(1);  //  make the line for inside square thinner
        squareInnerModel = glm::translate(squareInnerModel, position);
        RenderSimpleMesh(meshes["squareInner"], shaders["VertexColor"], squareInnerModel);
    }
    
    glLineWidth(1.0f);  //  reset for other objects
}

void Tema2::RenderObjects() {
    //  put trees, buildings and clouds at their saved positions
    for (int i = 0; i < trees.size(); i++) {
        modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, treePositions[i]);
        RenderSimpleMesh(trees[i], shaders["VertexColor"], modelMatrix);
    }
    
    for (int i = 0; i < buildings.size(); i++) {
        modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, buildingPositions[i]);
        RenderSimpleMesh(buildings[i], shaders["VertexColor"], modelMatrix);
    }

    int spheresPerCloud = 3;
    
    for (int i = 0; i < cloudPositions.size(); i += spheresPerCloud) {
        for (int j = 0; j < spheresPerCloud; j++) {
            if (i + j < clouds.size() && i + j < cloudPositions.size()) {
                modelMatrix = glm::mat4(1);
                modelMatrix = glm::translate(modelMatrix, cloudPositions[i + j]);
                RenderSimpleMesh(clouds[i + j], shaders["VertexColor"], modelMatrix);
            }
        }
    }

    //  also, render the checkpoints
    RenderCheckpoint();
}

void Tema2::RenderDrone(const std::string& name, glm::vec3 position) {
    glm::mat4 baseDrone = glm::mat4(1);
    baseDrone = glm::translate(baseDrone, position);
    vector<glm::vec3> vectors = myCamera->GetVectors();
    float rotationOY = atan2(vectors[0].x, vectors[0].z);  
    baseDrone = glm::rotate(baseDrone, rotationOY, glm::vec3(0, 1, 0));

    //  link the arrow to drone
    glm::mat4 indicatorMatrix = baseDrone;
    indicatorMatrix = glm::translate(indicatorMatrix, glm::vec3(0.0f, 0.0f, .75f));

    //  make the indicator look after the target position
    glm::vec3 indicator_position = glm::vec3(indicatorMatrix[3]);
    glm::vec3 target = glm::vec3(1, 1, 1);

    //  this is without collisions - is rendered only to see if the arrow indicates correctly the object at the target position
    RenderSimpleMesh(buildings[0], shaders["VertexColor"], glm::translate(glm::mat4(1), target));  

    glm::vec3 direction = glm::normalize(target - indicator_position);
    glm::vec3 up = glm::vec3(0, 1, 0);

    glm::mat4 orientation = glm::toMat4(glm::quatLookAt(direction, up));  //  how the indicator should be rotated
    //  0,1,2 - comp of rotation
    indicatorMatrix[0] = orientation[0];
    indicatorMatrix[1] = orientation[1];
    indicatorMatrix[2] = orientation[2];

    RenderSimpleMesh(meshes["indicator"], shaders["VertexColor"], indicatorMatrix);

    //  create arms for the drone
    glm::mat4 arm1Model = baseDrone;
    arm1Model = glm::rotate(arm1Model, glm::radians(45.0f), glm::vec3(0, 1, 0));

    glm::mat4 arm2Model = baseDrone;
    arm2Model = glm::rotate(arm2Model, glm::radians(-45.0f), glm::vec3(0, 1, 0));

    RenderSimpleMesh(meshes[name + "_arm"], shaders["VertexColor"], arm1Model);
    RenderSimpleMesh(meshes[name + "_arm"], shaders["VertexColor"], arm2Model);

    //  link the cubes to the before created arms
    vector<glm::mat4> cubeModels;
    cubeModels.push_back(glm::translate(arm1Model, glm::vec3(-armOffset, verticalOffset, 0)));  // left cube arm1
    cubeModels.push_back(glm::translate(arm1Model, glm::vec3(armOffset, verticalOffset, 0)));   // right cube arm1
    cubeModels.push_back(glm::translate(arm2Model, glm::vec3(-armOffset, verticalOffset, 0)));  // left cube arm2
    cubeModels.push_back(glm::translate(arm2Model, glm::vec3(armOffset, verticalOffset, 0)));   // right cube arm2

    //  for each cube, render an elice and make it rotate continously
    for (int i = 0; i < cubeModels.size(); i++) {
        RenderSimpleMesh(meshes[name + "_cube"], shaders["VertexColor"], cubeModels[i]);
        glm::mat4 eliceModel = glm::translate(cubeModels[i], glm::vec3(0, eliceHeight, 0));

        float currentRotation = eliceRotationAngle; 
        if (i == 2 || i == 3)
            currentRotation += 90.0f;
           
        eliceModel = glm::rotate(eliceModel, glm::radians(currentRotation), glm::vec3(0, 1, 0));
        eliceModel = glm::rotate(eliceModel, glm::radians(90.0f), glm::vec3(1, 0, 0));  // Make horizontal
        
        RenderSimpleMesh(meshes[name + "_elice"], shaders["VertexColor"], eliceModel);
    }
}

void Tema2::RenderScene() {
    glm::mat4 terrainModel = glm::mat4(1);
    RenderSimpleMesh(meshes["terrain"], shaders["TerrainShader"], terrainModel);

    RenderDrone("drone", myCamera->GetTargetPosition(thirdPerson));  //  offset => dau camera si mai in spate
    RenderObjects();
}

void Tema2::RenderMinimap() {
    glm::ivec2 resolution = window->GetResolution();
    ViewportArea minimap = ViewportArea(20.0f, 20.0f, 240.0f, 120.0f);

    glClear(GL_DEPTH_BUFFER_BIT);
    glViewport(minimap.x, minimap.y, minimap.width, minimap.height);

    projectionMatrix = glm::ortho(-7.5f, 7.5f, -7.5f, 7.5f, -20.0f, 100.0f);
    minimapRender = true;
    RenderScene();
    minimapRender = false;
    projectionMatrix = glm::perspective(fov, aspectRatio, 0.01f, 200.0f);
}

void Tema2::RenderClock() {
    //  create new viewport for time passer race
    glm::ivec2 resolution = window->GetResolution();
    float height = 80.0f;
    float width = 500.0f;
    ViewportArea clock = ViewportArea(resolution.x / 2 - width / 2, resolution.y - height / 2, width, height);

    glClear(GL_DEPTH_BUFFER_BIT);
    glViewport(clock.x, clock.y, clock.width, clock.height);

    projectionMatrix = glm::ortho(0.0f, 1.0f, 0.0f, 1.0f, -1.0f, 100.0f);
    clockRender = true;

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));

    //  make the bar animate continously
    float length = 2.f;
    float clockLength = max(0.0f, (-2.0f / 60.0f) * timePassed + 2.0f);

    //  create the bars for the clock
    Mesh* clockMesh1 = MeshRenderer::GenerateParalelipiped("clock", 0.75f, 1.0f, length, 
                                                        glm::vec3(1.0f, 1.0f, 1.0f));
    RenderSimpleMesh(clockMesh1, shaders["VertexColor"], glm::translate(model, glm::vec3(0.0f, 0.0f, -0.01f)));

    Mesh* clockMesh = MeshRenderer::GenerateParalelipiped("clock", 0.75f, 1.0f, clockLength, 
                                                        glm::vec3(1.0f, 1.0f, 0.0f));
    RenderSimpleMesh(clockMesh, shaders["VertexColor"], model);

    clockRender = false;
    projectionMatrix = glm::perspective(fov, aspectRatio, 0.01f, 200.0f);
}

void Tema2::Update(float deltaTimeSeconds)
{
    timePassed += deltaTimeSeconds;  //  time passed at each frame
    eliceRotationAngle += 620.0f * deltaTimeSeconds;  //  speed for elice

    RenderScene();
    RenderClock();
    RenderMinimap();
}

void Tema2::FrameEnd() {
}

void Tema2::RenderSimpleMesh(Mesh *mesh, Shader *shader, const glm::mat4 &modelMatrix)
{
    if (!mesh || !shader || !shader->program)
        return;

    // render an object using the specified shader and the specified position
    glUseProgram(shader->program);

    glm::mat4 viewMatrix = myCamera->GetViewMatrix();
    glm::vec3 drone_position = myCamera->GetTargetPosition(thirdPerson);
    if (minimapRender) {
        viewMatrix = glm::lookAt(drone_position, glm::vec3(drone_position.x, 0.0f, drone_position.z), glm::vec3(0.0f, 0.0f, -1.0f));
        //  camera mea se uita de unde e drona in jos, y = 0
    }
    if (clockRender) {
        viewMatrix = glm::mat4(1.0f);  //  se uita in fata la ceas
    }
    glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));
    glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    glUniform3f(glGetUniformLocation(shader->program, "drone_position"), drone_position.x, drone_position.y, drone_position.z);

    mesh->Render();
}

bool Tema2::CollisonWithBuildings() {
    //  get drone position and check if there is any collision with a building on OX, OY, OZ
    glm::vec3 dronePosition = myCamera->GetTargetPosition(thirdPerson);
    
    for (int i = 0; i < buildingPositions.size(); i++) {
        glm::vec3 currentBuildingPos = buildingPositions[i];
        float buildingSize = 4.0f;

        bool collisionOX = false;
        float diffOX = abs(dronePosition.x - currentBuildingPos.x);
        if (diffOX <= buildingSize / 2 + droneRadius)
            collisionOX = true;

        bool collisionOY = false;
        float diffOY = abs(dronePosition.y - currentBuildingPos.y);
        if (diffOY <= buildingSize / 2 + droneRadius)
            collisionOY = true;

        bool collisionOZ = false;
        float diffOZ = abs(dronePosition.z - currentBuildingPos.z);
        if (diffOZ <= buildingSize / 2 + droneRadius)
            collisionOZ = true;

        if (collisionOX && collisionOY && collisionOZ)
            return true;
    }

    return false;
}

bool Tema2::CollisionWithTrees() {
    glm::vec3 dronePosition = myCamera->GetTargetPosition(thirdPerson);

    for (int i = 0; i < treePositions.size(); i++) {
        //  get current drone position and check if there is a collision with a tree (its trunk, bottom or up cone)
        glm::vec3 treePos = treePositions[i];

        float trunkHeight = 8.0f;
        float trunkRadius = 0.8f;
        float coneRadius = 2.5f;
        float bottomConeHeight = 8.0f;
        float upConeHeight = 8.0f;
        float treeHeight = 4.0f;

        //  check collision with tree trunk    
        float diffXZ = sqrt(pow(dronePosition.x - treePos.x, 2) + pow(dronePosition.z - treePos.z, 2));
        
        if (diffXZ <= trunkRadius + droneRadius && dronePosition.y - droneRadius <= treePos.y + trunkHeight && dronePosition.y + droneRadius >= treePos.y)
            return true;

        if (dronePosition.y - droneRadius <= treePos.y + treeHeight && dronePosition.y + droneRadius >= treePos.y + trunkHeight) {
            //  bottom cone
            glm::vec3 bottomConeStart = treePos + glm::vec3(0, trunkHeight, 0);
            glm::vec3 bottomConeEnd = bottomConeStart + glm::vec3(0, bottomConeHeight, 0);
        
            if (dronePosition.y + droneRadius >= bottomConeStart.y && dronePosition.y - droneRadius <= bottomConeEnd.y) {
                float heightInBottomCone = dronePosition.y - bottomConeStart.y;
                float radiusAtHeightBottom = coneRadius * (1 - heightInBottomCone / bottomConeHeight);
                
                if (diffXZ <= radiusAtHeightBottom + droneRadius)
                    return true;
            }

            //  up cone
            glm::vec3 upConeStart = bottomConeEnd;
            glm::vec3 upConeEnd = upConeStart + glm::vec3(0, upConeHeight, 0);
        
            if (dronePosition.y + droneRadius >= upConeStart.y && dronePosition.y - droneRadius <= upConeEnd.y) {
                float heightInUpCone = dronePosition.y - upConeStart.y;
                float radiusAtHeightUp = coneRadius * (1 - heightInUpCone / upConeHeight);
            
                if (diffXZ <= radiusAtHeightUp + droneRadius)
                    return true;
            }
        }
    }

    return false;
}

void Tema2::OnInputUpdate(float deltaTime, int mods) {
    /*  retain position and if there is a collision undo the position and
        move the drone to its last position without a collision
    */
    float cameraSpeed = 5.0f;
    vector<int> moves(6);
    
    if (window->KeyHold(GLFW_KEY_W)) {
        moves[0] = 1;
        myCamera->TranslateForward(deltaTime * cameraSpeed);
    }
    if (window->KeyHold(GLFW_KEY_A)) {
        moves[1] = 1;
        myCamera->TranslateRight(-deltaTime * cameraSpeed);
    }
    if (window->KeyHold(GLFW_KEY_S)) {
        moves[2] = 1;
        myCamera->TranslateForward(-deltaTime * cameraSpeed);
    }
    if (window->KeyHold(GLFW_KEY_D)) { 
        moves[3] = 1;
        myCamera->TranslateRight(deltaTime * cameraSpeed);
    }
    if (window->KeyHold(GLFW_KEY_Q)) {
        moves[4] = 1;
        myCamera->TranslateUpward(-deltaTime * cameraSpeed);
    }
    if (window->KeyHold(GLFW_KEY_E)) {
        moves[5] = 1;
        myCamera->TranslateUpward(deltaTime * cameraSpeed);
    }

    glm::vec3 dronePosition = myCamera->GetTargetPosition(thirdPerson);

    if (CollisionWithTrees() || CollisonWithBuildings() || dronePosition.y - droneRadius < GetTerrainHeight(dronePosition.x, dronePosition.z)) {
        if (moves[0] == 1)
            myCamera->TranslateForward(-deltaTime * cameraSpeed);
        if (moves[1] == 1)
            myCamera->TranslateRight(deltaTime * cameraSpeed);
        if (moves[2] == 1)
            myCamera->TranslateForward(deltaTime * cameraSpeed);
        if (moves[3] == 1)
            myCamera->TranslateRight(-deltaTime * cameraSpeed);
        if (moves[4] == 1)
            myCamera->TranslateUpward(deltaTime * cameraSpeed);
        if (moves[5] == 1)
            myCamera->TranslateUpward(-deltaTime * cameraSpeed);
    }

    //  rotate the drone
    if (window->KeyHold(GLFW_KEY_P))
        myCamera->RotateFirstPerson_OY(-RADIANS(deltaTime * 180.0f));
    if (window->KeyHold(GLFW_KEY_O))
        myCamera->RotateFirstPerson_OY(RADIANS(deltaTime * 180.0f));
}

void Tema2::OnKeyPress(int key, int mods)
{
    //  switch camera mode
    if (key == GLFW_KEY_F)
        thirdPerson = !thirdPerson;
}

void Tema2::OnKeyRelease(int key, int mods)
{
    // Add key release event
}

void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
}

void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}

void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}

void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Tema2::OnWindowResize(int width, int height)
{
}
