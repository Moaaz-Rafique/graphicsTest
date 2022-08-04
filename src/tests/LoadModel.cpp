#include "LoadModel.h"
#include <iostream>
#include "imgui/imgui.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/io.hpp>
#include "Sphere.h"



namespace test {
	
	LoadModel::LoadModel()
		: 
		
		//m_Indices{
		//	/*
		//	*/
		//	0,1,2,
		//	0,2,3,
		//	7,1,6,
		//	7,1,2,
		//	7,2,3,
		//	7,3,4,
		//	7,4,5,
		//	7,5,6,
		//	0,3,4,
		//	0,4,5,
		//	0,5,6,
		//	0,1,6,
		//	
		//},
		rotation { 3.3f, 0.0f, 0.30f },
		translation{ 0.0f, 0.0f, 0.0f },
		scale {1,1,1},
		lightPos { 1.0f, 3.0f, 2.0f, 1.0f }		
	{
		float cubeVertex[] = {
			0  ,0  ,0,1,	1,1,1,1.0, 1,1,1,
			1  ,0  ,0,1,	1,1,1,1.0, 1,1,1,
			1  ,1  ,0,1,	1,1,1,1.0, 1,1,1,
			0  ,1  ,0,1,	1,1,1,1.0, 1,1,1,
			0  ,1  ,1,1,	1,1,1,1.0, 1,1,1,
			0  ,0  ,1,1,	1,1,1,1.0, 1,1,1,
			1  ,0  ,1,1,	1,1,1,1.0, 1,1,1,
			1  ,1  ,1,1,	1,1,1,1.0, 1,1,1
		};		
		float cubeIndeces[] = {
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
		};
		int customIndices[noOfIndeces] = {			
			5,6,7,
			11,12,13,
			17,18,19,
			1,2,11,
			2,11,13,
			1,19,22,
			19,1,2,
			19,13,2
		};


		


		//displayLocations();

		shader = new Shader("res/shaders/Model.shader");
		shader->Bind();
		vb = new VertexBuffer(m_Vertex, sizeOfVertexBuffer * (sizeof(float)));
		layout.Push<float>(4);
		layout.Push<float>(4);
		layout.Push<float>(3);
		va.AddBuffer(*vb, layout);
		ib = new IndexBuffer(m_Indices, noOfIndeces);
	}

	LoadModel::~LoadModel()
	{

	}

	void LoadModel::OnUpdate(float deltaTime)
	{
		if (uvDensity >= 60)uvDensity = 59;
		for (int i = 0; i < noOfIndeces; i += 1) {
			m_Indices[i] = 0;
		}
		for (int i = 0; i < sizeOfVertexBuffer; i += 1) {
			m_Vertex[i] = 0;
		}

		int currentVertexStartPointer = 0;
		int currentIndexStartPointer = 0;

		for (int i = 0; i < currentShapePointer;i++) {
			float extras[] = { 1,1,1,1 };
			try {
				shapes[i]->addToVertexBuffer(currentIndexStartPointer, sizeOfVertexBuffer, currentIndexStartPointer, noOfIndeces, m_Vertex, m_Indices, extras);

			}
			catch (std::string e) {
				std::cout << e << "\n";
			}
			currentVertexStartPointer += shapes[i]->getTotalVertexCount();
			currentIndexStartPointer += shapes[i]->getTotalIndexCount();

		}

		//animate rotation
		if (animate) {
			for (int i = 0; i < 3; i++) {
				rotation[i] += .01;
			}
		}


		

		/*
		for (int i = 0; i < sizeOfVertexBuffer; i++) {
			m_Vertex[i] = 0;
		}

		float radius = 1.0f;
		int i = lightIntensity * noOfVVertices;
		{
			for (int j = 0; j < 3 * noOfVVertices; j++)
			{
				int k = noOfParametersPerVertex * (i + j);
				m_Vertex[k] = radius * sin(2.0 * 3.1415926 * (float)i / (noOfVertices)) * sin(3.1415926 * (float)j / noOfVVertices);
				m_Vertex[k + 1] = radius * cos(2.0 * 3.1415926 * (float)i / (noOfVertices)) * sin(3.1415926 * (float)j / noOfVVertices);
				m_Vertex[k + 2] = radius * cos(3.1415926 * (float)j / noOfVVertices);

				m_Vertex[k + 3] = 1;

				if (abs(m_Vertex[k]) < .000001)m_Vertex[k] = 0;
				if (abs(m_Vertex[k + 1]) < .000001)m_Vertex[k + 1] = 0;
				if (abs(m_Vertex[k + 2]) < .000001)m_Vertex[k + 2] = 0;

				m_Vertex[k + 4] = abs(m_Vertex[k]);
				m_Vertex[k + 5] = abs(m_Vertex[k + 1]);
				m_Vertex[k + 6] = abs(m_Vertex[k + 2]);
				m_Vertex[k + 7] = 1;

				m_Vertex[k + 8] = (m_Vertex[k]);
				m_Vertex[k + 9] = (m_Vertex[k + 1]);
				m_Vertex[k + 10] = (m_Vertex[k + 2]);
			}
		}*/
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
		glClearColor(.150f, .150f, .150f, 1.0f);
		glClearDepth(1.0f);
		glBufferSubData(GL_ARRAY_BUFFER, 0, (sizeof(m_Vertex)), m_Vertex);
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, (sizeof(m_Indices)), m_Indices);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		shader->SetUniformMat4f("u_MVP", mvp);
		shader->SetUniform4f("u_LightPos", lightPos[0], lightPos[1], lightPos[2], lightPos[3]);
		va.Bind();
		ib->Bind();
		
		//glLightModelfv(GL_LIGHT_MODEL_AMBIENT, {});

		//Add positioned light

		if (wireframeMode) {
			glDisable(GL_DEPTH_TEST);
			glLineWidth(2.0f);
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glDrawElements(GL_TRIANGLES, ib->GetCount(), GL_UNSIGNED_INT, nullptr);
		}
		else {
			glEnable(GL_DEPTH_TEST);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glDrawElements(GL_TRIANGLES, ib->GetCount(), GL_UNSIGNED_INT, nullptr);
		}
	}

	void LoadModel::OnImGuiRender(int& e)
	{
		ImGui::Begin("Model Controls");

		ImGui::DragFloat3("Translate", glm::value_ptr(translation), .01f);
		ImGui::DragFloat3("Rotation", glm::value_ptr(rotation), .10f);
		ImGui::DragFloat3("Scale", glm::value_ptr(scale), .10f);
		ImGui::DragFloat4("Light Position", glm::value_ptr(lightPos), .150f);		
		if (ImGui::Button("Display locs"))displayLocations();
		ToggleButton("Switch to Wireframe mode", &wireframeMode);
		ImGui::Text("Wireframe Toggle");
		ToggleButton("Animate", &animate);
		ImGui::Text("Animation Toggle");
		ImGui::Separator();
		
		
		ImGui::DragInt("Model Density", &uvDensity, 1, 3, 20);
		ImGui::DragFloat3("New Model Location", glm::value_ptr(newModelLocation), .01f);
		ImGui::DragFloat("New Model Scale", &newModelScale, .01f);
		if (ImGui::Button("Add New Sphere Model"))addSphere();
		ImGui::Value("Total Shapes", currentShapePointer);		
		ImGui::End();

		



	}
	
	void LoadModel::displayLocations()
	{
		std::cout << "\n[";

		for (int i = 0*11; i < 12; i += 11) {

			if (i % (11*3) == 0) {
				std::cout << "\n";
			}
			std::cout << "{'x':" << m_Vertex[i] << ", ";
			std::cout << "'y':" << m_Vertex[i + 1] << ", ";
			std::cout << "'z':" << m_Vertex[i + 2] << "}, \n";
		}
		std::cout << "]";

		std::cout << "\n[";

		for (int i = 0 * 11; i < 12; i +=1) {

			if (i % (1 * 3) == 0) {
				std::cout << "\n";
			}
			std::cout <<  m_Indices[i] << ", ";
		}
		std::cout << "]";

	}

	void LoadModel::addSphere()
	{
		if (currentShapePointer > 9)std::cout << "Max Shapes Exceeded";
		std::cout << "Adding Sphere...\n";
		shapes[currentShapePointer++] = new Sphere(uvDensity, newModelLocation, newModelScale);
	}	

	void LoadModel::ToggleButton(const char* str_id, bool* v)
	{
		ImVec2 p = ImGui::GetCursorScreenPos();
		ImDrawList* draw_list = ImGui::GetWindowDrawList();

		float height = ImGui::GetFrameHeight();
		float width = height * 1.55f;
		float radius = height * 0.50f;

		if (ImGui::InvisibleButton(str_id, ImVec2(width, height)))
			*v = !*v;
		ImU32 col_bg;
		if (ImGui::IsItemHovered())
			col_bg = *v ? IM_COL32(145 + 20, 211, 68 + 20, 255) : IM_COL32(218 - 20, 218 - 20, 218 - 20, 255);
		else
			col_bg = *v ? IM_COL32(145, 211, 68, 255) : IM_COL32(218, 218, 218, 255);

		draw_list->AddRectFilled(p, ImVec2(p.x + width, p.y + height), col_bg, height * 0.5f);
		draw_list->AddCircleFilled(ImVec2(*v ? (p.x + width - radius) : (p.x + radius), p.y + radius), radius - 1.5f, IM_COL32(255, 255, 255, 255));
	}
}
