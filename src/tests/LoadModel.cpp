#include "LoadModel.h"
#include <iostream>
#include "imgui/imgui.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/io.hpp>



namespace test {
	
	LoadModel::LoadModel()
		: m_Vertex{
			0  ,0  ,0,1,	1,1,1,1.0, 1,1,1,
			1  ,0  ,0,1,	1,1,1,1.0, 1,0,-1,
			1  ,1  ,0,1,	1,1,1,1.0, 0,-1,0,
			0  ,1  ,0,1,	1,1,1,1.0, 0,1,-1,
			0  ,1  ,1,1,	1,1,1,1.0, 1,0,0,
			0  ,0  ,1,1,	1,1,1,1.0, -1,0,0,
			1  ,0  ,1,1,	1,1,1,1.0, -1,-1,1,
			1  ,1  ,1,1,	1,1,1,1.0, 1,0,-1
		},
		m_Indices{
			/*
			*/
			0,1,2,
			0,2,3,
			7,1,6,
			7,1,2,
			7,2,3,
			7,3,4,
			7,4,5,
			7,5,6,
			0,3,4,
			0,4,5,
			0,5,6,
			0,1,6,
			
		},
		rotation { 0.0f, 0.0f, 0.0f },
		translation { 0.0f, 0.0f, 0.0f },
		scale {1,1,1}
	{

		//for (int i = 0; i < sizeOfVertexBuffer; i++) {
		//	if (i % 8 < 3) {
		//		std::cout << m_Vertex[i]<<", ";
		//		//m_Vertex[i] +=.5;
		//		m_Vertex[i] *= 1;
		//	}
		//	else if(i % 8 == 3)
		//		std::cout << "\n ";
		//		//continue;
		//	else if(i%8<7)
		//		m_Vertex[i] *= .1;
		//}
		shader = new Shader("res/shaders/Model.shader");
		shader->Bind();
		vb = new VertexBuffer(m_Vertex, sizeOfVertexBuffer * (sizeof(float)));
		layout.Push<float>(4);
		layout.Push<float>(4);
		layout.Push<float>(3);
		va.AddBuffer(*vb, layout);
		ib = new IndexBuffer(m_Indices, 6 * 2 * 3);
	}

	LoadModel::~LoadModel()
	{

	}

	void LoadModel::OnUpdate(float deltaTime)
	{
		//animate rotation
		for (int i = 0; i < 3; i++) {
			rotation[i] += .01;
		}

		glm::mat4 proj = glm::perspective(glm::radians(45.0f), 1280.0f/ 960, 0.1f, 100.0f);
		glm::mat4 viewTranslate = glm::translate(
			glm::lookAt(
				glm::vec3(4, 0, 1), // Camera is at (4,3,3), in World Space
				glm::vec3(0, 0, 0), // and looks at the origin
				glm::vec3(-1, 0, 0)  // Head is up (set to 0,-1,0 to look upside-down)
			),
			translation);
		glm::mat4 viewRotateY = glm::rotate(
			viewTranslate,
			rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
		glm::mat4 viewRotateX = glm::rotate(
			viewRotateY,
			rotation.y, glm::vec3(-1.0f, 0.0f, 0.0f));
		glm::mat4 view = glm::rotate(
			viewRotateX,
			rotation.x, glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 model = glm::scale(
			glm::mat4(1.0f),
			scale);
		mvp = proj * view * model;
	}

	void LoadModel::OnRender()
	{
		glClearColor(.50f, .50f, .50f, 1.0f);
		glClearDepth(1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
		GLfloat mat_shininess[] = { 50.0 };
		GLfloat light_position[] = { 10.0, 10.0, 1.0, 0.0 };
		
		glShadeModel(GL_SMOOTH);

		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
		glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
		glLightfv(GL_LIGHT0, GL_POSITION, light_position);



		shader->SetUniformMat4f("u_MVP", mvp);
		shader->SetUniform4f("u_LightPos", lightPos[0], lightPos[1], lightPos[2], lightPos[3]);
		va.Bind();
		ib->Bind();
		
		//glLightModelfv(GL_LIGHT_MODEL_AMBIENT, {});

		//Add positioned light
		
		
		glBufferSubData(GL_ARRAY_BUFFER, 0, (sizeof(m_Vertex)), m_Vertex);




		glDrawElements(GL_TRIANGLES, ib->GetCount(), GL_UNSIGNED_INT, nullptr);
		
	}

	void LoadModel::OnImGuiRender(int& e)
	{
		ImGui::Begin("Model Matrix");
		ImGui::DragFloat3("Translate", glm::value_ptr(translation), .01f);
		ImGui::DragFloat3("Rotation", glm::value_ptr(rotation), .10f);
		ImGui::DragFloat3("Scale", glm::value_ptr(scale), .10f);
		ImGui::DragFloat4("Light Position", glm::value_ptr(lightPos), .150f);
		ImGui::DragFloat("Light Intensity", &lightIntensity, .150f);
		ImGui::End();
	}
	glm::vec3 computeNormal(
		glm::vec3 const& a,
		glm::vec3 const& b,
		glm::vec3 const& c)
	{
		return glm::normalize(glm::cross(c - a, b - a));
	}
}
