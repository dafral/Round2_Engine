#include "Application.h"
#include "ModuleRenderer3D.h"
#include "DebugDraw.h"
#include "Color.h"

void DebugDraw::DrawBox(float3* corners, float line_width, Color color)
{
	GLfloat vertices[] = { corners[0].x, corners[0].y, corners[0].z,    corners[2].x, corners[2].y, corners[2].z,    corners[2].x, corners[2].y, corners[2].z,    corners[6].x, corners[6].y, corners[6].z,     // v0,v1,v2,v3 (front)
						   corners[4].x, corners[4].y, corners[4].z,    corners[6].x, corners[6].y, corners[6].z,    corners[4].x, corners[4].y, corners[4].z,    corners[0].x, corners[0].y, corners[0].z,    // v0,v3,v4,v5 (right)
						   corners[1].x, corners[1].y, corners[1].z,    corners[3].x, corners[3].y, corners[3].z,    corners[3].x, corners[3].y, corners[3].z,    corners[7].x, corners[7].y, corners[7].z,    // v0,v5,v6,v1 (top)
						   corners[5].x, corners[5].y, corners[5].z,    corners[7].x, corners[7].y, corners[7].z,    corners[5].x, corners[5].y, corners[5].z,    corners[1].x, corners[1].y, corners[1].z,   // v1,v6,v7,v2 (left)
						   corners[0].x, corners[0].y, corners[0].z,    corners[1].x, corners[1].y, corners[1].z,    corners[2].x, corners[2].y, corners[2].z,    corners[3].x, corners[3].y, corners[3].z,   // v7,v4,v3,v2 (bottom)
						   corners[4].x, corners[4].y, corners[4].z,    corners[5].x, corners[5].y, corners[5].z,    corners[6].x, corners[6].y, corners[6].z,    corners[7].x, corners[7].y, corners[7].z };  // v4,v7,v6,v5 (back)

	int num_vertices = 24;

	glLineWidth(line_width);
	glColor3f(color.r, color.g, color.b);

	// Activate and specify pointer to vertex array
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, vertices);
	glDrawArrays(GL_LINES, 0, num_vertices);

	// Deactivate vertex arrays after drawing
	glDisableClientState(GL_VERTEX_ARRAY);
	glColor3f(App->renderer3D->color_hue[0], App->renderer3D->color_hue[1], App->renderer3D->color_hue[2]);
}

void DebugDraw::DrawSphere(float3 pos, float radius, float line_width, Color color)
{
	uint rings = 12;
	uint sectors = 24;

	std::vector<GLfloat> vertices;
	std::vector<GLushort> indices;

	float const R = 1. / (float)(rings - 1);
	float const S = 1. / (float)(sectors - 1);

	for (int r = 0; r < rings; ++r)
	{
		for (int s = 0; s < sectors; ++s)
		{
			float const y = sin(-(PI / 2) + PI * r * R);
			float const x = cos(2 * PI * s * S) * sin(PI * r * R);
			float const z = sin(2 * PI * s * S) * sin(PI * r * R);

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

	glLineWidth(line_width);
	glColor3f(color.r, color.g, color.b);

	glEnableClientState(GL_VERTEX_ARRAY);

	glVertexPointer(3, GL_FLOAT, 0, &vertices[0]);
	glTranslated(pos.x, pos.y, pos.z);
	glDrawElements(GL_LINES, indices.size(), GL_UNSIGNED_SHORT, &indices[0]);
	glDisableClientState(GL_VERTEX_ARRAY);
	glColor3f(App->renderer3D->color_hue[0], App->renderer3D->color_hue[1], App->renderer3D->color_hue[2]);
}
