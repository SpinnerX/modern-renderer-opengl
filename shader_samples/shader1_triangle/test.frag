#version 330 core

out vec4 frag_color;

in vec3 outColor;
in vec2 outTexCoords;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main() {
    // frag_color = vec4(1.0f, 0.4f, 0.2f, 1.0f);
    // frag_color = outColor;
    // frag_color = texture(texture1, outTexCoords);
    frag_color = mix(texture(texture1, outTexCoords), texture(texture2, outTexCoords), 0.2);

}