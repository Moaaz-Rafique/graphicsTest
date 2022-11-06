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
#include<stb_image/stb_image.h>
#include <string>


class GridImage : public Shape {
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
	unsigned char* img;
	int width, height, channels;

	void GetPixel(stbi_uc* image, size_t imageWidth, size_t x, size_t y, stbi_uc* r, stbi_uc* g, stbi_uc* b, stbi_uc* a) {
		*r = image[4 * (y * imageWidth + x) + 0];
		*g = image[4 * (y * imageWidth + x) + 1];
		*b = image[4 * (y * imageWidth + x) + 2];
		*a = image[4 * (y * imageWidth + x) + 3];
	}
	int loadImage(const char* imagePath) {
		try
		{
			this->img = stbi_load(imagePath, &this->width, &this->height, &this->channels, 4);
			if (this->img == NULL) {
				printf("Error in loading the image\n");
				throw std::exception("Image Error");
			}
		}
		catch (const std::exception& e)
		{
			this->img = stbi_load("res/textures/sword1.png", &this->width, &this->height, &this->channels, 4);
			std::cout << e.what() << std::endl;
		}
		if (this->img == NULL) {
			printf("Error in loading the image\n");
			return 0;
		}
		printf("Loaded image with a width of %dpx, a height of %dpx and %d channels\n", this->width, this->height, this->channels);
		/*int tk = width;
		for (int i = 0; i < tk; i += 1) {
			for (int j = 0; j < tk; j += 1) {
				stbi_uc r, g, b, a;
				GetPixel(img, this->width, i, j, &r, &g, &b, &a);
				if (int(a)%10 > 0)
					std::cout << "##";
				else
					std::cout << "--";
			}
			std::cout << "\n";
		}*/
	}
public:
	~GridImage() {
		stbi_image_free(this->img);
	}
	GridImage(int uvDensity, glm::vec4 color, glm::vec3 loc, float scale, const char* imagePath) {
		if (uvDensity > 60)uvDensity = 60;
		this->uvDensity = uvDensity;
		this->loc = loc;
		this->scale = scale;
		this->color = color;
		this->seed = glm::linearRand(-5.0f, 5.0f);
		this->img = NULL;
		this->loadImage(imagePath);
	}
	virtual void addToVertexBuffer(int startPosition, int endPosition, int startIndexPosition, int endIndexPosition, float m_Vertex[], unsigned int m_Indices[], float extras[]) {
		int tk = uvDensity;
		float radius = 2.0f / 40.0f;
		int maxSize = 0;
		int k = 0;

		for (int i = 0; i < tk; i += 1) {
			for (int j = 0; j < tk; j += 1) {

				k = noOfParametersPerVertex * (i * tk + j + startPosition);

				if ((k + 10) > endPosition) {
					std::cout << "maxSize:" << maxSize << ", " << k << ": Overflow Vertex\n";
					return;
				}

				stbi_uc r, g, b, a;
				GetPixel(img, width, i * (float)width/tk, j * (float)height / tk, &r, &g, &b, &a);

				m_Vertex[k] = radius * i;
				m_Vertex[k + 1] = radius * j;
				m_Vertex[k + 2] = radius * int(a)/16;
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

					m_Vertex[k + 4] = int(r) / 256.0;
					m_Vertex[k + 5] = int(g) / 256.0;
					m_Vertex[k + 6] = int(b) / 256.0;
					m_Vertex[k + 7] = int(a) / 256.0;

					m_Vertex[k + 8] = (m_Vertex[k]);
					m_Vertex[k + 9] = (m_Vertex[k + 1]);
					m_Vertex[k + 10] = (m_Vertex[k + 2]);
				}

				//std::cout << k << ", " << m_Vertex[k] << ", " << m_Vertex[k + 1] << ", " << m_Vertex[k + 2] << "\n";
				maxSize++;

			}
		}
		int i, j, zerothIndex;
		k = startIndexPosition;
		for (i = 0; i < tk - 1; i += 1) {
			for (j = 0; j < tk - 1; j += 1) {
				zerothIndex = i * tk + j;

				m_Indices[k] = zerothIndex + startPosition;
				m_Indices[k + 1] = zerothIndex + tk + startPosition;
				m_Indices[k + 2] = zerothIndex + 1 + startPosition;

				m_Indices[k + 3] = zerothIndex + 1 + startPosition;
				m_Indices[k + 4] = zerothIndex + tk + startPosition;
				m_Indices[k + 5] = zerothIndex + tk + 1 + startPosition;

				k += 6;
			}
		}
	}

	int getTotalVertexCount() {
		return uvDensity * uvDensity;
	}
	int getTotalIndexCount() {
		return uvDensity * uvDensity * 6;
	}

};
