#include "Material.h"
#include <iostream>

Material::Material(const std::string& name)
	: name(name), ambient(0.2f), diffuse(0.8f, 0.8f, 0.8f), specular(1.0f), shininess(32.0f), textureId(0)
{
	std::cout << "Created material: " << name << std::endl;
}

void Material::setDiffuseMap(const std::string& path)
{
	diffuseMapPath = path;
	if (!path.empty())
	{
		std::cout << "Loading texture for material " << name << ": " << path << std::endl;
		diffuseTexture = std::make_unique<Texture>(path.c_str());

		if (diffuseTexture->isLoaded())
		{
			textureId = diffuseTexture->getId();
			std::cout << "Successfully loaded texture ID " << textureId
				<< " for material " << name << std::endl;
		}
		else
		{
			std::cout << "Failed to load texture for material " << name << std::endl;
			textureId = 0;
			diffuseTexture.reset();
		}
	}
}

void Material::apply(Shader& shader) const
{
	std::cout << "Applying material: " << name << std::endl;

	// Set material properties
	shader.set3Float("material.ambient", ambient.x, ambient.y, ambient.z);
	shader.set3Float("material.diffuse", diffuse.x, diffuse.y, diffuse.z);
	shader.set3Float("material.specular", specular.x, specular.y, specular.z);
	shader.setFloat("material.shininess", shininess);
	
	// Handle texture
	if (hasTexture())
	{
		diffuseTexture->bind(0);  // Bind to texture unit 0
		shader.setInt("texture_diffuse", 0);
		shader.setBool("hasTexture", true);
		std::cout << "Applied texture ID " << textureId << " for material " << name << std::endl;
	}
	else
	{
		shader.setBool("hasTexture", false);
		std::cout << "No texture applied for material " << name << std::endl;
	}
}

Material::~Material()
{
	diffuseTexture.reset();
}