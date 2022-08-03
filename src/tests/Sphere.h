#pragma once

#include "Shape.h"
#include <cmath>
#include <iostream>

class Sphere : Shape {
private:
	static const int noOfUVertices = 30;
	static const int noOfVVertices = 30;
	static const int noOfVertices = noOfUVertices * 3 * noOfVVertices;
	static const int noOfIndeces = noOfVertices * 6;//6 * 2 * 3;//6*2*3 6 faces 2 tries per face 3 vertices per tri
	static const int sizeOfVertexBuffer = noOfVertices * noOfParametersPerVertex;
	int uvDensity;
public:
	void addToVertexBuffer(int startPosition, int endPosition, int startIndexPosition, int endIndexPosition, float vertexArray[], int indexArray[], float extras[]) {
		uvDensity = (int)extras[0];
		if (uvDensity > 30)uvDensity = 30;
		float r = extras[1];
		float g = extras[2];
		float b = extras[3];
		float a = extras[4];

		int tk = uvDensity * uvDensity * 3;
		int tj = uvDensity;
		float radius = 2.0f / 2;
		float m_Vertex[sizeOfVertexBuffer];
		for (int i = 0; i < tk; i += 3 * tj) {
			/*int i = 3 * noOfVVertices;
			{*/

			for (int j = 0; j < tj + 1; j++)
			{
				int k = noOfParametersPerVertex * (i + j);
				
				m_Vertex[k] = radius * sin(2.0 * 3.1415926 * (float)i / (tk)) * sin(3.1415926 * (float)j / (tj));
				m_Vertex[k + 1] = radius * cos(2.0 * 3.1415926 * (float)i / (tk)) * sin(3.1415926 * (float)j / (tj));
				m_Vertex[k + 2] = radius * cos(3.1415926 * (float)j / (tj));
				m_Vertex[k + 3] = 1;

				
				m_Vertex[k + 4] = r;
				m_Vertex[k + 5] = g;
				m_Vertex[k + 6] = b;
				m_Vertex[k + 7] = a;

				m_Vertex[k + 8] = (m_Vertex[k]);
				m_Vertex[k + 9] = (m_Vertex[k + 1]);
				m_Vertex[k + 10] = (m_Vertex[k + 2]);
			}
		}
		for (int i = i; i < tk; i++) {
			vertexArray[startPosition+i] = m_Vertex[i];
			if ((tk + startPosition) > endPosition) {
				 throw std::invalid_argument("overflow");
			}
		}
		int m_Indices[noOfIndeces];
		int l = tj * 3;
		for (int i = 0; i < tk; i += 1) {
			if (i % l == l - 1)continue;
			{
				m_Indices[i * 6] = i % tk;
				m_Indices[i * 6 + 1] = (1 + i) % tk;
				m_Indices[i * 6 + 2] = (l + i) % tk;

				m_Indices[i * 6 + 3] = (1 + i) % tk;
				m_Indices[i * 6 + 4] = (l + 1 + i) % tk;
				m_Indices[i * 6 + 5] = (l + i) % tk;
			}

		}
		for (int i = i; i < tk; i++) {
			indexArray[startIndexPosition + i] = m_Vertex[i];
			if ((tk + startIndexPosition) > endIndexPosition) {
				throw std::invalid_argument("overflow");
			}
		}
	}

};
