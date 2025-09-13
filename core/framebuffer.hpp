#pragma once
#include <cstdint>
#include <vector>

struct framebuffer_extent {
    uint32_t width=0;
    uint32_t height=0;
};

enum class attachment : uint8_t {
    none = 0,
    rgba8,
    rgba16,
    depth24stencil8,
    shadow_map,
    normal_map,
    depth = depth24stencil8,
};

struct pixel_information {
    int attachment_idx;
    int x;
    int y;
};

struct clear_op {
    uint32_t index=1;
    int value=-1;
};

class framebuffer {
public:
    framebuffer() = default;
    framebuffer(const framebuffer_extent& p_extent);

    void bind();

    void unbind();

    void invalidate(const framebuffer_extent& p_extent);

    void clear(const clear_op& p_data);

    uint64_t read_pixel(const pixel_information& p_info);

private:
    uint32_t m_framebuffer_id = -1;

    uint32_t m_color_attachment = -1;

    uint32_t m_depth_attachment = -1;

    uint32_t m_depthmap_fbo = -1;

    uint32_t m_samples = 1;

    std::vector<attachment> m_color_attachments;

    //! @note once this works, then we will remove m_ColorAttachment and m_DepthAttachment altogether
    attachment m_color;
    attachment m_depth;
    attachment m_depth_format_check = attachment::none;

};
