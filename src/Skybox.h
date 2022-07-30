#pragma once

#include "Texture.h"

class Skybox : public Texture
{
  public:
    explicit Skybox(std::string facesDIR);

    explicit Skybox(const char* facesDIR);

    ~Skybox() override;

  private:
    std::vector<std::string> GetAllFiles(std::string dir);

    void CreateSkyBox(std::string facesDIR);
};
