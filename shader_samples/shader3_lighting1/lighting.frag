#version 330 core

out vec4 frag_color;

in vec3 outColor;
in vec2 outTexCoords;

uniform sampler2D texture_data;

void main() {
    frag_color = texture(texture_data, outTexCoords);

}