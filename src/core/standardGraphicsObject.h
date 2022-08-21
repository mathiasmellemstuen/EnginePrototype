#ifndef ENGINEPROTOTYPE_STANDARD_GRAPHICS_OBJECT
#define ENGINEPROTOTYPE_STARDARD_GRAPHICS_OBJECT

#include "object.h"
#include <string>
#include "../graphics/renderer.h"
#include "../graphics/graphicsEntity.h"
#include "../graphics/graphicsEntityInstance.h"

class StandardGraphicsObject : public Object {
	public:
        virtual void update(float& deltaTime);
        virtual void debug();
		StandardGraphicsObject(const std::string& name, Renderer& renderer, GraphicsEntity& graphicsEntity);
	protected:
		GraphicsEntityInstance<UniformBufferObject>* graphicsEntityInstance;
		Transform* transform;
		
};

#endif
