#version 330 core
in vec2 TexCoords;
out vec4 FragColor;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

uniform sampler2D texture_diffuse;
uniform bool hasTexture;
uniform Material material;

void main() {
    if(hasTexture) {
        vec4 texColor = texture(texture_diffuse, TexCoords);
        FragColor = texColor;
    } else {
        FragColor = vec4(material.diffuse, 1.0);
    }
}