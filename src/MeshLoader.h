#pragma once

struct MeshLoader
{
	std::vector<float> load_model_from_file(const char* filename, glm::mat4 preTransform);
};
