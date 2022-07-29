#include "ParticleSystem.h"

#include <GL/glew.h>
#include "imgui/imgui.h"
#include "../VertexBuffer.h"
#include "../VertexArray.h"
#include "../IndexBuffer.h"
#include <iostream>
#include <glm/gtc/random.hpp>
#include <glm/gtc/type_ptr.hpp>
#

namespace test {

	ParticleSystem::ParticleSystem() 
		: m_ClearColor {.2f, .3f, .8f, 1.0f},
		  m_StartColor { .0f, .0f, .0f, 1.0f }
		/*,
		m_Loc { .0f, .0f },
		m_Vertex{
				1.0f, 1.0f, 1.0f, 1.0f,
				-1.0f, -1.0f, 1.0f, 1.0f,
				.50f, -.50f, 1.0f, 1.0f,
				-.50f, -.50f, 1.0f, 1.0f,
				-.450f, .350f, 1.0f, 1.0f,
				-.150f, -.250f, 1.0f, 1.0f,
				.350f, .250f, 1.0f, 1.0f,
				-.350f, -.250f, 1.0f, 1.0f,
				.350f, .350f, 1.0f, 1.0f
		}*/
	{
		/*for (int i = 0; i < PARTICLES; i++) {
			locations[i][0] = glm::linearRand(-1.0f, 1.0f);
			locations[i][1] = glm::linearRand(-1.0f, 1.0f);
			m_Lifetime[i] = 5.0f;
		}
		calNewVertexValues();*/
		
		for (int i = 0; i < (PARTICLES + ATTRACTORS) * 3; i++) {
			m_Indices[i] = i;
		}	
		for (int i = 0; i < ATTRACTORS; i++) {
			attractors[i] = { 0.5, 0.5, 0 };
		}
		vb = new VertexBuffer(nullptr, (PARTICLES + ATTRACTORS) * (4 + 4) * 3 * (sizeof(float)));
		layout.Push<float>(4);
		layout.Push<float>(4);
		va.AddBuffer(*vb, layout);
		ib = new IndexBuffer(m_Indices, (PARTICLES + ATTRACTORS) * 3);
		
	}
	ParticleSystem::~ParticleSystem() {
	}

	void ParticleSystem::OnUpdate(float deltaTime) {
		
		float offX = .01f, offY = .01f;
		int t_Particles = 0;
		for (int i = 0; i < PARTICLES; i++ ) {	
			if (particles[i].lifeTime > 0.0f) {
				particles[i].changeFunction(deltaTime, maxLife, attractors, ATTRACTORS);
				t_Particles++;
			}
			else {
				particles[i].reset();
			}
		}
		n_Particles = t_Particles;
		calNewVertexValues();		
		dT += deltaTime;
		frame++;

		if (dT >= 1.0f) {
			framesPerSec = frame;
			frame = 0;
			dT = 0.0f;
		}
	}
	void ParticleSystem::OnRender() {
		glClearColor(m_ClearColor[0], m_ClearColor[1], m_ClearColor[2], m_ClearColor[3]);
		glClear(GL_COLOR_BUFFER_BIT);
		va.Bind();
		ib->Bind();
		glBufferSubData(GL_ARRAY_BUFFER, 0, (sizeof(m_Vertex)), m_Vertex);
		
		glDrawElements(GL_TRIANGLES, ib->GetCount(), GL_UNSIGNED_INT, nullptr);
	}
	void ParticleSystem::OnImGuiRender(int& e) {


		ImGui::Text("FPS: %d", framesPerSec);
		ImGui::ColorEdit4("Clear Color", glm::value_ptr(m_ClearColor));
		

		if (ImGui::Button("Show Vertex Locations")) {
			displayLocations();
		}
		ImGui::ProgressBar((float)n_Particles/PARTICLES);


		if(ImGui::Button("Random Locs for particles")){
			for (int i = 0; i < PARTICLES; i++) {
				particles[i].randomPos();
				particles[i].randomLifeTime();
			}			
		}
		
		
		
		ImGui::RadioButton("Add Particle", &e, 0);
		ImGui::RadioButton("Add Attractor", &e, 1);


		if (e == 0) {
			ImGui::ColorEdit4("Object Start Color", glm::value_ptr(m_StartColor));
			ImGui::ColorEdit4("Object End Color", glm::value_ptr(m_EndColor));
			ImGui::DragInt("New Particles", &noOfNewParitcles);
			ImGui::DragFloat("Particle Mass", &pMass, .001f);
			ImGui::DragFloatRange2("Lifetime range", &minLife, &maxLife);
			if (ImGui::Button("Fire")) {
				m_ClearColor = { 0.0f, 0.0f, 0.0f, 1.0f };
				m_StartColor = { 1.0f, 0.0f, 0.0f, .20f };
				m_EndColor = { 1.0f, .80f, .03f, .001f };
				minLife = 5.0f;
				maxLife = 12.0f;
				noOfNewParitcles = 10;
				startVel = glm::vec2( - .0001f, .001f);
				endVel = glm::vec2(.0001f, .01f);
				pMass = .001f;
			}
			if (ImGui::Button("Smoke")) {
				m_ClearColor = { 0.0f, 0.0f, 0.0f, 1.0f };
				m_StartColor = { .30f, 0.3f, 0.33f, .10f };
				m_EndColor = { .01f, .01f, .01f, .001f };
				minLife = 10.0f;
				maxLife = 12.0f;
				noOfNewParitcles = 30;
				startVel = glm::vec2(-.0001f, .001f);
				endVel = glm::vec2(.0001f, .01f);
				pMass = .0001f;
			}
			if (ImGui::Button("Water")) {
				m_ClearColor = { 0.0f, 0.0f, 0.0f, 1.0f };
				m_StartColor = { 0.9f, 0.9f, 0.9f, 0.9f };
				m_EndColor = { 0.114f, 0.114f, 0.8f, 1.0f };
				minLife = 10.0f;
				maxLife = 12.0f;
				noOfNewParitcles = 100;
				startVel = glm::vec2(.01f, .00f);
				endVel = glm::vec2(.02f, .0001f);
				pMass = .65f;
			}
			if (ImGui::Button("fireworks")) {
			m_ClearColor = { 0.0f, 0.0f, 0.0f, 1.0f };
			m_StartColor = { 1.0f, 0.0243f, 0.0329f, 0.1f };
			m_EndColor = { 0.0f, 0.0243f, 1.0f, 0.1f };
			minLife = 4.0f;
			maxLife = 5.0f;
			noOfNewParitcles = 70;
			startVel = glm::vec2(.01f, .01f);
			endVel = glm::vec2(-.01f, -.01f);
			pMass = .005f;
		}
		}
		else if (e == 1) {
			ImGui::DragFloat("Attractor Power", &attractorPower, .001f);
		}
	}
	void ParticleSystem::AddParticle(float x, float y, int e)
	{
		if (e == 0) {
			for (int i = 0; i < noOfNewParitcles; i++) {
				particles[n_ParticlesPointer].lifeTime = maxLife;//glm::linearRand(minLife, maxLife);
				particles[n_ParticlesPointer].position = { x + glm::linearRand(-.01f, .01f), y + glm::linearRand(-.05f, .05f) };
				particles[n_ParticlesPointer].velocity = glm::linearRand(startVel, endVel);
				particles[n_ParticlesPointer].rotation = 0;
				particles[n_ParticlesPointer].startColor = m_StartColor;
				particles[n_ParticlesPointer].endColor = m_EndColor;
				particles[n_ParticlesPointer].mass = pMass;
				n_ParticlesPointer = (++n_ParticlesPointer) % PARTICLES;
			}
		}
		if (e == 1) {
			attractors[n_AttractorPointer % ATTRACTORS] = { x, y, attractorPower };
			n_AttractorPointer++;
		}
		//n_Particles++;
	}
	void ParticleSystem::calNewVertexValues(float top, float right, float left, float size)
	{		
		int totalPara = 3 * (4 + 4);
		int paraPerVertex = 4 + 4;
		for (int i = PARTICLES; i < (PARTICLES + ATTRACTORS); i++) {
			int k = i - PARTICLES;
			for (int j = 0; j < 3; j++) {
				glm::vec2 p = { attractors[k].x, attractors[k].y };

				m_Vertex[i * totalPara + 0 + j * paraPerVertex] = p.x + size * attractors[k].z / 10 * cos(j * 2.0f * 3.14f / 3.0f + 3.14f / 2.0f);
				m_Vertex[i * totalPara + 1 + j * paraPerVertex] = p.y + size * attractors[k].z / 10 * sin(j * 2.0f * 3.14f / 3.0f + 3.14f / 2.0f);
				m_Vertex[i * totalPara + 2 + j * paraPerVertex] = 1.0f;
				m_Vertex[i * totalPara + 3 + j * paraPerVertex] = 1.0f;

				m_Vertex[i * totalPara + 4 + j * paraPerVertex] = 1.0f / j;
				m_Vertex[i * totalPara + 5 + j * paraPerVertex] = 1.0f / j;
				m_Vertex[i * totalPara + 6 + j * paraPerVertex] = 1.0f;
				m_Vertex[i * totalPara + 7 + j * paraPerVertex] = 1.0f / ATTRACTORS;
			}
		}
		for (int i = 0; i < PARTICLES; i++) {
			Particle p = particles[i];
			if (p.lifeTime > 0.0f) {
				//float off = p.lifeTime/100;//
				for (int j = 0; j < 3; j++) {
					m_Vertex[i * totalPara + 0 + j * paraPerVertex] = p.position.x + (p.lifeTime / maxLife) * size / 10 * cos(j * 2.0f * 3.14f / 3.0f + p.rotation + 3.14f / 2.0f);
					m_Vertex[i * totalPara + 1 + j * paraPerVertex] = p.position.y + (p.lifeTime / maxLife) * size / 10 * sin(j * 2.0f * 3.14f / 3.0f + p.rotation + 3.14f / 2.0f);
					m_Vertex[i * totalPara + 2 + j * paraPerVertex] = 1.0f;
					m_Vertex[i * totalPara + 3 + j * paraPerVertex] = 1.0f;

					m_Vertex[i * totalPara + 4 + j * paraPerVertex] = p.color[0];
					m_Vertex[i * totalPara + 5 + j * paraPerVertex] = p.color[1];
					m_Vertex[i * totalPara + 6 + j * paraPerVertex] = p.color[2];
					m_Vertex[i * totalPara + 7 + j * paraPerVertex] = p.color[3];
				}
				/*m_Vertex[i * totalPara + 0] = p.position.x;
				m_Vertex[i * totalPara + 1] = p.position.y - top * size * p.lifeTime;
				m_Vertex[i * totalPara + 2] = 1.0f;
				m_Vertex[i * totalPara + 3] = 1.0f;

				m_Vertex[i * totalPara + 4] = p.color[0];
				m_Vertex[i * totalPara + 5] = p.color[1];
				m_Vertex[i * totalPara + 6] = p.color[2];
				m_Vertex[i * totalPara + 7] = p.color[3];

				m_Vertex[i * totalPara + 0 + paraPerVertex] = p.position.x - left * size * p.lifeTime;
				m_Vertex[i * totalPara + 1 + paraPerVertex] = p.position.y + top * size * p.lifeTime;
				m_Vertex[i * totalPara + 2 + paraPerVertex] = 1.0f;
				m_Vertex[i * totalPara + 3 + paraPerVertex] = 1.0f;

				m_Vertex[i * totalPara + 4 + paraPerVertex] = p.color[0];
				m_Vertex[i * totalPara + 5 + paraPerVertex] = p.color[1];
				m_Vertex[i * totalPara + 6 + paraPerVertex] = p.color[2];
				m_Vertex[i * totalPara + 7 + paraPerVertex] = p.color[3];

				m_Vertex[i * totalPara + 0 + paraPerVertex + paraPerVertex] = p.position.x + right * size * p.lifeTime;
				m_Vertex[i * totalPara + 1 + paraPerVertex + paraPerVertex] = p.position.y + top * size * p.lifeTime;
				m_Vertex[i * totalPara + 2 + paraPerVertex + paraPerVertex] = 1.0f;
				m_Vertex[i * totalPara + 3 + paraPerVertex + paraPerVertex] = 1.0f;
				m_Vertex[i * totalPara + 4 + paraPerVertex + paraPerVertex] = p.color[0];
				m_Vertex[i * totalPara + 5 + paraPerVertex + paraPerVertex] = p.color[1];
				m_Vertex[i * totalPara + 6 + paraPerVertex + paraPerVertex] = p.color[2];
				m_Vertex[i * totalPara + 7 + paraPerVertex + paraPerVertex] = p.color[3];*/

			}
			else {
				m_Vertex[i * totalPara + 0] = 0.0f;
				m_Vertex[i * totalPara + 1] = 0.0f;				

				m_Vertex[i * totalPara + 0 + paraPerVertex] = 0.0f;
				m_Vertex[i * totalPara + 1 + paraPerVertex] = 0.0f;

				m_Vertex[i * totalPara + 0 + paraPerVertex + paraPerVertex] = 0.0f;
				m_Vertex[i * totalPara + 1 + paraPerVertex + paraPerVertex] = 0.0f;

			}
		}
		
	}

	void ParticleSystem::displayLocations()
	{
		for (Particle p : particles) {
			std::cout << p.position.x << ", " << p.position.y << ", " << p.lifeTime <<std::endl;
		}
		for (glm::vec3 p : attractors) {
			std::cout << p.x << ", " << p.y << ", " << p.z << std::endl;
		}
		std::cout << ib->GetCount() * 3 << std::endl;
	}
}
