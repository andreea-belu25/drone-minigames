#pragma once

#include <string>
#include <vector>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"

using namespace std;

namespace MeshRenderer
{
    Mesh* GenerateTerrain(float length, float width, int rows, int columns);
    Mesh* GenerateParalelipiped(const std::string& name, float height, float width, float length, glm::vec3 color);
    Mesh* GenerateCube(const std::string& name, float size, glm::vec3 color);
    Mesh* GenerateCylinder(float radius, float height, int segments, glm::vec3 color);
    Mesh* GenerateCone(float radius, float height, int segments, glm::vec3 baseCenter, glm::vec3 color);
    Mesh* GenerateSphere(const std::string& name, float radius, int stacks, int segments, glm::vec3 color);
    Mesh* GenerateConeTree(float bottomConeRadius, float upConeRadius, float heightBottomCone, float heightUpCone, 
                        glm::vec3 centerBottomCone, glm::vec3 centerUpCone, glm::vec3 colorBottomCone, glm::vec3 colorUpCone, int segments);
    Mesh* GenerateTree(float radiusTrunk, float heightTrunk, glm::vec3 colorTrunk, int segments, float bottomConeRadius, 
                        float upConeRadius, float heightBottomCone, float heightUpCone, glm::vec3 centerBottomCone, 
                        glm::vec3 centerUpCone, glm::vec3 colorBottomCone, glm::vec3 colorUpCone);
    vector<Mesh*> GenerateRandomBuildings(float size);
    vector<Mesh*> GenerateRandomTrees(float height);
    vector<Mesh*> GenerateRandomClouds(int nums, float minSize, float maxSize);
    vector<Mesh*> CreateDrone(const std::string& name, float armLength, float armWidth, float cubeSize, float eliceLength, float eliceWidth);
    Mesh* CreateOutsideSquare(const std::string &name, float length, glm::vec3 color);
    Mesh* CreateInnerSquare(const std::string &name, float length, glm::vec3 color);
    Mesh* CreateTriangle(const std::string &name, float length);
}