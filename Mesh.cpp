#include "pch.h"
#include "Mesh.h"

Mesh::Mesh(const char* name, const char* dir)
{
	this->name = name;

	if (dir != nullptr)
		load(dir);
}

Mesh::~Mesh()
{
	for (auto& c : m_meshChunks) {
		glDeleteVertexArrays(1, &c.vao);
		glDeleteBuffers(1, &c.vbo);
		glDeleteBuffers(1, &c.ibo);
	}
}

void Mesh::create(Primitives::TYPE type, float argOne, float argTwo, int argThree)
{
	switch (type)
	{
	case Primitives::Plane:
		prim = Plane::Create();
		hasIBO = true;
		break;
	case Primitives::Cube:
		prim = Cube::Create();
		hasIBO = true;
		break;
	case Primitives::Cylinder:
		prim = Cylinder::Create(argOne, argTwo, argThree);
		hasIBO = true;
		break;
	case Primitives::Sphere:
		prim = Sphere::Create(argOne, argTwo, argThree);
		hasIBO = true;
		break;
	case Primitives::Torus:
		prim = Torus::Create(argOne, argTwo, argThree, 10);
		hasIBO = true;
		break;
	case Primitives::SkyBox:
		prim = SkyBox::Create();
		hasIBO = false;
		break;
	default:
		break;
	}

	MeshChunk chunk;

	// generate buffers
	glGenBuffers(1, &chunk.vbo);
	glGenBuffers(1, &chunk.ibo);
	glGenVertexArrays(1, &chunk.vao);

	// bind vertex array aka a mesh wrapper
	glBindVertexArray(chunk.vao);

	std::vector<float> newvertices;
	for (int i = 0; i < prim.vertices.size(); i++)
	{
		// Postitions
		newvertices.push_back(prim.vertices[i].x);
		newvertices.push_back(prim.vertices[i].y);
		newvertices.push_back(prim.vertices[i].z);

		// Normals
		if (prim.normals.size() < 1)
		{
			newvertices.push_back(1);
			newvertices.push_back(1);
			newvertices.push_back(1);
		}
		else
		{
			newvertices.push_back(prim.normals[i].x);
			newvertices.push_back(prim.normals[i].y);
			newvertices.push_back(prim.normals[i].z);
		}

		// UVS
		if (prim.uvs.size() < 1)
		{
			newvertices.push_back(0.5);
			newvertices.push_back(0.5);
		}
		else
		{
			newvertices.push_back(prim.uvs[i].x);
			newvertices.push_back(prim.uvs[i].y);
		}

		// Tangents
		if (prim.tangents.size() < 1)
		{
			newvertices.push_back(1);
			newvertices.push_back(1);
			newvertices.push_back(1);
		}
		else
		{
			newvertices.push_back(prim.tangents[i].x);
			newvertices.push_back(prim.tangents[i].y);
			newvertices.push_back(prim.tangents[i].z);
		}

		// Biangents
		if (prim.biTangents.size() < 1)
		{
			newvertices.push_back(1);
			newvertices.push_back(1);
			newvertices.push_back(1);
		}
		else
		{
			newvertices.push_back(prim.biTangents[i].x);
			newvertices.push_back(prim.biTangents[i].y);
			newvertices.push_back(prim.biTangents[i].z);
		}
	}

	// store index count for rendering
	chunk.indexCount = (unsigned int)prim.indices.size();

	// Fill vertex Buffer
	glBindBuffer(GL_ARRAY_BUFFER, chunk.vbo);
	glBufferData(GL_ARRAY_BUFFER, newvertices.size() * sizeof(float), newvertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, chunk.ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, prim.indices.size() * sizeof(unsigned int), prim.indices.data(), GL_STATIC_DRAW);

	// Enable first element as position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Enable second element as normals
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Enable third element as UVS
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// Enable third element as Tangents
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(8 * sizeof(float)));
	glEnableVertexAttribArray(3);

	// Enable third element as BiTangents
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(11 * sizeof(float)));
	glEnableVertexAttribArray(4);

	// Unbind buffer
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	m_meshChunks.push_back(chunk);

	return (void)0;
}

bool Mesh::load(const char* filename, bool loadTextures, bool flipTextureV)
{
	if (m_meshChunks.empty() == false) {
		printf("Mesh already initialised, can't re-initialise!\n");
		return false;
	}

	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string error = "";

	std::string file = filename;
	std::string folder = file.substr(0, file.find_last_of('\\') + 1);

	bool success = tinyobj::LoadObj(shapes, materials, error,
		filename, folder.c_str());

	//std::cout << error << std::endl;

	if (success == false) {
		printf("%s\n", error.c_str());
		return false;
	}

	fileName = filename;

	// copy materials
	m_materials.resize(materials.size());
	int index = 0;
	for (auto& m : materials) {
		if (materials.size() > 1)
		{
			m_materials[index].ambient = glm::vec3(m.ambient[0], m.ambient[1], m.ambient[2]);
			m_materials[index].albedo = glm::vec3(m.albedo[0], m.albedo[1], m.albedo[2]);
			m_materials[index].specular = glm::vec3(m.specular[0], m.specular[1], m.specular[2]);
			m_materials[index].emissive = glm::vec3(m.emission[0], m.emission[1], m.emission[2]);
			m_materials[index].specularPower = m.shininess;
			m_materials[index].opacity = m.dissolve;

			// textures

			if (m.ambient_texname.size() > 1)
				m_materials[index].ambientTexture.load((folder + m.ambient_texname).c_str());

			if (m.diffuse_texname.size() > 1)
				m_materials[index].diffuseTexture.load((folder + m.diffuse_texname).c_str());

			if (m.specular_texname.size() > 1)
				m_materials[index].specularTexture.load((folder + m.specular_texname).c_str());

			if (m.specular_highlight_texname.size() > 1)
				m_materials[index].specularHighlightTexture.load((folder + m.specular_highlight_texname).c_str());

			if (m.bump_texname.size() > 1)
				m_materials[index].normalTexture.load((folder + m.bump_texname).c_str());

			if (m.displacement_texname.size() > 1)
				m_materials[index].displacementTexture.load((folder + m.displacement_texname).c_str());
		}
		++index;
	}

	// copy shapes
	m_meshChunks.reserve(shapes.size());

	for (auto& s : shapes) {
		MeshChunk chunk;

		// generate buffers
		glGenBuffers(1, &chunk.vbo);
		glGenBuffers(1, &chunk.ibo);
		glGenVertexArrays(1, &chunk.vao);

		// bind vertex array aka a mesh wrapper
		glBindVertexArray(chunk.vao);

		// set the index buffer data
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, chunk.ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,
			s.mesh.indices.size() * sizeof(unsigned int),
			s.mesh.indices.data(), GL_STATIC_DRAW);

		// store index count for rendering
		chunk.indexCount = (unsigned int)s.mesh.indices.size();

		// create vertex data
		std::vector<Vertex> vertices;
		vertices.resize(s.mesh.positions.size() / 3);
		size_t vertCount = vertices.size();

		bool hasPosition = s.mesh.positions.empty() == false;
		bool hasNormal = s.mesh.normals.empty() == false;
		bool hasTexture = s.mesh.texcoords.empty() == false;

		for (size_t i = 0; i < vertCount; ++i) {
			if (hasPosition)
				vertices[i].position = glm::vec3(s.mesh.positions[i * 3 + 0], s.mesh.positions[i * 3 + 1], s.mesh.positions[i * 3 + 2]);
			if (hasNormal)
				vertices[i].normal = glm::vec3(s.mesh.normals[i * 3 + 0], s.mesh.normals[i * 3 + 1], s.mesh.normals[i * 3 + 2]);

			// flip the T / V (might not always be needed, depends on how mesh was made)
			if (hasTexture)
				vertices[i].texcoord = glm::vec2(s.mesh.texcoords[i * 2 + 0], flipTextureV ? 1.0f - s.mesh.texcoords[i * 2 + 1] : s.mesh.texcoords[i * 2 + 1]);
		}

		// calculate for normal mapping
		if (hasNormal && hasTexture)
			calculateTangents(vertices, s.mesh.indices);

		// bind vertex buffer
		glBindBuffer(GL_ARRAY_BUFFER, chunk.vbo);

		// fill vertex buffer
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

		// Enable first element as position
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 15 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		// Enable second element as normals
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 15 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		// Enable third element as UVS
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 15 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);

		// Enable third element as Tangents
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 15 * sizeof(float), (void*)(8 * sizeof(float)));
		glEnableVertexAttribArray(3);

		// Enable third element as BiTangents
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 15 * sizeof(float), (void*)(12 * sizeof(float)));
		glEnableVertexAttribArray(4);

		// bind 0 for safety
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		// set chunk material
		chunk.materialID = s.mesh.material_ids.empty() ? -1 : s.mesh.material_ids[0];

		m_meshChunks.push_back(chunk);
	}

	// load obj
	return true;
}

void BindTexture(Texture* texture, int textureIndex)
{
	if (texture != nullptr)
	{
		glActiveTexture(GL_TEXTURE0 + textureIndex);
		texture->Bind();
	}
	else
	{
		glActiveTexture(GL_TEXTURE0 + textureIndex);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void Mesh::draw(bool usePatches) {
	if (m_materials.size() > 1)
	{
		int program = -1;
		glGetIntegerv(GL_CURRENT_PROGRAM, &program);

		if (program == -1) {
			printf("No shader bound!\n");
			return;
		}

		int specularStrengthUniform = glGetUniformLocation(program, "specularStrength");

		int ambientTexUniform = glGetUniformLocation(program, "ambientTexture");
		int diffuseTexUniform = glGetUniformLocation(program, "diffuseTexture");
		int specTexUniform = glGetUniformLocation(program, "specularTexture");
		int specHighlightTexUniform = glGetUniformLocation(program, "roughTexture");
		int normalTexUniform = glGetUniformLocation(program, "normalTexture");
		int dispTexUniform = glGetUniformLocation(program, "displacementTexture");

		// set texture slots (these don't change per material)
		if (diffuseTexUniform >= 0)
			glUniform1i(diffuseTexUniform, 0);
		if (specTexUniform >= 0)
			glUniform1i(specTexUniform, 1);
		if (normalTexUniform >= 0)
			glUniform1i(normalTexUniform, 2);
		if (ambientTexUniform >= 0)
			glUniform1i(ambientTexUniform, 3);
		if (specHighlightTexUniform >= 0)
			glUniform1i(specHighlightTexUniform, 4);
		if (dispTexUniform >= 0)
			glUniform1i(dispTexUniform, 5);

		// draw the mesh chunks
		int count = -1;
		for (auto& c : m_meshChunks) {
			if (m_materials[c.materialID].diffuseTexture.GetID() != 0)
				BindTexture(&m_materials[c.materialID].diffuseTexture, 0);

			if (m_materials[c.materialID].specularTexture.GetID() != 0)
				BindTexture(&m_materials[c.materialID].specularTexture, 1);

			if (m_materials[c.materialID].normalTexture.GetID() != 0)
				BindTexture(&m_materials[c.materialID].normalTexture, 2);

			if (m_materials[c.materialID].ambientTexture.GetID() != 0)
				BindTexture(&m_materials[c.materialID].ambientTexture, 3);

			if (m_materials[c.materialID].specularHighlightTexture.GetID() != 0)
				BindTexture(&m_materials[c.materialID].specularHighlightTexture, 4);

			if (m_materials[c.materialID].displacementTexture.GetID() != 0)
				BindTexture(&m_materials[c.materialID].displacementTexture, 5);

			// bind and draw geometry
			glBindVertexArray(c.vao);
			if (usePatches)
				glDrawElements(GL_PATCHES, c.indexCount, GL_UNSIGNED_INT, 0);
			else
				glDrawElements(GL_TRIANGLES, c.indexCount, GL_UNSIGNED_INT, 0);
		}
	}
	else
	{
		for (auto& c : m_meshChunks) {
			// bind and draw geometry
			glBindVertexArray(c.vao);
			if (usePatches)
				glDrawElements(GL_PATCHES, c.indexCount, GL_UNSIGNED_INT, 0);
			else
				glDrawElements(GL_TRIANGLES, c.indexCount, GL_UNSIGNED_INT, 0);
		}
	}
}

void Mesh::calculateTangents(std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices) {
	unsigned int vertexCount = (unsigned int)vertices.size();
	glm::vec4* tan1 = new glm::vec4[vertexCount * 2];
	glm::vec4* tan2 = tan1 + vertexCount;
	memset(tan1, 0, vertexCount * sizeof(glm::vec4) * 2);

	unsigned int indexCount = (unsigned int)indices.size();
	for (unsigned int a = 0; a < indexCount; a += 3) {
		long i1 = indices[a];
		long i2 = indices[a + 1];
		long i3 = indices[a + 2];

		const glm::vec3& v1 = vertices[i1].position;
		const glm::vec3& v2 = vertices[i2].position;
		const glm::vec3& v3 = vertices[i3].position;

		const glm::vec2& w1 = vertices[i1].texcoord;
		const glm::vec2& w2 = vertices[i2].texcoord;
		const glm::vec2& w3 = vertices[i3].texcoord;

		float x1 = v2.x - v1.x;
		float x2 = v3.x - v1.x;
		float y1 = v2.y - v1.y;
		float y2 = v3.y - v1.y;
		float z1 = v2.z - v1.z;
		float z2 = v3.z - v1.z;

		float s1 = w2.x - w1.x;
		float s2 = w3.x - w1.x;
		float t1 = w2.y - w1.y;
		float t2 = w3.y - w1.y;

		float r = 1.0F / (s1 * t2 - s2 * t1);
		glm::vec4 sdir((t2 * x1 - t1 * x2) * r, (t2 * y1 - t1 * y2) * r,
			(t2 * z1 - t1 * z2) * r, 0);
		glm::vec4 tdir((s1 * x2 - s2 * x1) * r, (s1 * y2 - s2 * y1) * r,
			(s1 * z2 - s2 * z1) * r, 0);

		tan1[i1] += sdir;
		tan1[i2] += sdir;
		tan1[i3] += sdir;

		tan2[i1] += tdir;
		tan2[i2] += tdir;
		tan2[i3] += tdir;
	}

	for (unsigned int a = 0; a < vertexCount; a++) {
		const glm::vec3& n = glm::vec3(vertices[a].normal);
		const glm::vec3& t = glm::vec3(tan1[a]);

		// Gram-Schmidt orthogonalize
		vertices[a].tangent = glm::vec4(glm::normalize(t - n * glm::dot(n, t)), 0);

		// Calculate handedness (direction of bitangent)
		vertices[a].tangent.w = (glm::dot(glm::cross(glm::vec3(n), glm::vec3(t)), glm::vec3(tan2[a])) < 0.0F) ? 1.0F : -1.0F;

		vertices[a].bitangent = glm::cross(n, t);
	}

	delete[] tan1;
}