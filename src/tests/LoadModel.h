#pragma once

#include <glm/ext/vector_float3.hpp>
#include "Test.h"
#include "../VertexArray.h"
#include "../VertexBuffer.h"
#include "../IndexBuffer.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "../Shader.h"

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
		void displayLocations();
		void ToggleButton(const char* str_id, bool* v);
	private:
		static const int noOfParametersPerVertex = 4 + 4 + 3;//4 position, 4 color, 3 normals
		static const int noOfUVertices = 60;
		static const int noOfVVertices = 60;
		static const int noOfVertices = noOfUVertices * 3 * noOfVVertices;
		static const int noOfIndeces = noOfVertices * 6;//6 * 2 * 3;//6*2*3 6 faces 2 tries per face 3 vertices per tri
		static const int sizeOfVertexBuffer = noOfVertices * noOfParametersPerVertex;
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
		glm::vec4 lightPos;
		int lightIntensity;
		glm::vec3 rotation;
		glm::vec3 scale;

		int uvDensity = 59;
		bool wireframeMode=false;
	};
}
