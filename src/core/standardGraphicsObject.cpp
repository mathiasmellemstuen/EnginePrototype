#include "standardGraphicsObject.h"
#include "../graphics/graphicsEntity.h"
#include "../graphics/uniformBufferObject.h"
#include "../graphics/graphicsEntityInstance.h"
#include "../graphics/camera.h"

StandardGraphicsObject::StandardGraphicsObject(const std::string& name, Renderer& renderer, GraphicsEntity& graphicsEntity) : Object(name) {

	GraphicsEntityInstance<UniformBufferObject>* gei = new GraphicsEntityInstance<UniformBufferObject>(renderer, getRenderPassObject(renderer, "world"), &graphicsEntity);
	Transform* t = new Transform({0.0f, 0.0f, 0.0f});
	
	addComponent(gei);
	addComponent(t);

	graphicsEntityInstance = gei;
	transform = t; 
}


void StandardGraphicsObject::debug() {
	Object::debug();
}

void StandardGraphicsObject::update(float& deltaTime) {

	graphicsEntityInstance->uniformBufferObject.model = transform->getModel();
	graphicsEntityInstance->uniformBufferObject.proj = Camera::mainCamera->projection;
	graphicsEntityInstance->uniformBufferObject.view = Camera::mainCamera->view;
	
}
