#pragma once

#include "Shape.h"
#include <cmath>
#include <iostream>

class Sphere : public Shape {
private:
	static const int noOfUVertices = 10;
	static const int noOfVVertices = 10;
	static const int noOfVertices = noOfUVertices * 3 * noOfVVertices;
	static const int noOfIndeces = noOfVertices * 6;//6 * 2 * 3;//6*2*3 6 faces 2 tries per face 3 vertices per tri
	static const int sizeOfVertexBuffer = noOfVertices * noOfParametersPerVertex;
	glm::vec3 loc;
	glm::vec4 color;
	int uvDensity;
	float scale;
public:
	Sphere(int uvDensity, glm::vec4 color, glm::vec3 loc, float scale = 1.0f) {
		if (uvDensity > 60)uvDensity = 60;
		this->uvDensity = uvDensity;
		this->loc = loc;
		this->scale = scale;
		this->color = color;
	}
	virtual void addToVertexBuffer(int startPosition, int endPosition, int startIndexPosition, int endIndexPosition, float m_Vertex[], unsigned int m_Indices[], float extras[]) {
		int tk = uvDensity * uvDensity * 3;
		int tj = uvDensity;
		float radius = 2.0f / 2;
		//float m_Vertex[sizeOfVertexBuffer];
		for (int i = 0; i < tk; i += 3 * tj) {
			/*int i = 3 * noOfVVertices;
			{*/

			for (int j = 0; j < tj+1; j++)
			{
				int k = noOfParametersPerVertex * (i + j + startPosition);

				if ((k + 10) > endPosition) {
					std::cout << "Overflow Vertex\n";
					return;
				}
				float theta = 2.0 * 3.1415926 * (float)i / (tk);
				float phi= 3.1415926 * (float)j / (tj);
				m_Vertex[k] = radius * sin(theta) * sin(phi);
				m_Vertex[k + 1] = radius * cos(theta) * sin(phi);
				m_Vertex[k + 2] = radius * cos(phi);
				m_Vertex[k + 3] = 1;
				
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


				//std::cout << k << ", " << m_Vertex[k] << ", " << m_Vertex[k + 1] << ", " << m_Vertex[k + 2] << "\n";
			}
		}
		/*for (int i = 0; i < tk; i++) {
			vertexArray[startPosition+i] = m_Vertex[i];
			if ((tk + startPosition) > endPosition) {
				 throw std::invalid_argument("overflow");
			}
		}*/
		//unsigned int m_Indices[noOfIndeces];
		int l = tj * 3;
		for (int i = 0; i < tk; i += 1) {
			int k = i * 6 + startIndexPosition;
			//std::cout<<k<<"\n";
			if(k>endIndexPosition) {
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
				
				m_Indices[k] = i % tk + startPosition;
				m_Indices[k + 1] = (1 + i) % tk + startPosition;
				m_Indices[k + 2] = (l + i) % tk + startPosition;
				
				m_Indices[k + 3] = (1 + i) % tk + startPosition;
				m_Indices[k + 4] = (l + 1 + i) % tk + startPosition;
				m_Indices[k + 5] = (l + i) % tk + startPosition;
				/*std::cout <<"\n-------" << k <<"-------\n";
				for (int j = 0; j < 6; j++) {
					std::cout << m_Indices[k + j] << ", ";						
				}*/

			}
		}
		/*for (int i = 0; i < tk; i++) {
			if (i % (1 * 6) == 0) {
				std::cout << "\n";
			}
			std::cout << m_Indices[i + startIndexPosition] << ", ";
		}*/
	}

	int getTotalVertexCount() {
		return uvDensity * uvDensity * 3 * noOfParametersPerVertex;
	}
	int getTotalIndexCount() {
		return uvDensity * uvDensity * 3 * 6;
	}

};
