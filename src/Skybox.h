#pragma once

#include "Texture.h"
#include <string>
#include <vector>

class Skybox : public Texture {
public:
	Skybox(std::string facesDIR);
	Skybox(const char* facesDIR);

    ~Skybox();

private:
    std::vector<std::string> GetAllFiles(std::string dir);

    void CreateSkyBox(std::string facesDIR);
};

