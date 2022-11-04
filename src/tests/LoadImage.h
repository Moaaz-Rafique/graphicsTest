#pragma once
#include<iostream>
#include<stb_image/stb_image.h>

#include <glm/ext/vector_float3.hpp>
#include "Test.h"
#include "../VertexArray.h"
#include "../VertexBuffer.h"
#include "../IndexBuffer.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "../Shader.h"
#include "Shape.h"
namespace test {

	class LoadImage : public Test
	{
	private:
		static const int noOfVertices = 100*100;
		static const int noOfIndeces = 6 * (noOfVertices + 6 /*for cube*/);;
		static const int sizeOfVertexBuffer = (noOfVertices + 8) * Shape::noOfParametersPerVertex;
		float m_Vertex[sizeOfVertexBuffer];
		unsigned int m_Indices[noOfIndeces];


	public:
		VertexArray  va;
		VertexBuffer* vb;
		IndexBuffer* ib;
		VertexBufferLayout layout;
		Shader* shader;

		LoadImage() {
			loadImage();
			shader = new Shader("res/shaders/Model.shader");
			shader->Bind();
			vb = new VertexBuffer(m_Vertex, sizeOfVertexBuffer * (sizeof(float)));
			layout.Push<float>(4);
			layout.Push<float>(4);
			layout.Push<float>(3);
			va.AddBuffer(*vb, layout);
			ib = new IndexBuffer(m_Indices, noOfIndeces);
		}
		~LoadImage() {

		}
		void OnUpdate(float deltaTime) {}
		void OnRender() {
			glClearColor(.150f, .150f, .150f, 1.0f);
			glClearDepth(1.0f);
			glBufferSubData(GL_ARRAY_BUFFER, 0, (sizeof(m_Vertex)), m_Vertex);
			glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, (sizeof(m_Indices)), m_Indices);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glm::mat4 proj = glm::perspective(glm::radians(45.0f), 1280.0f / 960, 0.1f, 100.0f);


			shader->SetUniformMat4f("u_MVP", proj);
			shader->SetUniform4f("u_LightPos", 0, 0, 0, 0);
			va.Bind();
			ib->Bind();

			//glLightModelfv(GL_LIGHT_MODEL_AMBIENT, {});

			//Add positioned light

				glDisable(GL_DEPTH_TEST);
				//glLineWidth(2.0f);
				glPointSize(5.0f);
				glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
				glDrawElements(GL_TRIANGLES, ib->GetCount(), GL_UNSIGNED_INT, nullptr);
		}

		void OnImGuiRender() {

		}
		void GetPixel(stbi_uc* image, size_t imageWidth, size_t x, size_t y, stbi_uc* r, stbi_uc* g, stbi_uc* b, stbi_uc* a) {
			*r = image[4 * (y * imageWidth + x) + 0];
			*g = image[4 * (y * imageWidth + x) + 1];
			*b = image[4 * (y * imageWidth + x) + 2];
			*a = image[4 * (y * imageWidth + x) + 3];
		}
		int loadImage() {
			int width, height, channels;
			unsigned char* img = stbi_load("res/textures/logo.png", &width, &height, &channels, 4);
			if (img == NULL) {
				printf("Error in loading the image\n");
				exit(1);

			}

			printf("Loaded image with a width of %dpx, a height of %dpx and %d channels\n", width, height, channels);
			for (int j = 0; j < height; j++) {
				for (int i = 0; i < width; i++)
				{
					/*unsigned bytePerPixel = channels;
					  unsigned char* pixelOffset = img + (i + width * j) * bytePerPixel;
					  unsigned char r = pixelOffset[0];
					  unsigned char g = pixelOffset[1];
					  unsigned char b = pixelOffset[2];*/
					stbi_uc r, g, b, a;
					GetPixel(img, width, i, j, &r, &g, &b, &a);
					int k = Shape::noOfParametersPerVertex * (i*width + j);

					m_Vertex[k] = int(i) / 100.0;
					m_Vertex[k + 1] = int(j) / 100.0;
					m_Vertex[k + 2] = int(a) / 26.0;
					m_Vertex[k + 3] = 1;


					m_Vertex[k + 4] = int(r) / 256.0;
					m_Vertex[k + 5] = int(g) / 256.0;
					m_Vertex[k + 6] = int(b) / 256.0;
					m_Vertex[k + 7] = 1;

					m_Vertex[k + 8] = (m_Vertex[k]);
					m_Vertex[k + 9] = (m_Vertex[k + 1]);
					m_Vertex[k + 10] = (m_Vertex[k + 2]);

				}
				std::cout << std::endl;

			};
			int i, j, zerothIndex;
			int k = 0;
			
			for (i = 0; i < width - 1; i += 1) {
				for (j = 0; j < height - 1; j += 1) {
					zerothIndex = i * width + j;

					m_Indices[k] = zerothIndex;
					m_Indices[k + 1] = zerothIndex + width;
					m_Indices[k + 2] = zerothIndex + 1;

					m_Indices[k + 3] = zerothIndex + 1;
					m_Indices[k + 4] = zerothIndex + width;
					m_Indices[k + 5] = zerothIndex + width + 1;

					k += 6;
				}
				//std::cout << "\t"<<i << "\n";
			}
			stbi_image_free(img);
			// ...
			
		}
	};
}

