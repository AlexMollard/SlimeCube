#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"

Texture::Texture(std::string name, const std::string& dir)
{
    this->name = std::move(name);
    Log::Trace(dir);
    SetUpTextureID();

    // Load Image and generate mipmaps
    stbi_set_flip_vertically_on_load(1);
    int width;
    int height;
    int nrChannels;
    unsigned char* data = stbi_load(dir.c_str(), &width, &height, &nrChannels, 0);

    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, nrChannels != 4 ? GL_RGB : GL_RGBA, width, height, 0, nrChannels != 4 ? GL_RGB : GL_RGBA,
            GL_UNSIGNED_BYTE, data);
        Log::Trace("Texture " + std::to_string(textureID) + ": " + dir);
    } else {
        Log::Error("Failed to load texture: " + dir);
    }
    stbi_image_free(data);
}

Texture::Texture(std::string name, unsigned int id) : textureID(id) { this->name = std::move(name); }

Texture::Texture(std::string name, const unsigned int* id) : textureID(*id) { this->name = std::move(name); }

Texture::Texture(std::string name) { this->name = std::move(name); }

Texture::Texture()
{
    name = "NA";

    SetUpTextureID();
}

Texture::Texture(const std::string& dir, void* args) { Load(dir); }

Texture::~Texture()
{
    Log::Trace("Deleting " + name);
    if (textureID != 0)
        glDeleteTextures(1, &textureID);
    textureID = 0;
}

void Texture::SetUpTextureID()
{
    // Create and bind texture ID
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Set Wrapping mode
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Set texture filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

Texture* Texture::Load(const std::string& dir)
{
    this->name = dir;

    SetUpTextureID();

    // Load Image and generate mipmaps
    stbi_set_flip_vertically_on_load(1);
    int width;
    int height;
    int nrChannels;
    unsigned char* data = stbi_load(dir.c_str(), &width, &height, &nrChannels, 0);

    if (data) {
        int texType = GL_RGBA;
        if (nrChannels == 1)
            texType = GL_DEPTH_COMPONENT;
        else if (nrChannels == 2)
            texType = GL_RG;
        else if (nrChannels == 3)
            texType = GL_RGB;

        glTexImage2D(GL_TEXTURE_2D, 0, texType, width, height, 0, texType, GL_UNSIGNED_BYTE, data);
    } else {
        Log::Error("Failed to load texture: " + dir);
    }
    stbi_image_free(data);

    return this;
}

std::shared_ptr<Texture> Texture::Create(std::string const& dir) { return std::make_shared<Texture>(dir, dir); }

std::shared_ptr<Texture> Texture::Create(const char* dir) { return std::make_shared<Texture>(dir, dir); }
