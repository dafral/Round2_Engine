#include "Globals.h"
#include "myPrimitives.h"
#include "Application.h"

// =============================================================================================================================
// CUBE 
// =============================================================================================================================

myCube::myCube() : size(1.0f)
{}

myCube::myCube(float size) : size(size)
{}

void myCube::Draw() const
{
	GLfloat vertices[] = { size, size, size,  -size, size, size,  -size,-size, size,   size,-size, size,     // v0,v1,v2,v3 (front)
							size, size, size,   size,-size, size,   size,-size,-size,   size, size,-size,    // v0,v3,v4,v5 (right)
							size, size, size,   size, size,-size,  -size, size,-size,  -size, size, size,    // v0,v5,v6,v1 (top)
							-size, size, size,  -size, size,-size,  -size,-size,-size,  -size,-size, size,   // v1,v6,v7,v2 (left)
							-size,-size,-size,   size,-size,-size,   size,-size, size,  -size,-size, size,   // v7,v4,v3,v2 (bottom)
							size,-size,-size,  -size,-size,-size,  -size, size,-size,   size, size,-size };  // v4,v7,v6,v5 (back)

	GLubyte indices[] = {   0, 1, 2,   2, 3, 0,       // front
							4, 5, 6,   6, 7, 4,       // right
							8, 9,10,  10,11, 8,       // top
							12,13,14,  14,15,12,      // left
							16,17,18,  18,19,16,      // bottom
							20,21,22,  22,23,20 };    // back

	// Activate and specify pointer to vertex array
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, vertices);

	// Draw cube
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, indices);

	// Deactivate vertex arrays after drawing
	glDisableClientState(GL_VERTEX_ARRAY); 

}

// =============================================================================================================================
// SPHERE 
// =============================================================================================================================

mySphere::mySphere(float radius, unsigned int rings, unsigned int sectors)
{
	float const R = 1. / (float)(rings - 1);
	float const S = 1. / (float)(sectors - 1);

	for (int r = 0; r < rings; ++r) {
		for (int s = 0; s < sectors; ++s) {
			float const y = sin(-M_PI_2 + M_PI * r * R);
			float const x = cos(2 * M_PI * s * S) * sin(M_PI * r * R);
			float const z = sin(2 * M_PI * s * S) * sin(M_PI * r * R);

			vertices.push_back(x * radius);
			vertices.push_back(y * radius);
			vertices.push_back(z * radius);

			if (r < rings - 1)
			{
				int curRow = r * sectors;
				int nextRow = (r + 1) * sectors;
				int nextS = (s + 1) % sectors;

				indices.push_back(curRow + s);
				indices.push_back(nextRow + s);
				indices.push_back(nextRow + nextS);

				indices.push_back(curRow + s);
				indices.push_back(nextRow + nextS);
				indices.push_back(curRow + nextS);
			}
		}
	}

}

void mySphere::Draw() const
{
	glEnableClientState(GL_VERTEX_ARRAY);

	glVertexPointer(3, GL_FLOAT, 0, &vertices[0]);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_SHORT, &indices[0]);

	glDisableClientState(GL_VERTEX_ARRAY);
}

// =============================================================================================================================
// PLANE
// =============================================================================================================================

myPlane::myPlane(float size) : size(size)
{
	// Vertices
	for (int row = 0; row < size; row++) 
	{
		for (int col = 0; col < size; col++) 
		{
			vertices.push_back(col);
			vertices.push_back(0.0f);
			vertices.push_back(row);
		}
	}

	// Indices
	for (int row = 0; row < size - 1; row++) 
	{
		if ((row & 1) == 0) 
		{ // even rows
			for (int col = 0; col < size; col++) 
			{
				indices.push_back(col + row * size);
				indices.push_back(col + (row + 1) * size); 
			}
		}
		else 
		{ // odd rows
			for (int col = size - 1; col > 0; col--) 
			{
				indices.push_back(col + (row + 1) * size);
				indices.push_back(col - 1 + + row * size);
			}
		}
	}	
}

void myPlane::Draw() const
{
	glEnableClientState(GL_VERTEX_ARRAY);

	glPushMatrix();
	glTranslatef(0 - size/2 + 0.5, 0, 0 - size/2 + 0.5); 

	glPolygonMode(GL_FRONT, GL_LINE);
	glPolygonMode(GL_BACK, GL_LINE);

	glColor3f(0.0, 100.0, 255.0);
	glVertexPointer(3, GL_FLOAT, 0, &vertices[0]);
	glDrawElements(GL_TRIANGLE_STRIP, indices.size(), GL_UNSIGNED_SHORT, &indices[0]);
	glColor3f(255.0, 255.0, 255.0);

	if (App->editor->configuration->wireframe)
	{
		glPolygonMode(GL_FRONT, GL_LINE);
		glPolygonMode(GL_BACK, GL_LINE);
		glPopMatrix();
	}
	else
	{
		glPolygonMode(GL_FRONT, GL_FILL);
		glPolygonMode(GL_BACK, GL_FILL);
		glPopMatrix();
	}

	glDisableClientState(GL_VERTEX_ARRAY);
}