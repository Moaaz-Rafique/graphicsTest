#pragma once

#include "Test.h"
#include "../Renderer.h"
#include "Particle.h"

//struct Particle
//{
//	float x;
//	float y;
//	float vx;
//	float vy;
//	float lifeTime;
//};

namespace test {
	class ParticleSystem : public Test {
	public:
		static const int PARTICLES = 3000;
		static const int ATTRACTORS = 15;
		VertexArray  va;
		VertexBuffer* vb;
		IndexBuffer* ib;
		VertexBufferLayout layout;
		

		ParticleSystem();
		~ParticleSystem();

		float dT = 0.0f;
		int frame = 0;
		int framesPerSec = 0;		

		int noOfNewParitcles = 5;
		void OnUpdate(float deltaTime);
		void OnRender();
		void OnImGuiRender(int& e);
		void AddParticle(float x, float y, int e);
		glm::vec3 attractors[ATTRACTORS];
	private:
		int n_Particles = 0;
		int n_ParticlesPointer = 0;
		int n_AttractorPointer = 0;
		glm::vec4 m_ClearColor;
		glm::vec4 m_StartColor;
		glm::vec4 m_EndColor;
		float minLife = 5.0f;
		float maxLife = 12.0f;
		float pMass = 0.001f;
		float velVar = .001f;

		float attractorPower = 1.0f;
		glm::vec2 startVel = { .00f, 0.0f };
		glm::vec2 endVel = { .00f, 0.0f };

		float m_Vertex[(PARTICLES + ATTRACTORS) * (4 + 4) * 3]; // 3 + 4 parameters / 3 vertices
		unsigned int m_Indices[(PARTICLES + ATTRACTORS) * 3];
		Particle particles[PARTICLES];
		void calNewVertexValues(float top = -.01f,	float right = .01f,	float left = .01f,float size = .80f);		
		void displayLocations();
	};
}