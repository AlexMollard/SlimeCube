#pragma once

class Primitives
{
public:
 enum TYPE
 {
  Plane,
  Cube,
  Cylinder,
  Sphere,
  Torus,
  SkyBox
 };

 TYPE type = TYPE::Cube;
 std::vector<glm::vec3> vertices;
 std::vector<unsigned int> indices;
 std::vector<glm::vec3> normals;
 std::vector<glm::vec3> triangles;
 std::vector<glm::vec3> tangents;
 std::vector<glm::vec3> biTangents;
 std::vector<glm::vec2> uvs;

 glm::vec3 calculateFaceNormal(glm::vec3 a, glm::vec3 b, glm::vec3 c);

 std::vector<glm::vec3> CalculateVertNormals(std::vector<glm::vec3> &vertices, std::vector<unsigned int> &indices);

 void CalculateTangents();
};

class Plane : public Primitives
{
 Plane();

public:
 TYPE type = TYPE::Plane;

 static Primitives Create() { return Plane(); }
};

class Cube : public Primitives
{
 Cube();

public:
 TYPE type = TYPE::Cube;

 static Primitives Create() { return Cube(); }
};

class Cylinder : public Primitives
{
 Cylinder(float radius, float halfLength, int slices);

public:
 TYPE type = TYPE::Cylinder;

 static Primitives Create(float radius, float halfLength, int slices) { return Cylinder(radius, halfLength, slices); }
};

class Sphere : public Primitives
{
 Sphere(float radius, float sectorCount, int stackCount);

public:
 TYPE type = TYPE::Sphere;

 static Primitives Create(float radius, float sectorCount, int stackCount) { return Sphere(radius, sectorCount, stackCount); }
};

class Torus : public Primitives
{
 Torus(double r = 0.07, double c = 0.05, int rSeg = 16, int cSeg = 100);

public:
 TYPE type = TYPE::Torus;

 static Primitives Create(double r = 0.07, double c = 0.05, int rSeg = 16, int cSeg = 100, int texture = 0)
 {
  return Torus(r, c, rSeg, cSeg);
 }
};

class SkyBox : public Primitives
{
 SkyBox();

public:
 TYPE type = TYPE::SkyBox;

 static Primitives Create() { return SkyBox(); }
};