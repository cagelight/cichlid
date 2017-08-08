#include "mesh.hh"

#include "bridge/glfw_common.hh"

using namespace cichlid::render;

static GLfloat const cube_v [] = {
	1.0,	1.0,	1.0,
	1.0,	1.0,	0.0,
	1.0,	0.0,	1.0,
	1.0,	0.0,	0.0,
	0.0,	1.0,	1.0,
	0.0,	0.0,	1.0,
	0.0,	1.0,	0.0,
	0.0,	0.0,	0.0,
	1.0,	1.0,	1.0,
	0.0,	1.0,	1.0,
	1.0,	1.0,	0.0,
	0.0,	1.0,	0.0,
	1.0,	0.0,	1.0,
	1.0,	0.0,	0.0,
	0.0,	0.0,	1.0,
	0.0,	0.0,	0.0,
	1.0,	1.0,	1.0,
	1.0,	0.0,	1.0,
	0.0,	1.0,	1.0,
	0.0,	0.0,	1.0,
	1.0,	1.0,	0.0,
	0.0,	1.0,	0.0,
	1.0,	0.0,	0.0,
	0.0,	0.0,	0.0,
};

static GLuint prim_cube_vao = 0, prim_cube_vbo = 0;

void mesh::init() {
	glCreateVertexArrays(1, &prim_cube_vao);
	glCreateBuffers(1, &prim_cube_vbo);
	glNamedBufferData( prim_cube_vbo, sizeof(cube_v), cube_v, GL_STATIC_DRAW);
	
	glVertexArrayAttribBinding(prim_cube_vao, 0, 0);
	glVertexArrayVertexBuffer(prim_cube_vao, 0, prim_cube_vbo, 0, sizeof(cube_v) / sizeof(GLfloat));
	glEnableVertexArrayAttrib(prim_cube_vao, 0);
	glVertexArrayAttribFormat(prim_cube_vao, 0, 3, GL_FLOAT, GL_FALSE, 0);
}

void mesh::term() {
	prim_cube_vao = 0;
	prim_cube_vbo = 0;
}

void mesh::bind(primitive prim) {
	switch (prim) {
		case primitive::cube:
			glBindVertexArray(prim_cube_vao);
			break;
	}
}

void mesh::draw() {
	glBindVertexArray(prim_cube_vao);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, sizeof(cube_v) / sizeof(GLfloat) / 3);
}
