#include <ptc_texture.h>

GLuint Texture::loadTexture(std::string texturePath)
{
    stbi_set_flip_vertically_on_load(true);
    for (int i = 0; i < usedPaths.size(); i++)
        if (usedPaths[i] == texturePath)
            return textures[i];

    GLuint newTexture = 0;
    glGenTextures(1, &newTexture);
    glBindTexture(GL_TEXTURE_2D, newTexture);

    data = stbi_load((std::string(TEXTURE_PATH) + texturePath).c_str(), &width, &height, &nrChannels, 0);
    if (!data)
    {
        SDL_Log("Failed to load texture: %s. Error: %s", texturePath.c_str(), stbi_failure_reason());
    }

    GLenum format = (nrChannels == 3) ? GL_RGB : GL_RGBA;
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);

    textures.push_back(newTexture);
    usedPaths.push_back(texturePath);

    return textures[textures.size() - 1];
}

int Texture::width, Texture::height, Texture::nrChannels;
unsigned char* Texture::data;
std::vector<std::string> Texture::usedPaths;
std::vector<GLuint> Texture::textures;