#include "MeshRenderer.h"

#include <vector>
#include <random>

#include "core/engine.h"
#include "utils/gl_utils.h"

using namespace std;

Mesh* MeshRenderer::GenerateTerrain(float height, float width, int rows, int columns) {
    vector<VertexFormat> vertices;
    vector<unsigned int> indices;

    float cellWidth = width / (columns - 1);
    float cellHeight = height / (rows - 1);
    unsigned int vertexOffset = 0;

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            glm::vec3 bottomLeft = glm::vec3(j * cellWidth - width / 2, 0.0f, i * cellHeight - height / 2);

            vertices.push_back(VertexFormat(bottomLeft, glm::vec3(0, 0, 0)));
            vertices.push_back(VertexFormat(bottomLeft + glm::vec3(cellWidth, 0, 0)));
            vertices.push_back(VertexFormat(bottomLeft + glm::vec3(cellWidth, 0, cellHeight)));
            vertices.push_back(VertexFormat(bottomLeft + glm::vec3(0, 0, cellHeight)));

            unsigned int bottomLeftIndex = vertexOffset;
            unsigned int bottomRightIndex = vertexOffset + 1;
            unsigned int topRightIndex = vertexOffset + 2;
            unsigned int topLeftIndex = vertexOffset + 3;

            //  first triangle
            indices.push_back(bottomLeftIndex);
            indices.push_back(topLeftIndex);
            indices.push_back(topRightIndex);

            //  second triangle
            indices.push_back(bottomLeftIndex);
            indices.push_back(topRightIndex);
            indices.push_back(bottomRightIndex);

            vertexOffset += 4;    
        }
    }

    Mesh* terrain = new Mesh("terrain");
    terrain->InitFromData(vertices, indices);
    return terrain;
}

Mesh* MeshRenderer::GenerateParalelipiped(const std::string& name, float height, float width, float length, glm::vec3 color) {
    vector<VertexFormat> vertices;
    vector<unsigned int> indices;

    //  front face
    vertices.push_back(VertexFormat(glm::vec3(-length / 2, -height / 2, width / 2), color, glm::vec3(0, 0, 1)));
    vertices.push_back(VertexFormat(glm::vec3(length / 2, -height / 2, width / 2), color, glm::vec3(0, 0, 1)));
    vertices.push_back(VertexFormat(glm::vec3(length / 2, height / 2, width / 2), color, glm::vec3(0, 0, 1)));
    vertices.push_back(VertexFormat(glm::vec3(-length / 2, height / 2, width / 2), color, glm::vec3(0, 0, 1)));

    //  back face
    vertices.push_back(VertexFormat(glm::vec3(-length / 2, -height / 2, -width / 2), color, glm::vec3(0, 0, -1)));
    vertices.push_back(VertexFormat(glm::vec3(length / 2, -height / 2, -width / 2), color, glm::vec3(0, 0, -1)));
    vertices.push_back(VertexFormat(glm::vec3(length / 2, height / 2, -width / 2), color, glm::vec3(0, 0, -1)));
    vertices.push_back(VertexFormat(glm::vec3(-length / 2, height / 2, -width / 2), color, glm::vec3(0, 0, -1)));

    //  front
    indices.push_back(0); indices.push_back(1); indices.push_back(2);
    indices.push_back(0); indices.push_back(2); indices.push_back(3);

    //  back
    indices.push_back(5); indices.push_back(4); indices.push_back(7);
    indices.push_back(5); indices.push_back(7); indices.push_back(6);

    //  left
    indices.push_back(4); indices.push_back(0); indices.push_back(3);
    indices.push_back(4); indices.push_back(3); indices.push_back(7);

    //  right 
    indices.push_back(1); indices.push_back(5); indices.push_back(6);
    indices.push_back(1); indices.push_back(6); indices.push_back(2);

    //  top
    indices.push_back(3); indices.push_back(2); indices.push_back(6);
    indices.push_back(3); indices.push_back(6); indices.push_back(7);

    //  bottom
    indices.push_back(4); indices.push_back(5); indices.push_back(1);
    indices.push_back(4); indices.push_back(1); indices.push_back(0);
    
    Mesh* parallelepiped = new Mesh("parallelepiped");
    parallelepiped->InitFromData(vertices, indices);
    return parallelepiped;
}

Mesh* MeshRenderer::GenerateCube(const std::string& name, float size, glm::vec3 color) {
    vector<VertexFormat> vertices;
    vector<unsigned int> indices;

    float halfSize = size / 2.0f;

    //  front face
    vertices.push_back(VertexFormat(glm::vec3(-halfSize, -halfSize,  halfSize), color, glm::vec3(0, 0, 1)));
    vertices.push_back(VertexFormat(glm::vec3( halfSize, -halfSize,  halfSize), color, glm::vec3(0, 0, 1)));
    vertices.push_back(VertexFormat(glm::vec3( halfSize,  halfSize,  halfSize), color, glm::vec3(0, 0, 1)));
    vertices.push_back(VertexFormat(glm::vec3(-halfSize,  halfSize,  halfSize), color, glm::vec3(0, 0, 1)));

    //  back face
    vertices.push_back(VertexFormat(glm::vec3(-halfSize, -halfSize, -halfSize), color, glm::vec3(0, 0, -1)));
    vertices.push_back(VertexFormat(glm::vec3( halfSize, -halfSize, -halfSize), color, glm::vec3(0, 0, -1)));
    vertices.push_back(VertexFormat(glm::vec3( halfSize,  halfSize, -halfSize), color, glm::vec3(0, 0, -1)));
    vertices.push_back(VertexFormat(glm::vec3(-halfSize,  halfSize, -halfSize), color, glm::vec3(0, 0, -1)));

    //  front
    indices.push_back(0); indices.push_back(1); indices.push_back(2);
    indices.push_back(0); indices.push_back(2); indices.push_back(3);

    //  back
    indices.push_back(5); indices.push_back(4); indices.push_back(7);
    indices.push_back(5); indices.push_back(7); indices.push_back(6);

    //  left
    indices.push_back(4); indices.push_back(0); indices.push_back(3);
    indices.push_back(4); indices.push_back(3); indices.push_back(7);

    //  right
    indices.push_back(1); indices.push_back(5); indices.push_back(6);
    indices.push_back(1); indices.push_back(6); indices.push_back(2);

    //  top
    indices.push_back(3); indices.push_back(2); indices.push_back(6);
    indices.push_back(3); indices.push_back(6); indices.push_back(7);

    //  bottom
    indices.push_back(4); indices.push_back(5); indices.push_back(1);
    indices.push_back(4); indices.push_back(1); indices.push_back(0);

    Mesh* cube = new Mesh(name);
    cube->InitFromData(vertices, indices);
    return cube;
}

vector<Mesh*> MeshRenderer::CreateDrone(const std::string& name, float armLength, float armWidth, 
                                           float cubeSize, float eliceLength, float eliceWidth) {
    std::vector<Mesh*> droneMeshes;
    
    glm::vec3 armColor = glm::vec3(0.75f);
    glm::vec3 eliceColor = glm::vec3(0);

    Mesh* arm = MeshRenderer::GenerateParalelipiped(name + "_arm", armWidth, armWidth, armLength, armColor);
    droneMeshes.push_back(arm);

    Mesh* cube = MeshRenderer::GenerateCube(name + "_cube", cubeSize, armColor);
    droneMeshes.push_back(cube);

    Mesh* elice = MeshRenderer::GenerateParalelipiped(name + "_elice", eliceWidth, eliceWidth, eliceLength, eliceColor);
    droneMeshes.push_back(elice);

    return droneMeshes;
}

Mesh* MeshRenderer::GenerateCylinder(float radius, float height, int segments, glm::vec3 color) {
    vector<VertexFormat> vertices;
    vector<unsigned int> indices;

    float angleStep = 2 * M_PI / segments;

    for (int i = 0; i < segments; i++) {
        float angle = i * angleStep;
        float x = radius * cos(angle);
        float z = radius * sin(angle);

        vertices.push_back(VertexFormat(glm::vec3(x, 0, z), color));  //  bottom circle
        vertices.push_back(VertexFormat(glm::vec3(x, height, z), color));  //  top circle
    }

    vertices.push_back(VertexFormat(glm::vec3(0, 0, 0), color));
    vertices.push_back(VertexFormat(glm::vec3(0, height, 0), color));
    
    //  sides
    for (int i = 0; i < segments; i++) {
        int bottomFirst = i * 2;
        int topFirst = bottomFirst + 1;
        int bottomSecond = ((i + 1) % segments) * 2;
        int topSecond = bottomSecond + 1;

        //  first triangle
        indices.push_back(bottomFirst);
        indices.push_back(bottomSecond);
        indices.push_back(topFirst);

        //  second triangle
        indices.push_back(topFirst);
        indices.push_back(bottomSecond);
        indices.push_back(topSecond);
    }

    int bottomCenterIndex = segments * 2;
    int topCenterIndex = bottomCenterIndex + 1;

    for (int i = 0; i < segments; i++) {
        //  bottom circle
        int bottomFirst = i * 2 + 1;
        int bottomSecond = ((i + 1) % segments) * 2;

        indices.push_back(bottomCenterIndex);
        indices.push_back(bottomFirst);
        indices.push_back(bottomSecond);

        //  top circle
        int topFirst = i * 2 + 1;
        int topSecond = ((i + 1) % segments) * 2 + 1;

        indices.push_back(topCenterIndex);
        indices.push_back(topSecond);
        indices.push_back(topFirst);
    }

    Mesh* cylinder = new Mesh("cylinder");
    cylinder->InitFromData(vertices, indices);
    return cylinder;
}

Mesh* MeshRenderer::GenerateCone(float radius, float height, int segments, glm::vec3 baseCenter, glm::vec3 color) {
    vector<VertexFormat> vertices;
    vector<unsigned int> indices;

    float angleStep = 2 * M_PI / segments;

    //  vertices for the base circle
    for (int i = 0; i < segments; i++) {
        float angle = i * angleStep;
        float x = radius * cos(angle);
        float z = radius * sin(angle);

        //  base circle vertex
        vertices.push_back(VertexFormat(glm::vec3(baseCenter.x + x, baseCenter.y, baseCenter.z + z), color));
    }

    //  base center and apex vertices
    vertices.push_back(VertexFormat(baseCenter, color));
    vertices.push_back(VertexFormat(baseCenter + glm::vec3(0, height, 0), color));

    int baseCenterIndex = segments;
    int apexIndex = segments + 1;

    //  indices for the base circle
    for (int i = 0; i < segments; i++) {
        int current = i;
        int next = (i + 1) % segments;

        indices.push_back(baseCenterIndex);
        indices.push_back(current);
        indices.push_back(next);
    }

    //  indices for the sides of the cone
    for (int i = 0; i < segments; i++) {
        int current = i;
        int next = (i + 1) % segments;

        indices.push_back(apexIndex);  //  apex
        indices.push_back(current);
        indices.push_back(next);
    }

    Mesh* cone = new Mesh("cone");
    cone->InitFromData(vertices, indices);
    return cone;
}

Mesh* MeshRenderer::GenerateSphere(const std::string& name, float radius, int stacks, int segments, glm::vec3 color) {
    vector<VertexFormat> vertices;
    vector<unsigned int> indices;

    //  generate vertices
    for (int i = 0; i <= stacks; i++) {
        float phi = M_PI * i / stacks;  //  vertical angle
        float sinPhi = sin(phi);
        float cosPhi = cos(phi);

        for (int j = 0; j <= segments; ++j) {
            float theta = 2.0f * M_PI * j / segments;  //  horizontal angle
            float sinTheta = sin(theta);
            float cosTheta = cos(theta);

            float x = radius * sinPhi * cosTheta;
            float y = radius * cosPhi;
            float z = radius * sinPhi * sinTheta;

            vertices.push_back(VertexFormat(glm::vec3(x, y, z), color));
        }
    }

    //  generate indices
    for (int i = 0; i < stacks; i++) {
        int k1 = i * (segments + 1);
        int k2 = k1 + segments + 1;
        int j = 0;

        while(j < segments) {
            if (i != 0) {  //  bottom of the sphere
                indices.push_back(k1);
                indices.push_back(k2);
                indices.push_back(k1 + 1);
            }

            if (i != (stacks - 1)) {  // top of the sphere
                indices.push_back(k1 + 1);
                indices.push_back(k2);
                indices.push_back(k2 + 1);
            }

            j++; k1++; k2++;
        }
    }

    Mesh* sphere = new Mesh(name);
    sphere->InitFromData(vertices, indices);
    return sphere;
}

Mesh* MeshRenderer::GenerateConeTree(float bottomConeRadius, float upConeRadius,
                    float heightBottomCone, float heightUpCone, glm::vec3 centerBottomCone, glm::vec3 centerUpCone,
                    glm::vec3 colorBottomCone, glm::vec3 colorUpCone, int segments) {
    
    Mesh* bottomCone = GenerateCone(bottomConeRadius, heightBottomCone, segments, centerBottomCone, colorBottomCone);
    Mesh* upCone = GenerateCone(upConeRadius, heightUpCone, segments, centerUpCone, colorUpCone);

    vector<VertexFormat> vertices;
    vector<unsigned int> indices;

    //  append vertices of both cones
    vertices.insert(vertices.end(), bottomCone->vertices.begin(), bottomCone->vertices.end());
    vertices.insert(vertices.end(), upCone->vertices.begin(), upCone->vertices.end());

    /*  append indices of both cones
        update the indices for the second cone considering the ones for the first cone
    */
    indices.insert(indices.end(), bottomCone->indices.begin(), bottomCone->indices.end());
    int bottomConeVertexCount = bottomCone->vertices.size();
    for (unsigned int index: upCone->indices) {
        indices.push_back(index + bottomConeVertexCount);
    }

    Mesh* coneTree = new Mesh("coneTree");
    coneTree->InitFromData(vertices, indices);
    return coneTree;
}

Mesh* MeshRenderer::GenerateTree(float radiusTrunk, float heightTrunk, glm::vec3 colorTrunk, int segments,
                    float bottomConeRadius, float upConeRadius, float heightBottomCone, float heightUpCone, 
                    glm::vec3 centerBottomCone, glm::vec3 centerUpCone, glm::vec3 colorBottomCone, glm::vec3 colorUpCone) {
    
    Mesh* treeTrunk = GenerateCylinder(radiusTrunk, heightTrunk, segments, colorTrunk);
    Mesh* treeCone = GenerateConeTree(bottomConeRadius, upConeRadius, heightBottomCone, heightUpCone, centerBottomCone, 
                                      centerUpCone, colorBottomCone, colorUpCone, segments);

    vector<VertexFormat> vertices;
    vector<unsigned int> indices;

    //  append vertices of both forms
    vertices.insert(vertices.end(), treeTrunk->vertices.begin(), treeTrunk->vertices.end());
    vertices.insert(vertices.end(), treeCone->vertices.begin(), treeCone->vertices.end());

    //  append indices of both forms, updating the indices of the second form
    indices.insert(indices.end(), treeTrunk->indices.begin(), treeTrunk->indices.end());
    int treeTrunkVertexCount = treeTrunk->vertices.size();
    for (unsigned int index: treeCone->indices) {
        indices.push_back(index + treeTrunkVertexCount);
    }

    Mesh* tree = new Mesh("tree");
    tree->InitFromData(vertices, indices);
    return tree;
}

vector<Mesh*> MeshRenderer::GenerateRandomBuildings(float size) {
    vector<Mesh*> buildings;
    
    random_device rd;
    mt19937 gen(rd());
    
    //  generate random color for buildings
    uniform_real_distribution<float> colorDist(0.0f, 1.0f);
    
    //  generate five cubes of random colors
    for (int i = 0; i < 5; i++) {
        glm::vec3 color(colorDist(gen), colorDist(gen), colorDist(gen));
        
        Mesh* building = GenerateCube("building", size, color);
        buildings.push_back(building);
    }
    
    return buildings;
}

glm::vec3 GenerateRandomGreenColor() {
    //  generate random color of green for the tree cone
    random_device rd;
    mt19937 gen(rd());

    uniform_real_distribution<> greenDis(0.4f, 1.0f);
    uniform_real_distribution<> redDis(0.0f, 0.4f);
    uniform_real_distribution<> blueDis(0.0f, 0.4f);

    float red = redDis(gen);
    float green = greenDis(gen);
    float blue = blueDis(gen);

    return glm::vec3(red, green, blue);
}

glm::vec3 GenerateRandomBrownColor() {
    //  generate random color of brown for the tree cylinder
    random_device rd;
    mt19937 gen(rd());

    uniform_real_distribution<> redDis(0.3f, 0.8f);
    uniform_real_distribution<> greenDis(0.2f, 0.6f);
    uniform_real_distribution<> blueDis(0.0f, 0.2f);

    float red = redDis(gen);
    float green = greenDis(gen);
    float blue = blueDis(gen);

    return glm::vec3(red, green, blue);
}

vector<Mesh*> MeshRenderer::GenerateRandomTrees(float height) {
    vector<Mesh*> trees;

    random_device rd;
    mt19937 gen(rd());
    
    /*  generate 10 trees of with random color for the cone tree and random color for the cylinder tree
        the cone tree is created with two glued cones
    */
    for (int i = 0; i < 10; i++) {
        float bottomConeRadius = 2.5f;
        float upConeRadius = 2.5f;
        float heightBottomCone = height * 2.0f;
        float heightUpCone = height * 2.0f;

        float heightTrunk = height * 2.0f;
        float radiusTrunk = 0.8f;
        glm::vec3 colorTrunk = GenerateRandomBrownColor();

        glm::vec3 centerBottomCone(0, heightTrunk, 0);
        glm::vec3 colorBottomCone = GenerateRandomGreenColor();

        glm::vec3 centerUpCone(0, heightTrunk + heightBottomCone / 2, 0);
        glm::vec3 colorUpCone = colorBottomCone;

        Mesh* tree = GenerateTree(radiusTrunk, heightTrunk, colorTrunk, 16, bottomConeRadius, upConeRadius,       
                                heightBottomCone, heightUpCone, centerBottomCone, centerUpCone,          
                                colorBottomCone, colorUpCone);
        
        trees.push_back(tree);  //  save trees in an array
    } 
    
    return trees;
}

vector<Mesh*> MeshRenderer::GenerateRandomClouds(int nums, float minSize, float maxSize) {
    vector<Mesh*> clouds;
    random_device rd;
    mt19937 gen(rd());  

    uniform_real_distribution<float> sizeDist(minSize, maxSize);
    uniform_real_distribution<float> brightnessDist(0.8f, 1.0f);
    
    const int spheresPerCloud = 3;  //  each cloud has exactly 3 spheres
    
    for (int i = 0; i < nums; i++) {
        //  generate nums clouds of different sizes and different brightness 
        float cloudSize = sizeDist(gen);
        float brightness = brightnessDist(gen);
        glm::vec3 color(brightness, brightness, brightness);
        
        //  create 3 identical spheres for each cloud
        for (int j = 0; j < spheresPerCloud; j++) {
            Mesh* sphere = GenerateSphere("cloud" + to_string(i) + "_sphere" + to_string(j),
                cloudSize * 0.5f, 12, 12, color);
            clouds.push_back(sphere);
        }
    }

    return clouds;
}

Mesh* MeshRenderer::CreateOutsideSquare(const std::string &name, float length, glm::vec3 color) {
    vector<VertexFormat> vertices = {
        VertexFormat(glm::vec3(-length / 2, -length / 2, 0), color),
        VertexFormat(glm::vec3(length / 2, -length / 2, 0), color),
        VertexFormat(glm::vec3(length / 2, length / 2, 0), color),
        VertexFormat(glm::vec3(-length / 2, length / 2, 0), color),
    };

    Mesh* square = new Mesh(name);
    vector<unsigned int> indices = { 0, 1, 2, 3, 0 };

    square->SetDrawMode(GL_LINE_STRIP);
    square->InitFromData(vertices, indices);
    return square;
}

Mesh* MeshRenderer::CreateInnerSquare(const std::string &name, float length, glm::vec3 color) {
    vector<VertexFormat> vertices = {
        VertexFormat(glm::vec3(-length / 2, -3.0f * length / 2, 0), color),  //  1
        VertexFormat(glm::vec3(-length / 2, -length / 2, 0), color),         //  2
        VertexFormat(glm::vec3(-length / 2, length / 2, 0), color),          //  3
        VertexFormat(glm::vec3(length / 2, length / 2, 0), color),           //  4
        VertexFormat(glm::vec3(length / 2, -length / 2, 0), color),          //  5
        VertexFormat(glm::vec3(length / 2, -3.0f * length / 2, 0), color)    //  6
    };

    Mesh* square = new Mesh(name);
    //  drawing sequence: 1->2->5->4->3->2->5->6
    vector<unsigned int> indices = { 0, 1, 4, 3, 2, 1, 4, 5 };

    square->SetDrawMode(GL_LINE_STRIP);
    square->InitFromData(vertices, indices);
    return square;
}

Mesh* MeshRenderer::CreateTriangle(const std::string &name, float length) {
    vector<VertexFormat> vertices = {
        VertexFormat(glm::vec3(-length / 2, 0, 0), glm::vec3(0.973f, 0.514f, 0.475f)),  //  1
        VertexFormat(glm::vec3(length / 2, 0, 0), glm::vec3(0.973f, 0.514f, 0.475f)),   //  2
        VertexFormat(glm::vec3(0, 0, -length), glm::vec3(0,0,0))                        //  3
    };

    Mesh* triangle = new Mesh(name);
    
    vector<unsigned int> indices = { 0, 1, 2, 0 };

    triangle->SetDrawMode(GL_TRIANGLES);
    triangle->InitFromData(vertices, indices);
    return triangle;
}