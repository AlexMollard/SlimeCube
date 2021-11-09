#include "Primitives.h"

#define PI 3.14159265358979323846264338327950288
#define TWOPI (2.0*3.14159265358979323846264338327950288)

Plane::Plane()
{
	vertices.push_back(glm::vec3(-1.0f, 1.0f, 0.0f));	// Back-Left	0
	vertices.push_back(glm::vec3(1.0f, 1.0f, 0.0f));	// Back-Right	1
	vertices.push_back(glm::vec3(-1.0f, -1.0f, 0.0f));	// Front-Left	2
	vertices.push_back(glm::vec3(1.0f, -1.0f, 0.0f));	//Front-Right	3

	unsigned int planeIndices[] =
	{
		0, 2, 1,	// first triangle
		1, 2, 3		// second triangle
	};

	uvs.push_back(glm::vec2(0, 1));
	uvs.push_back(glm::vec2(1, 1));
	uvs.push_back(glm::vec2(0, 0));
	uvs.push_back(glm::vec2(1, 0));

	normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
	normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
	normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
	normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));

	int length = sizeof(planeIndices) / sizeof(unsigned int);

	indices = std::vector<unsigned int>(planeIndices, planeIndices + length);
}

Cube::Cube()
{
	vertices.push_back(glm::vec3(1.0f, 1.0f, -1.0f));    // Top Right Of The Quad (Top)
	vertices.push_back(glm::vec3(-1.0f, 1.0f, -1.0f));    // Top Left Of The Quad (Top)
	vertices.push_back(glm::vec3(-1.0f, 1.0f, 1.0f));    // Bottom Left Of The Quad (Top)
	vertices.push_back(glm::vec3(1.0f, 1.0f, 1.0f));    // Bottom Right Of The Quad (Top)

	vertices.push_back(glm::vec3(1.0f, -1.0f, 1.0f));    // Top Right Of The Quad (Bottom)
	vertices.push_back(glm::vec3(-1.0f, -1.0f, 1.0f));    // Top Left Of The Quad (Bottom)
	vertices.push_back(glm::vec3(-1.0f, -1.0f, -1.0f));    // Bottom Left Of The Quad (Bottom)
	vertices.push_back(glm::vec3(1.0f, -1.0f, -1.0f));    // Bottom Right Of The Quad (Bottom

	vertices.push_back(glm::vec3(1.0f, 1.0f, 1.0f));    // Top Right Of The Quad (Front)
	vertices.push_back(glm::vec3(-1.0f, 1.0f, 1.0f));    // Top Left Of The Quad (Front)
	vertices.push_back(glm::vec3(-1.0f, -1.0f, 1.0f));    // Bottom Left Of The Quad (Front)
	vertices.push_back(glm::vec3(1.0f, -1.0f, 1.0f));    // Bottom Right Of The Quad (Front)

	vertices.push_back(glm::vec3(1.0f, -1.0f, -1.0f));    // Top Right Of The Quad (Back)
	vertices.push_back(glm::vec3(-1.0f, -1.0f, -1.0f));    // Top Left Of The Quad (Back)
	vertices.push_back(glm::vec3(-1.0f, 1.0f, -1.0f));    // Bottom Left Of The Quad (Back)
	vertices.push_back(glm::vec3(1.0f, 1.0f, -1.0f));    // Bottom Right Of The Quad (Back)

	vertices.push_back(glm::vec3(-1.0f, 1.0f, 1.0f));    // Top Right Of The Quad (Left)
	vertices.push_back(glm::vec3(-1.0f, 1.0f, -1.0f));    // Top Left Of The Quad (Left)
	vertices.push_back(glm::vec3(-1.0f, -1.0f, -1.0f));    // Bottom Left Of The Quad (Left)
	vertices.push_back(glm::vec3(-1.0f, -1.0f, 1.0f));    // Bottom Right Of The Quad (Left)

	vertices.push_back(glm::vec3(1.0f, 1.0f, -1.0f));    // Top Right Of The Quad (Right)
	vertices.push_back(glm::vec3(1.0f, 1.0f, 1.0f));    // Top Left Of The Quad (Right)
	vertices.push_back(glm::vec3(1.0f, -1.0f, 1.0f));    // Bottom Left Of The Quad (Right)
	vertices.push_back(glm::vec3(1.0f, -1.0f, -1.0f));    // Bottom Right Of The Quad (Right)

	unsigned int cubeIndices[] =
	{
		0, 1, 3,	// Top
		1, 2, 3,	// Top

		5, 7, 4,	// Bottom
		7, 5, 6,	// Bottom

		8, 9, 10,	// Front
		10, 11, 8,	//  Front

		12, 13, 14,	// Back
		14, 15, 12,	//  Back

		16, 17, 18,	// Left
		18, 19, 16,	//  Left

		20, 21, 22,	// Right
		22, 23, 20	//  Right
	};

	int length = sizeof(cubeIndices) / sizeof(unsigned int);
	indices = std::vector<unsigned int>(cubeIndices, cubeIndices + length);

	normals = CalculateVertNormals(vertices, indices);

	float cubeUvData[] =
	{
		1.0f, 0.0f,	   // Top Right Of The Quad (Top)
		0.0f, 0.0f,	    // Top Left Of The Quad (Top)
		0.0f, 1.0f,	   // Bottom Left Of The Quad (Top)
		1.0f, 1.0f,	  // Bottom Right Of The Quad (Top)

		1.0f, 0.0f,	   // Top Right Of The Quad (Bottom)
		0.0f, 0.0f,	    // Top Left Of The Quad (Bottom)
		0.0f, 1.0f,	     // Bottom Left Of The Quad (Bottom)
		1.0f, 1.0f,	    // Bottom Right Of The Quad (Bottom

		1.0f, 0.0f,	  // Top Right Of The Quad (Front)
		0.0f, 0.0f,	   // Top Left Of The Quad (Front)
		0.0f, 1.0f,	    // Bottom Left Of The Quad (Front)
		1.0f, 1.0f,	   // Bottom Right Of The Quad (Front)

		1.0f, 1.0f,	    // Top Right Of The Quad (Back)
		0.0f, 1.0f,	     // Top Left Of The Quad (Back)
		0.0f, 0.0f,	    // Bottom Left Of The Quad (Back)
		1.0f, 0.0f,	   // Bottom Right Of The Quad (Back)

		1.0f, 0.0f,	   // Top Right Of The Quad (Left)
		0.0f, 0.0f,	    // Top Left Of The Quad (Left)
		0.0f, 1.0f,	     // Bottom Left Of The Quad (Left)
		1.0f, 1.0f,	    // Bottom Right Of The Quad (Left)

		1.0f, 0.0f,	   // Top Right Of The Quad (Right)
		0.0f, 0.0f,	  // Top Left Of The Quad (Right)
		0.0f, 1.0f,	   // Bottom Left Of The Quad (Right)
		1.0f, 1.0f	    // Bottom Right Of The Quad (Right)
	};

	glm::vec2* arr = (glm::vec2*)cubeUvData;
	int n = sizeof(cubeUvData) / sizeof(cubeUvData[0]);
	uvs = std::vector<glm::vec2>(arr, arr + n);

	CalculateTangents();
}

Cylinder::Cylinder(float radius, float halfLength, int slices)
{
	vertices.push_back(glm::vec3(0.0f, halfLength, 0.0f));
	vertices.push_back(glm::vec3(0.0f, -halfLength, 0.0f));

	for (int i = 0; i < slices; i++)
	{
		float theta = TWOPI * ((float)i / slices);

		vertices.push_back(glm::vec3(radius * glm::cos(theta), halfLength, radius * glm::sin(theta)));
		vertices.push_back(glm::vec3(radius * glm::cos(theta), -halfLength, radius * glm::sin(theta)));
	}

	for (int i = 2; i < vertices.size() - 2; i += 2)
	{
		indices.push_back(i + 2);
		indices.push_back(i);
		indices.push_back(0);

		indices.push_back(i + 3);
		indices.push_back(1);
		indices.push_back(i + 1);

		indices.push_back(i);
		indices.push_back(i + 2);
		indices.push_back(i + 1);

		indices.push_back(i + 2);
		indices.push_back(i + 3);
		indices.push_back(i + 1);
	}

	indices.push_back(2);
	indices.push_back(vertices.size() - 2);
	indices.push_back(0);

	indices.push_back(1);
	indices.push_back(vertices.size() - 1);
	indices.push_back(3);

	indices.push_back(vertices.size() - 2);
	indices.push_back(2);
	indices.push_back(vertices.size() - 1);

	indices.push_back(2);
	indices.push_back(3);
	indices.push_back(vertices.size() - 1);

	normals = CalculateVertNormals(vertices, indices);
}

Sphere::Sphere(float radius, float sectorCount, int stackCount)
{
	float x, y, z, xy;                              // vertex position
	float nx, ny, nz, lengthInv = 1.0f / radius;    // vertex normal
	float s, t;                                     // vertex texCoord

	float sectorStep = TWOPI / sectorCount;
	float stackStep = PI / stackCount;
	float sectorAngle, stackAngle;

	for (int i = 0; i <= stackCount; ++i)
	{
		stackAngle = PI / 2.0f - i * stackStep;        // starting from pi/2 to -pi/2
		xy = radius * cosf(stackAngle);             // r * cos(u)
		z = radius * sinf(stackAngle);              // r * sin(u)

		// add (sectorCount+1) vertices per stack
		// the first and last vertices have same position and normal, but different tex coords
		for (int j = 0; j <= sectorCount; ++j)
		{
			sectorAngle = j * sectorStep;           // starting from 0 to 2pi

			// vertex position (x, y, z)
			x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
			y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)

			vertices.push_back(glm::vec3(x, y, z));

			// normalized vertex normal (nx, ny, nz)
			nx = x * lengthInv;
			ny = y * lengthInv;
			nz = z * lengthInv;
			normals.push_back(glm::vec3(nx, ny, nz));

			// vertex tex coord (s, t) range between [0, 1]
			s = (float)j / sectorCount;
			t = (float)i / stackCount;
			uvs.push_back(glm::vec2(s, t));
		}
	}

	int k1, k2;
	for (int i = 0; i < stackCount; ++i)
	{
		k1 = i * (sectorCount + 1);     // beginning of current stack
		k2 = k1 + sectorCount + 1;      // beginning of next stack

		for (int j = 0; j < sectorCount; ++j, ++k1, ++k2)
		{
			// 2 triangles per sector excluding first and last stacks
			// k1 => k2 => k1+1
			if (i != 0)
			{
				indices.push_back(k1);
				indices.push_back(k2);
				indices.push_back(k1 + 1);
			}

			// k1+1 => k2 => k2+1
			if (i != (stackCount - 1))
			{
				indices.push_back(k1 + 1);
				indices.push_back(k2);
				indices.push_back(k2 + 1);
			}
		}
	}
}

Torus::Torus(double r, double c, int rSeg, int cSeg)
{
	cSeg = 100;
	for (int i = 0; i < rSeg; i++) {
		for (int j = 0; j <= cSeg; j++) {
			for (int k = 0; k <= 1; k++) {
				double s = (i + k) % rSeg + 0.5;
				double t = j % (cSeg + 1);

				double x = (c + r * cos(s * TWOPI / rSeg)) * cos(t * TWOPI / cSeg);
				double y = (c + r * cos(s * TWOPI / rSeg)) * sin(t * TWOPI / cSeg);
				double z = r * sin(s * TWOPI / rSeg);

				double u = (i + k) / (float)rSeg;
				double v = t / (float)cSeg;

				vertices.push_back(glm::vec3(2 * x, 2 * y, 2 * z));
				normals.push_back(glm::vec3(2 * x, 2 * y, 2 * z));
				uvs.push_back(glm::vec2(u, v));
			}
			if (j)
			{
				indices.push_back(vertices.size() - 4);
				indices.push_back(vertices.size() - 2);
				indices.push_back(vertices.size() - 3);
				indices.push_back(vertices.size() - 3);
				indices.push_back(vertices.size() - 2);
				indices.push_back(vertices.size() - 1);
			}
		}
	}

	normals = CalculateVertNormals(vertices, indices);
}

glm::vec3 Primitives::calculateFaceNormal(glm::vec3 a, glm::vec3 b, glm::vec3 c)
{
	return glm::cross(b - a, c - a);;
}

std::vector<glm::vec3> Primitives::CalculateVertNormals(std::vector<glm::vec3>& vertices, std::vector<unsigned int>& indices)
{
	std::vector<glm::vec3> newNormals;

	//Initialize Normals to Nothing
	for (int i = 0; i < vertices.size(); i++)
	{
		newNormals.push_back(glm::vec3(0));
	}

	//Loop Over Indicies 3 at a Time and Calculate the Face Normal for thoose 3 Verticies. Add Face normal onto Normals
	for (int i = 0; i < indices.size(); i += 3)
	{
		int a = indices[i];
		int b = indices[i + 1];
		int c = indices[i + 2];

		glm::vec3 FaceNormal = (calculateFaceNormal(vertices[a], vertices[b], vertices[c]));

		newNormals[a] += FaceNormal;
		newNormals[b] += FaceNormal;
		newNormals[c] += FaceNormal;
	}

	//Loop Over all Normals and Normalize them
	for (int i = 0; i < newNormals.size(); i++)
	{
		newNormals[i] = glm::normalize(newNormals[i]);
	}

	return newNormals;
}

void Primitives::CalculateTangents()
{
	for (int i = 0; i < indices.size(); i += 3)
	{
		int i0 = indices[i];
		int i1 = indices[i + 1];
		int i2 = indices[i + 2];

		glm::vec3& v0 = vertices[i0];
		glm::vec3& v1 = vertices[i1];
		glm::vec3& v2 = vertices[i2];

		glm::vec2& uv0 = uvs[i0];
		glm::vec2& uv1 = uvs[i1];
		glm::vec2& uv2 = uvs[i2];

		glm::vec3 deltaPos1 = v1 - v0;
		glm::vec3 deltaPos2 = v2 - v0;

		glm::vec2 deltaUV1 = uv1 - uv0;
		glm::vec2 deltaUV2 = uv2 - uv0;

		float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
		glm::vec3 tangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y) * r;
		glm::vec3 bitangent = (deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x) * r;

		tangents.push_back(tangent);
		tangents.push_back(tangent);
		tangents.push_back(tangent);

		biTangents.push_back(bitangent);
		biTangents.push_back(bitangent);
		biTangents.push_back(bitangent);
	}
}

SkyBox::SkyBox()
{
	vertices.push_back(glm::vec3(1.0f, 1.0f, -1.0f) * glm::vec3(-1));    // Top Right Of The Quad (Top)
	vertices.push_back(glm::vec3(-1.0f, 1.0f, -1.0f) * glm::vec3(-1));    // Top Left Of The Quad (Top)
	vertices.push_back(glm::vec3(-1.0f, 1.0f, 1.0f) * glm::vec3(-1));    // Bottom Left Of The Quad (Top)
	vertices.push_back(glm::vec3(1.0f, 1.0f, 1.0f) * glm::vec3(-1));    // Bottom Right Of The Quad (Top)

	vertices.push_back(glm::vec3(1.0f, -1.0f, 1.0f) * glm::vec3(-1));    // Top Right Of The Quad (Bottom)
	vertices.push_back(glm::vec3(-1.0f, -1.0f, 1.0f) * glm::vec3(-1));    // Top Left Of The Quad (Bottom)
	vertices.push_back(glm::vec3(-1.0f, -1.0f, -1.0f) * glm::vec3(-1));    // Bottom Left Of The Quad (Bottom)
	vertices.push_back(glm::vec3(1.0f, -1.0f, -1.0f) * glm::vec3(-1));    // Bottom Right Of The Quad (Bottom

	vertices.push_back(glm::vec3(1.0f, 1.0f, 1.0f) * glm::vec3(-1));    // Top Right Of The Quad (Front)
	vertices.push_back(glm::vec3(-1.0f, 1.0f, 1.0f) * glm::vec3(-1));    // Top Left Of The Quad (Front)
	vertices.push_back(glm::vec3(-1.0f, -1.0f, 1.0f) * glm::vec3(-1));    // Bottom Left Of The Quad (Front)
	vertices.push_back(glm::vec3(1.0f, -1.0f, 1.0f) * glm::vec3(-1));    // Bottom Right Of The Quad (Front)

	vertices.push_back(glm::vec3(1.0f, -1.0f, -1.0f) * glm::vec3(-1));    // Top Right Of The Quad (Back)
	vertices.push_back(glm::vec3(-1.0f, -1.0f, -1.0f) * glm::vec3(-1));    // Top Left Of The Quad (Back)
	vertices.push_back(glm::vec3(-1.0f, 1.0f, -1.0f) * glm::vec3(-1));    // Bottom Left Of The Quad (Back)
	vertices.push_back(glm::vec3(1.0f, 1.0f, -1.0f) * glm::vec3(-1));    // Bottom Right Of The Quad (Back)

	vertices.push_back(glm::vec3(-1.0f, 1.0f, 1.0f) * glm::vec3(-1));    // Top Right Of The Quad (Left)
	vertices.push_back(glm::vec3(-1.0f, 1.0f, -1.0f) * glm::vec3(-1));    // Top Left Of The Quad (Left)
	vertices.push_back(glm::vec3(-1.0f, -1.0f, -1.0f) * glm::vec3(-1));    // Bottom Left Of The Quad (Left)
	vertices.push_back(glm::vec3(-1.0f, -1.0f, 1.0f) * glm::vec3(-1));    // Bottom Right Of The Quad (Left)

	vertices.push_back(glm::vec3(1.0f, 1.0f, -1.0f) * glm::vec3(-1));    // Top Right Of The Quad (Right)
	vertices.push_back(glm::vec3(1.0f, 1.0f, 1.0f) * glm::vec3(-1));    // Top Left Of The Quad (Right)
	vertices.push_back(glm::vec3(1.0f, -1.0f, 1.0f) * glm::vec3(-1));    // Bottom Left Of The Quad (Right)
	vertices.push_back(glm::vec3(1.0f, -1.0f, -1.0f) * glm::vec3(-1));    // Bottom Right Of The Quad (Right)

	unsigned int cubeIndices[] =
	{
		0, 1, 3,	// Top
		1, 2, 3,	// Top

		5, 7, 4,	// Bottom
		7, 5, 6,	// Bottom

		8, 9, 10,	// Front
		10, 11, 8,	//  Front

		12, 13, 14,	// Back
		14, 15, 12,	//  Back

		16, 17, 18,	// Left
		18, 19, 16,	//  Left

		20, 21, 22,	// Right
		22, 23, 20	//  Right
	};

	int length = sizeof(cubeIndices) / sizeof(unsigned int);
	indices = std::vector<unsigned int>(cubeIndices, cubeIndices + length);

	normals = CalculateVertNormals(vertices, indices);

	float cubeUvData[] =
	{
		1.0f, 0.0f,	   // Top Right Of The Quad (Top)
		0.0f, 0.0f,	    // Top Left Of The Quad (Top)
		0.0f, 1.0f,	   // Bottom Left Of The Quad (Top)
		1.0f, 1.0f,	  // Bottom Right Of The Quad (Top)

		1.0f, 0.0f,	   // Top Right Of The Quad (Bottom)
		0.0f, 0.0f,	    // Top Left Of The Quad (Bottom)
		0.0f, 1.0f,	     // Bottom Left Of The Quad (Bottom)
		1.0f, 1.0f,	    // Bottom Right Of The Quad (Bottom

		1.0f, 0.0f,	  // Top Right Of The Quad (Front)
		0.0f, 0.0f,	   // Top Left Of The Quad (Front)
		0.0f, 1.0f,	    // Bottom Left Of The Quad (Front)
		1.0f, 1.0f,	   // Bottom Right Of The Quad (Front)

		1.0f, 1.0f,	    // Top Right Of The Quad (Back)
		0.0f, 1.0f,	     // Top Left Of The Quad (Back)
		0.0f, 0.0f,	    // Bottom Left Of The Quad (Back)
		1.0f, 0.0f,	   // Bottom Right Of The Quad (Back)

		1.0f, 0.0f,	   // Top Right Of The Quad (Left)
		0.0f, 0.0f,	    // Top Left Of The Quad (Left)
		0.0f, 1.0f,	     // Bottom Left Of The Quad (Left)
		1.0f, 1.0f,	    // Bottom Right Of The Quad (Left)

		1.0f, 0.0f,	   // Top Right Of The Quad (Right)
		0.0f, 0.0f,	  // Top Left Of The Quad (Right)
		0.0f, 1.0f,	   // Bottom Left Of The Quad (Right)
		1.0f, 1.0f	    // Bottom Right Of The Quad (Right)
	};

	glm::vec2* arr = (glm::vec2*)cubeUvData;
	int n = sizeof(cubeUvData) / sizeof(cubeUvData[0]);
	uvs = std::vector<glm::vec2>(arr, arr + n);
}