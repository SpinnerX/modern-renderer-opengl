#include <core/texture.hpp>
#include <glad/glad.h>
#include <core/image.hpp>
#include <print>

texture::texture(const std::filesystem::path& p_path, bool p_gamma_enable) {
    glGenTextures(1, &m_texture_id);
    bind();
    invalidate(p_path, p_gamma_enable);
}

void texture::bind(uint32_t p_index) {
    if(p_index == 0){
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_texture_id);
        return;
    }
    
    glActiveTexture(GL_TEXTURE0 + p_index);
    glBindTexture(GL_TEXTURE_2D, m_texture_id);
}

void texture::unbind() {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void texture::invalidate(const std::filesystem::path& p_path, bool p_gamma_enable) {
    
    // load and generate the texture
    int image_width, image_height, pixel_channels;
    stbi_set_flip_vertically_on_load(true);
    stbi_uc *data = stbi_load(p_path.string().c_str(), &image_width, &image_height, &pixel_channels, 0);
    
    GLenum internal_format;
    GLenum format_to_use;

    if(pixel_channels == 1){
        internal_format = GL_RED;
        format_to_use = GL_RED;
    }
    else if(pixel_channels == 3){
        internal_format = GL_RGB;
        format_to_use = GL_RGB;
    }
    else if(pixel_channels == 4){
        internal_format = GL_RGBA;
        format_to_use = GL_RGBA;
    }

    

    if (data){
        if(!p_gamma_enable){
            glTexImage2D(GL_TEXTURE_2D, 0, (int)internal_format, image_width, image_height, 0, format_to_use, GL_UNSIGNED_BYTE, data);
        }
        else{
            internal_format = GL_SRGB;
            format_to_use = GL_RGB;
            glTexImage2D(GL_TEXTURE_2D, 0, (int)internal_format, image_width, image_height, 0, format_to_use, GL_UNSIGNED_BYTE, data);
        }
        glGenerateMipmap(GL_TEXTURE_2D);

        // set the texture wrapping/filtering options (on the currently bound texture object)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    else{
        // std::cout << "Failed to load texture" << std::endl;
        std::println("Failed to load texture!!\n");
    }
    stbi_image_free(data);

    m_texture_loaded = true;
}