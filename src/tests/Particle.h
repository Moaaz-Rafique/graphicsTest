#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>

class Particle {
public:
	glm::vec2 position = {0,0};
	glm::vec2 velocity = glm::vec2(0.0001f, 0.01f);
	glm::vec2 axl = glm::vec2(0, 0);
	glm::vec4 color = glm::vec4(1.0f, 0.01f, 0.01f, 1.0f);
	glm::vec4 startColor = glm::vec4(1.0f, 0.01f, 0.01f, 1.0f);
	glm::vec4 endColor = glm::vec4(1.0f, 0.01f, 0.01f, 1.0f);
	float mass = .001f;
	float lifeTime = 0.00f;
	float rotation = 0.0f;
	glm::vec2 acceleration = glm::vec2(0.01f, 0.01f);


	void changeFunction(float deltaTime, float maxLife, glm::vec3 attractors[], int noOfAttractors) {
		//position += velocity * (12 - lifeTime) * .20f;//
		//\sin\left(2.5y\right)\left(2.5-y^{.5}\right)



		position.y += velocity.y / (lifeTime / maxLife); // 
		position.x += velocity.x / (lifeTime / maxLife);
		glm::vec2 axlAttraction(0, 0);
		for (int i = 0; i < noOfAttractors; i++) {
			auto attractor = attractors[i];
			if (attractor.z > 0.0f) {
				float dist = glm::distance(position, { attractor.x, attractor.y });
				axlAttraction += attractor.z * (glm::vec2(attractor.x, attractor.y) - position) * mass / (dist * dist);
				if (abs(axlAttraction.x) < 0.01)axlAttraction.x = signbit(axlAttraction.x) * 0.01;
				if (abs(axlAttraction.y) < 0.01)axlAttraction.y = signbit(axlAttraction.y) * 0.01;

				if (abs(axlAttraction.x) > 100.0f)axlAttraction.x = signbit(axlAttraction.x) * 100.0f;
				if (abs(axlAttraction.y) > 100.0f)axlAttraction.y = signbit(axlAttraction.y) * 100.0f;
			}
		}

		velocity += .05f * mass * (axl + axlAttraction) / 1000.0f;

		//if (position.x > 1.0)position.x = -1.0f;
		//if (position.y > 1.0)position.y = -1.0f;
		
		//if (position.x < -1.0)position.x = 1.0f;
		//if (position.y < -1.0)position.y = 1.0f;

		//if (position.x > 1.0)lifeTime = .0f;
		//if (position.y > 1.0)lifeTime = .0f;


		rotation += glm::linearRand(-.05f, .05f) * velocity.y;
		lifeTime -= deltaTime * 4.0f;
		color = glm::mix(endColor, startColor, lifeTime/maxLife);
	}
	void randomPos() {
		position = glm::vec2(glm::linearRand(-1.0f, 1.0f), glm::linearRand(-1.0f, 1.0f));
	}
	void randomLifeTime() {
		lifeTime = glm::linearRand(2.0f, 12.0f);
	}
	void reset() {
		lifeTime = 0.0f;
		position = glm::vec2(0.0f);

	}
};