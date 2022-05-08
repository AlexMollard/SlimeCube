#pragma once

#include "glew/glew.h"
#include "GLFW/glfw3.h"
#include "Resource.h"
#include <string>
#include <memory>

class Texture : private Resource {
public:
    Texture(std::string name, const std::string& dir);

    Texture(std::string name, unsigned int id);

    Texture(std::string name, const unsigned int *id);

    Texture(const std::string& dir, void *args);

    explicit Texture(std::string name);

    Texture();

    ~Texture();

    void SetUpTextureID();

    Texture* Load(const std::string& dir);

    void Bind() { glBindTexture(GL_TEXTURE_2D, textureID); };

    unsigned int GetID() const { return textureID; };

    void SetID(unsigned int newID) { textureID = newID; };

    const std::string& GetName() { return name; };

    const char *GetNameC() { return name.c_str(); };

    static std::shared_ptr<Texture> Create(std::string const &dir);

    static std::shared_ptr<Texture> Create(const char* dir);

    uint32_t textureID = 0;
};

