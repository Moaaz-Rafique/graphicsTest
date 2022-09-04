#pragma once
#pragma once

#include "Shape.h"
#include <cmath>
#include <iostream>
#include <glm/gtc/random.hpp>
#include <glm/ext/vector_float4.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/gtc/noise.hpp>
#include "Noise.h"

class Grid : public Shape {
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
	Grid(int uvDensity, glm::vec4 color, glm::vec3 loc, float scale = 1.0f) {
		if (uvDensity > 60)uvDensity = 60;
		this->uvDensity = uvDensity;
		this->loc = loc;
		this->scale = scale;
		this->color = color;
		this->seed = glm::linearRand(-5.0f, 5.0f);
	}
	virtual void addToVertexBuffer(int startPosition, int endPosition, int startIndexPosition, int endIndexPosition, float m_Vertex[], unsigned int m_Indices[], float extras[]) {
		int tk = uvDensity;
		float radius = 2.0f / 20.0f;
		int maxSize = 0;
		int k = 0;
		for (int i = 0; i < tk; i += 1) {
			for (int j = 0; j < tk; j += 1) {

				k = noOfParametersPerVertex * (i * tk + j + startPosition);

				if ((k + 10) > endPosition) {
					std::cout << "maxSize:" << maxSize << ", " << k << ": Overflow Vertex\n";
					return;
				}
				m_Vertex[k] = radius * i;
				m_Vertex[k + 1] = radius * j;
				m_Vertex[k + 2] = radius * NoiseHelper().SmoothedNoise(radius * i * 10.0, radius * j * 10.0,(int) seed * 40);
					/** sin(seed + (float)startPosition + (float) i * j ) 
					* sin(seed + (float)startIndexPosition + (float)i * seed * j);*/
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
		}
		/*if (k > endIndexPosition) {
					std::cout << "Overflow Indices\n";
					return;
		}*/
		int i, j, zerothIndex;
		k = startIndexPosition;
		for (i = 0; i < tk - 1; i += 1) {
			for (j = 0; j < tk-1; j += 1) {				
				zerothIndex = i * tk + j;
				
				m_Indices[k] = zerothIndex + startPosition;
				m_Indices[k + 1] = zerothIndex + tk + startPosition;
				m_Indices[k + 2] = zerothIndex + 1 + startPosition;

				m_Indices[k + 3] = zerothIndex + 1 + startPosition;
				m_Indices[k + 4] = zerothIndex + tk + startPosition;
				m_Indices[k + 5] = zerothIndex + tk + 1 + startPosition;
				
				k += 6;
			}
			//std::cout << "\t"<<i << "\n";
		}		
	}

	int getTotalVertexCount() {
		return uvDensity * uvDensity;
	}
	int getTotalIndexCount() {
		return uvDensity * uvDensity * 6;
	}

};
