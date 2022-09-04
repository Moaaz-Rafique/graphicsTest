#pragma once

class Shape {
private:
public:
	static const int noOfParametersPerVertex = 4 + 4 + 3;//4 position, 4 color, 3 normals
	virtual void addToVertexBuffer(int startPosition, int endPosition, int startIndexPosition, int endIndexPosition, float vertexArray[], unsigned int indexArray[], float extras[]) = 0;
	virtual int getTotalVertexCount() = 0;
	virtual int getTotalIndexCount() = 0;
};
