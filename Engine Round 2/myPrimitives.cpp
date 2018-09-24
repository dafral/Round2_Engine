#include "Globals.h"
#include "myPrimitives.h"
#include "Application.h"

myCube::myCube() : size(1.0f)
{}

myCube::myCube(float size) : size(size)
{}

void myCube::DrawCubeArrays()
{
	GLfloat vertices1[] = { size, size, size,  -size, size, size,  -size,-size, size,      // v0-v1-v2 (front)
							-size,-size, size,   size,-size, size,   size, size, size,      // v2-v3-v0

							size, size, size,   size,-size, size,   size,-size,-size,      // v0-v3-v4 (right)
							size,-size,-size,   size, size,-size,   size, size, size,      // v4-v5-v0

							size, size, size,   size, size,-size,  -size, size,-size,      // v0-v5-v6 (top)
							-size, size,-size,  -size, size, size,   size, size, size,      // v6-v1-v0

							-size, size, size,  -size, size,-size,  -size,-size,-size,      // v1-v6-v7 (left)
							-size,-size,-size,  -size,-size, size,  -size, size, size,      // v7-v2-v1

							-size,-size,-size,   size,-size,-size,   size,-size, size,      // v7-v4-v3 (bottom)
							size,-size, size,  -size,-size, size,  -size,-size,-size,      // v3-v2-v7

							size,-size,-size,  -size,-size,-size,  -size, size,-size,      // v4-v7-v6 (back)
							-size, size,-size,   size, size,-size,   size,-size,-size };    // v6-v5-v4

	// normal array
	GLfloat normals1[] = { 0, 0, size,   0, 0, size,   0, 0, size,      // v0-v1-v2 (front)
						   0, 0, size,   0, 0, size,   0, 0, size,      // v2-v3-v0
						   
						   size, 0, 0,   size, 0, 0,   size, 0, 0,      // v0-v3-v4 (right)
						   size, 0, 0,   size, 0, 0,   size, 0, 0,      // v4-v5-v0
						   
						   0, size, 0,   0, size, 0,   0, size, 0,      // v0-v5-v6 (top)
						   0, size, 0,   0, size, 0,   0, size, 0,      // v6-v1-v0
						   
						   -size, 0, 0,  -size, 0, 0,  -size, 0, 0,      // v1-v6-v7 (left)
						   -size, 0, 0,  -size, 0, 0,  -size, 0, 0,      // v7-v2-v1
						   
						   0,-size, 0,   0,-size, 0,   0,-size, 0,      // v7-v4-v3 (bottom)
						   0,-size, 0,   0,-size, 0,   0,-size, 0,      // v3-v2-v7
						   
						   0, 0,-size,   0, 0,-size,   0, 0,-size,      // v4-v7-v6 (back)
						   0, 0,-size,   0, 0,-size,   0, 0,-size };    // v6-v5-v4

	// color array
	GLfloat colors1[] = { size, size, size,   size, size, 0,   size, 0, 0,      // v0-v1-v2 (front)
						  size, 0, 0,   size, 0, size,   size, size, size,      // v2-v3-v0
						  
						  size, size, size,   size, 0, size,   0, 0, size,      // v0-v3-v4 (right)
						  0, 0, size,   0, size, size,   size, size, size,      // v4-v5-v0
						  
						  size, size, size,   0, size, size,   0, size, 0,      // v0-v5-v6 (top)
						  0, size, 0,   size, size, 0,   size, size, size,      // v6-v1-v0
						  
						  size, size, 0,   0, size, 0,   0, 0, 0,      // v1-v6-v7 (left)
						  0, 0, 0,   size, 0, 0,   size, size, 0,      // v7-v2-v1
						  
						  0, 0, 0,   0, 0, size,   size, 0, size,      // v7-v4-v3 (bottom)
						  size, 0, size,   size, 0, 0,   0, 0, 0,      // v3-v2-v7
						  
						  0, 0, size,   0, 0, 0,   0, size, 0,		 // v4-v7-v6 (back)
						  0, size, 0,   0, size, size,   0, 0, size };    // v6-v5-v4

	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);
	glNormalPointer(GL_FLOAT, 0, normals1);
	glColorPointer(3, GL_FLOAT, 0, colors1);
	glVertexPointer(3, GL_FLOAT, 0, vertices1);

	glPushMatrix();
	glTranslatef(2, 2, 0);   // move to upper-right corner

	glDrawArrays(GL_TRIANGLES, 0, 36);

	glPopMatrix();

	glDisableClientState(GL_VERTEX_ARRAY);  // disable vertex arrays
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
}

void myCube::DrawCubeElements()
{
	GLfloat vertices2[] = { size, size, size,  -size, size, size,  -size,-size, size,   size,-size, size,   // v0,v1,v2,v3 (front)
							size, size, size,   size,-size, size,   size,-size,-size,   size, size,-size,   // v0,v3,v4,v5 (right)
							size, size, size,   size, size,-size,  -size, size,-size,  -size, size, size,   // v0,v5,v6,v1 (top)
							-size, size, size,  -size, size,-size,  -size,-size,-size,  -size,-size, size,   // v1,v6,v7,v2 (left)
							-size,-size,-size,   size,-size,-size,   size,-size, size,  -size,-size, size,   // v7,v4,v3,v2 (bottom)
							size,-size,-size,  -size,-size,-size,  -size, size,-size,   size, size,-size }; // v4,v7,v6,v5 (back)

	// normal array
	GLfloat normals2[] = {  0, 0, size,   0, 0, size,   0, 0, size,   0, 0, size,   // v0,v1,v2,v3 (front)
							size, 0, 0,   size, 0, 0,   size, 0, 0,   size, 0, 0,   // v0,v3,v4,v5 (right)
							0, size, 0,   0, size, 0,   0, size, 0,   0, size, 0,   // v0,v5,v6,v1 (top)
							-size, 0, 0,  -size, 0, 0,  -size, 0, 0,  -size, 0, 0,   // v1,v6,v7,v2 (left)
							0,-size, 0,   0,-size, 0,   0,-size, 0,   0,-size, 0,   // v7,v4,v3,v2 (bottom)
							0, 0,-size,   0, 0,-size,   0, 0,-size,   0, 0,-size }; // v4,v7,v6,v5 (back)

	// color array
	GLfloat colors2[] = {	size, size, size,   size, size, 0,   size, 0, 0,   size, 0, size,   // v0,v1,v2,v3 (front)
							size, size, size,   size, 0, size,   0, 0, size,   0, size, size,	 // v0,v3,v4,v5 (right)
							size, size, size,   0, size, size,   0, size, 0,   size, size, 0,   // v0,v5,v6,v1 (top)
							size, size, 0,   0, size, 0,   0, 0, 0,   size, 0, 0,				// v1,v6,v7,v2 (left)
							0, 0, 0,   0, 0, size,   size, 0, size,   size, 0, 0,				// v7,v4,v3,v2 (bottom)
							0, 0, size,   0, 0, 0,   0, size, 0,   0, size, size };				// v4,v7,v6,v5 (back)

	GLubyte indices[] = {   0, 1, 2,   2, 3, 0,      // front
							4, 5, 6,   6, 7, 4,      // right
							8, 9,10,  10,11, 8,      // top
							12,13,14,  14,15,12,      // left
							16,17,18,  18,19,16,      // bottom
							20,21,22,  22,23,20 };    // back


	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);
	glNormalPointer(GL_FLOAT, 0, normals2);
	glColorPointer(3, GL_FLOAT, 0, colors2);
	glVertexPointer(3, GL_FLOAT, 0, vertices2);

	glPushMatrix();
	//glTranslatef(-2, -2, 0);                // move to bottom-left corner

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, indices);

	glPopMatrix();

	glDisableClientState(GL_VERTEX_ARRAY);  // disable vertex arrays
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
}

mySphere::mySphere(float radius, unsigned int rings, unsigned int sectors)
{
	float const R = 1. / (float)(rings - 1);
	float const S = 1. / (float)(sectors - 1);
	int r, s;

	vertices.resize(rings * sectors * 3);
	normals.resize(rings * sectors * 3);
	texcoords.resize(rings * sectors * 2);
	std::vector<GLfloat>::iterator v = vertices.begin();
	std::vector<GLfloat>::iterator n = normals.begin();
	std::vector<GLfloat>::iterator t = texcoords.begin();
	for (r = 0; r < rings; r++) for (s = 0; s < sectors; s++) {
		float const y = sin(-M_PI_2 + M_PI * r * R);
		float const x = cos(2 * M_PI * s * S) * sin(M_PI * r * R);
		float const z = sin(2 * M_PI * s * S) * sin(M_PI * r * R);

		*t++ = s * S;
		*t++ = r * R;

		*v++ = x * radius;
		*v++ = y * radius;
		*v++ = z * radius;

		*n++ = x;
		*n++ = y;
		*n++ = z;
	}

	indices.resize(rings * sectors * 4);
	std::vector<GLushort>::iterator i = indices.begin();
	for (r = 0; r < rings; r++) for (s = 0; s < sectors; s++) {
		*i++ = r * sectors + s;
		*i++ = r * sectors + (s + 1);
		*i++ = (r + 1) * sectors + (s + 1);
		*i++ = (r + 1) * sectors + s;
	}
}

void mySphere::draw(GLfloat x, GLfloat y, GLfloat z)
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glTranslatef(x, y, z);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glVertexPointer(3, GL_FLOAT, 0, &vertices[0]);
	glNormalPointer(GL_FLOAT, 0, &normals[0]);
	glTexCoordPointer(2, GL_FLOAT, 0, &texcoords[0]);
	glDrawElements(GL_QUADS, indices.size(), GL_UNSIGNED_SHORT, &indices[0]);
	glPopMatrix();
}