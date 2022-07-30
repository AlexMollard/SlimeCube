#include "Skybox.h"

#include "dirent.h"
#include "stb_image.h"

Skybox::Skybox(std::string facesDIR) { CreateSkyBox(facesDIR); }

Skybox::Skybox(const char* facesDIR) { Skybox(std::string(facesDIR)); }

Skybox::~Skybox() {}

std::vector<std::string> Skybox::GetAllFiles(std::string dirType)
{
    std::vector<std::string> fileNames;
    DIR* dir;
    struct dirent* ent;
    if ((dir = opendir(dirType.c_str())) != NULL) {
        /* print all the files and directories within directory */
        while ((ent = readdir(dir)) != NULL) {
            if (ent->d_name[0] != 46) {
                fileNames.push_back(ent->d_name);
                fileNames.push_back(std::string(dirType.c_str()) + ent->d_name);
            }
        }
        closedir(dir);
    } else {
        perror("");
    }

    return fileNames;
}

void Skybox::CreateSkyBox(std::string facesDIR)
{
    std::vector<std::string> dirs = GetAllFiles(facesDIR);
    std::vector<std::string> faces;

    for (int i = 0; i < dirs.size(); i += 2) {
        faces.push_back(dirs[i + 1]);
    }

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++) {
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        // std::cout << faces[i] << ": " << nrChannels << std::endl;
        if (data) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, nrChannels != 4 ? GL_RGB : GL_RGBA, width, height, 0,
                nrChannels != 4 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        } else {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}