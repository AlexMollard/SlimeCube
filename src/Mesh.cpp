#include "Mesh.h"
#define TINYOBJLOADER_IMPLEMENTATION // define this in only *one* .cc
// Optional. define TINYOBJLOADER_USE_MAPBOX_EARCUT gives robust trinagulation. Requires C++11
//#define TINYOBJLOADER_USE_MAPBOX_EARCUT
#include "objLoader.h"

Mesh::Mesh(const char* name, const char* dir)
{
	this->name = name;

	if (dir != nullptr)
		Load(dir);
}

Mesh::~Mesh()
{
	for (auto& c : m_meshChunks) {
		glDeleteVertexArrays(1, &c.vao);
		glDeleteBuffers(1, &c.vbo);
		glDeleteBuffers(1, &c.ibo); 
	}
}

void Mesh::Create(Primitives::TYPE type, float argOne, float argTwo, int argThree)
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

bool Mesh::Load(const char* filename, bool loadTextures, bool flipTextureV)
{
	this->fileName = filename;
	hasIBO = true;
	std::string inputfile = filename;
	tinyobj::ObjReaderConfig reader_config;

	tinyobj::ObjReader reader;

	if (!reader.ParseFromFile(inputfile, reader_config)) 
	{
		if (!reader.Error().empty()) {
			Log::Error("TinyObjReader: " + reader.Error());
		}
		exit(1);
	}

	if (!reader.Warning().empty()) {
		Log::Warn("TinyObjReader: " + reader.Warning());
	}

	auto& attrib = reader.GetAttrib();
	auto& shapes = reader.GetShapes();
	auto& materials = reader.GetMaterials();

	std::string folder = fileName.substr(0, fileName.find_last_of('/') + 1);

	// copy materials
	m_materials.resize(materials.size());
	int index = 0;
	for (auto& m : materials) {
		if (materials.size() > 1)
		{
			m_materials[index].ambient = glm::vec3(m.ambient[0], m.ambient[1], m.ambient[2]);
			m_materials[index].albedo = glm::vec3(m.diffuse[0], m.diffuse[1], m.diffuse[2]);
			m_materials[index].specular = glm::vec3(m.specular[0], m.specular[1], m.specular[2]);
			m_materials[index].emissive = glm::vec3(m.emission[0], m.emission[1], m.emission[2]);
			m_materials[index].specularPower = m.shininess;
			m_materials[index].opacity = m.dissolve;

			// textures

			if (m.ambient_texname.size() > 1)
				m_materials[index].ambientTexture.Load((folder + m.ambient_texname).c_str());

			if (m.diffuse_texname.size() > 1)
				m_materials[index].diffuseTexture.Load((folder + m.diffuse_texname).c_str());

			if (m.specular_texname.size() > 1)
				m_materials[index].specularTexture.Load((folder + m.specular_texname).c_str());

			if (m.specular_highlight_texname.size() > 1)
				m_materials[index].specularHighlightTexture.Load((folder + m.specular_highlight_texname).c_str());

			if (m.bump_texname.size() > 1)
				m_materials[index].normalTexture.Load((folder + m.bump_texname).c_str());

			if (m.displacement_texname.size() > 1)
				m_materials[index].displacementTexture.Load((folder + m.displacement_texname).c_str());
		}
		++index;
	}

	// copy shapes
	m_meshChunks.reserve(shapes.size());
	for (auto& shape : shapes)
	{
		// Loop over faces(polygon)
		size_t index_offset = 0;

		MeshChunk chunk;

		// create vertex data
		std::vector<Vertex> vertices;
		vertices.resize(shape.mesh.num_face_vertices.size() * 3);

		std::vector<unsigned int> indices;
		for (size_t f = 0; f < shape.mesh.num_face_vertices.size(); f++) {
			
			size_t fv = size_t(shape.mesh.num_face_vertices[f]); // 3
			indices.push_back(shape.mesh.indices[index_offset].vertex_index);

			// Loop over vertices in the face.
			for (size_t v = 0; v < fv; v++) {
				// access to vertex
				tinyobj::index_t idx = shape.mesh.indices[index_offset + v];


				tinyobj::real_t vx = attrib.vertices[3 * size_t(idx.vertex_index) + 0];
				tinyobj::real_t vy = attrib.vertices[3 * size_t(idx.vertex_index) + 1];
				tinyobj::real_t vz = attrib.vertices[3 * size_t(idx.vertex_index) + 2];

				vertices[v + f].position = glm::vec3(vx, vy, vz);

				// Check if `normal_index` is zero or positive. negative = no normal data
				if (idx.normal_index >= 0) {
					tinyobj::real_t nx = attrib.normals[3 * size_t(idx.normal_index) + 0];
					tinyobj::real_t ny = attrib.normals[3 * size_t(idx.normal_index) + 1];
					tinyobj::real_t nz = attrib.normals[3 * size_t(idx.normal_index) + 2];

					vertices[v + f].normal = glm::vec3(nx, ny, nz);
				}

				// Check if `texcoord_index` is zero or positive. negative = no texcoord data
				if (idx.texcoord_index >= 0) {
					tinyobj::real_t tx = attrib.texcoords[2 * size_t(idx.texcoord_index) + 0];
					tinyobj::real_t ty = attrib.texcoords[2 * size_t(idx.texcoord_index) + 1];

					vertices[v + f].texcoord = glm::vec2(tx, ty); // THIS MIGHT NEED TO BE FLIPPED
				}

				// Optional: vertex colors
				tinyobj::real_t red   = attrib.colors[3*size_t(idx.vertex_index)+0];
				tinyobj::real_t green = attrib.colors[3*size_t(idx.vertex_index)+1];
				tinyobj::real_t blue  = attrib.colors[3*size_t(idx.vertex_index)+2];
			}
			index_offset += fv;

		}

		// generate buffers
		glGenBuffers(1, &chunk.vbo);
		glGenBuffers(1, &chunk.ibo);
		glGenVertexArrays(1, &chunk.vao);

		// bind vertex array aka a mesh wrapper
		glBindVertexArray(chunk.vao);

		// bind vertex buffer
		glBindBuffer(GL_ARRAY_BUFFER, chunk.vbo);

		// fill vertex buffer
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

		// set the index buffer data
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, chunk.ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,
			indices.size() * sizeof(unsigned int),
			indices.data(), GL_STATIC_DRAW);

		// store index count for rendering
		chunk.indexCount = (unsigned int)indices.size();

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
		chunk.materialID = shape.mesh.material_ids.empty() ? -1 : shape.mesh.material_ids[0];

		m_meshChunks.push_back(chunk);

		// calculate for normal mapping
		//calculateTangents(vertices, shape.mesh.indices);
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

void Mesh::Draw(bool usePatches) {
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