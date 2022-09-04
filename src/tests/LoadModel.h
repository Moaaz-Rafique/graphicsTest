#pragma once

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

	class LoadModel : public Test
	{
		struct Vertex {
			glm::vec3 pos;
		};

	public:
		LoadModel();
		~LoadModel();
		void OnUpdate(float deltaTime);
		void OnRender();
		void OnImGuiRender(int& e);
		void ToggleButton(const char* str_id, bool* v);		 
		void displayLocations();
		void addSphere();
		void addRandomShape(int vertices);
		void addGrid(int vertices);
	private:
		static const int noOfShapes = 50;
		static const int maxUVDensity = 60;
		static const int noOfVertices = 60*60*3*16;
		static const int noOfIndeces = 6 * (noOfVertices + 6 /*for cube*/);//6 * 2 * 3;//6*2*3 6 faces 2 tries per face 3 vertices per tri
		static const int sizeOfVertexBuffer = (noOfVertices + 8 /*for cube*/) * Shape::noOfParametersPerVertex;
		float m_Vertex[sizeOfVertexBuffer];
		unsigned int m_Indices[noOfIndeces];
		VertexArray  va;
		VertexBuffer* vb;
		IndexBuffer* ib;
		VertexBufferLayout layout;
		Shader *shader;
		//shader.Bind();
		glm::mat4 view;
		glm::mat4 model;
		glm::mat4 mvp;
		
		glm::vec3 translation;
		glm::vec3 rotation;
		glm::vec3 scale;		

		Shape* shapes[noOfShapes + 1];
		bool wireframeMode=true;		
		bool animate = false;

		unsigned int currentShapePointer = 0;

		int uvDensity = 3;
		glm::vec3 newModelLocation;
		float newModelScale = .5;
		glm::vec4 lightPos;
		glm::vec4 newModelColor = {1,1,1,1};		
		bool overflow = false;
	};
}
