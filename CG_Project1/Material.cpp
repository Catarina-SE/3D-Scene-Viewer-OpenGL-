#include "Material.h"
#include <iostream>

void Material::setDiffuseMap(const std::string& path) {
	if (!path.empty()) {
		diffuseTexture = std::make_unique<Texture>(path.c_str());
		if (diffuseTexture->isLoaded()) {
			textureId = diffuseTexture->getId();
		}
		else {
			textureId = 0;
			diffuseTexture.reset();
		}
	}
}

void Material::apply(Shader& shader) const {
	if (hasTexture()) {
		diffuseTexture->bind(0);
		shader.setInt("texture_diffuse", 0);
		shader.setBool("hasTexture", true);
	}
	else {
		shader.setBool("hasTexture", false);
	}
}