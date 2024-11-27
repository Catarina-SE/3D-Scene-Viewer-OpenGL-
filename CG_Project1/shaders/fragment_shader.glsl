#version 330 core

in vec2 TexCoords;

out vec4 FragColor;

uniform sampler2D texture_diffuse;
uniform bool hasTexture;

void main() {
    if(hasTexture) {
        FragColor = texture(texture_diffuse, TexCoords);
    } else {
        FragColor = vec4(1.0);
    }
}