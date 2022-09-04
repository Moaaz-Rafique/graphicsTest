#pragma once

#include "Shape.h"
#include <cmath>
#include <iostream>
#include <glm/gtc/random.hpp>

class RandomShape : public Shape {
private:
	static const int noOfUVertices = 10;
	static const int noOfVVertices = 10;
	static const int noOfVertices = noOfUVertices * 3 * noOfVVertices;
	static const int noOfIndeces = noOfVertices * 6;//6 * 2 * 3;//6*2*3 6 faces 2 tries per face 3 vertices per tri
	static const int sizeOfVertexBuffer = noOfVertices * noOfParametersPerVertex;
	glm::vec3 loc;
	glm::vec4 color;
	int uvDensity;
	float seed;
	float scale;
public:
	RandomShape(int uvDensity, glm::vec4 color, glm::vec3 loc, float scale = 1.0f) {
		if (uvDensity > 60)uvDensity = 60;
		this->uvDensity = uvDensity;
		this->loc = loc;
		this->scale = scale;
		this->color = color;
		this->seed = glm::linearRand(-1.0f, 1.0f);
	}
	virtual void addToVertexBuffer(int startPosition, int endPosition, int startIndexPosition, int endIndexPosition, float m_Vertex[], unsigned int m_Indices[], float extras[]) {
		int tk = uvDensity;
		float radius = 2.0f / 2;
		int maxSize = 0;
		int k = 0;
		for (int i = 0; i < tk; i += 1) {
			
				k = noOfParametersPerVertex * (i + startPosition);

				if ((k + 10) > endPosition) {
					std::cout << "maxSize:" << maxSize << ", " << k << ": Overflow Vertex\n";
					return;
				}
				m_Vertex[k] = radius * sin(seed * seed*i+33.21);
				m_Vertex[k + 1] = radius * cos(5.0f*seed * i * 2);
				m_Vertex[k + 2] = radius * sin(239.0f*i*seed*seed*seed);
				m_Vertex[k + 3] = 1;
				
				{
					//Apply Scale

					m_Vertex[k] *= scale;
					m_Vertex[k + 1] *= scale;
					m_Vertex[k + 2] *= scale;

					//Apply location

					m_Vertex[k] += loc.x;
					m_Vertex[k + 1] += loc.y;
					m_Vertex[k + 2] += loc.z;

					m_Vertex[k + 4] = color.r;
					m_Vertex[k + 5] = color.g;
					m_Vertex[k + 6] = color.b;
					m_Vertex[k + 7] = color.a;

					m_Vertex[k + 8] = (m_Vertex[k]);
					m_Vertex[k + 9] = (m_Vertex[k + 1]);
					m_Vertex[k + 10] = (m_Vertex[k + 2]);
				}
				
				//std::cout << k << ", " << m_Vertex[k] << ", " << m_Vertex[k + 1] << ", " << m_Vertex[k + 2] << "\n";
				maxSize++;
			
		}

		
		int l = tk * 3;
		for (int i = 0; i < tk; i += 1) {
			int k = i * 6 + startIndexPosition;
			//std::cout<<k<<"\n";
			if (k > endIndexPosition) {
				std::cout << "Overflow Indices\n";
				//bool showingPopup = true;
				////while (showingPopup) {
				//ImGui::OpenPopup("Error");
				//ImGui::BeginPopupModal("Error");

				//ImGui::EndPopup();
				//}
				return;
			}
			if (i % l == l - 1)continue;
			{

				m_Indices[k] =		i % tk			+ startPosition;
				m_Indices[k + 1] = (1 + i) % tk		+ startPosition;
				m_Indices[k + 2] = (l + i) % tk		+ startPosition;

				m_Indices[k + 3] = (1 + i) % tk		+ startPosition;
				m_Indices[k + 4] = (l + 1 + i) % tk + startPosition;
				m_Indices[k + 5] = (l + i) % tk		+ startPosition;
				/*std::cout <<"\n-------" << k <<"-------\n";
				for (int j = 0; j < 6; j++) {
					std::cout << m_Indices[k + j] << ", ";
				}*/

			}
		}
	}

	int getTotalVertexCount() {
		return uvDensity;
	}
	int getTotalIndexCount() {
		return uvDensity*6;
	}

};
