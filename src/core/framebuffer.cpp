#include <core/framebuffer.hpp>
#include <glad/glad.h>
#include <print>

framebuffer::framebuffer(const framebuffer_extent& p_extent) {
    invalidate(p_extent);
}


void framebuffer::invalidate(const framebuffer_extent& p_extent) {
    glGenFramebuffers(1, &m_framebuffer_id);
    bind();

    
    glGenTextures(1, &m_color_attachment);
    glBindTexture(GL_TEXTURE_2D, m_color_attachment);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, (int)p_extent.width, (int)p_extent.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //! @note Handling our render buffer object
    //! @note They are objects that contain images. Used specifically with framebuffer objects
    uint32_t render_buffer_object;
    glGenRenderbuffers(1, &render_buffer_object);
    glBindRenderbuffer(GL_RENDERBUFFER, render_buffer_object);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, static_cast<int>(p_extent.width), static_cast<int>(p_extent.height));
    glBindFramebuffer(GL_FRAMEBUFFER, render_buffer_object);

    // attaching color attachment to framebuffer
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_color_attachment, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, render_buffer_object);

    //! @note Setting depth attachment
    glGenTextures(1, &m_depth_attachment);
    glBindTexture(GL_TEXTURE_2D, m_depth_attachment);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, static_cast<int>(p_extent.width), static_cast<int>(p_extent.height), 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr);

    // attaching depth attachment to framebuffer
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_depth_attachment, 0);
    // glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + 1, GL_TEXTURE_2D, m_DepthAttachment, 0);

    // Unbind();

    //! @note Creating texture for specifying framebuffer depth map (for shadow passes)
    glGenTextures(1, &m_depthmap_fbo);
    // m_FramebufferID = m_depthmap_fbo;
    glBindTexture(GL_TEXTURE_2D, m_depthmap_fbo);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, static_cast<int>(p_extent.width), static_cast<int>(p_extent.height), 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    //! @note Attaching depth/shadow mapping to this framebuffer
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthmap_fbo, 0);
    // glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + 2, GL_TEXTURE_2D, m_DepthMapFbo, 0);
    // glDrawBuffer(GL_NONE);
    // glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    


    if(glCheckFramebufferStatus(m_framebuffer_id) != GL_FRAMEBUFFER_COMPLETE){
        std::println("Framebuffer Incomplete!");
    }
    else{
        std::println("Framebuffers Completed!!!");
    }
    
    unbind();
}

void framebuffer::bind() {
    glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer_id);
}

void framebuffer::unbind() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void framebuffer::clear(const clear_op& p_data) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glClearBufferiv(GL_COLOR_ATTACHMENT0, 0, &p_data.value);
}

uint64_t framebuffer::read_pixel(const pixel_information& p_info) {
    glReadBuffer(GL_COLOR_ATTACHMENT0 + p_info.attachment_idx);
    int pixel_data;
    glReadPixels(p_info.x, p_info.y, 1, 1, GL_RED_INTEGER, GL_INT, &pixel_data);
    return pixel_data;
}