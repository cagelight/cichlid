#define RENDER_SOURCE
#include "common.hh"
#include RENDER_PRIVATE

#include "mesh.hh"

using namespace cichlid;

static GLfloat const line_x_v [] = {
	0.0,	0.0,	0.0,
	1.0,	0.0,	0.0
};

static GLfloat const line_y_v [] = {
	0.0,	0.0,	0.0,
	0.0,	1.0,	0.0
};

static GLfloat const line_z_v [] = {
	0.0,	0.0,	0.0,
	0.0,	0.0,	1.0
};

static GLfloat const uqzn_v [] = { // TRIANGLE FAN
	1.0,	1.0,	0.0,
	0.0,	1.0,	0.0,
	1.0,	0.0,	0.0,
	0.0,	0.0,	0.0,
};

static GLfloat const uq_uv [] = {
	1.0,	1.0,
	0.0,	1.0,
	1.0,	0.0,
	0.0,	0.0,
};

static GLfloat const cube_v [] = { // FIXME -- total junk
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

static GLuint line_x_vao = 0, line_x_vbo = 0;
static GLuint line_y_vao = 0, line_y_vbo = 0;
static GLuint line_z_vao = 0, line_z_vbo = 0;

static GLuint prim_uq_uv_vbo = 0;
static GLuint prim_uqzn_vao = 0, prim_uqzn_vbo = 0;
static GLuint prim_cube_vao = 0, prim_cube_vbo = 0;

void render::mesh::init() {
	
	// LINE
	glCreateVertexArrays(1, &line_x_vao);
	glCreateBuffers(1, &line_x_vbo);
	glNamedBufferData( line_x_vbo, sizeof(line_x_v), line_x_v, GL_STATIC_DRAW);
	
	glVertexArrayAttribBinding(line_x_vao, 0, 0);
	glVertexArrayVertexBuffer(line_x_vao, 0, line_x_vbo, 0, 3 * sizeof(GLfloat));
	glEnableVertexArrayAttrib(line_x_vao, 0);
	glVertexArrayAttribFormat(line_x_vao, 0, 3, GL_FLOAT, GL_FALSE, 0);
	
	glCreateVertexArrays(1, &line_y_vao);
	glCreateBuffers(1, &line_y_vbo);
	glNamedBufferData( line_y_vbo, sizeof(line_y_v), line_y_v, GL_STATIC_DRAW);
	
	glVertexArrayAttribBinding(line_y_vao, 0, 0);
	glVertexArrayVertexBuffer(line_y_vao, 0, line_y_vbo, 0, 3 * sizeof(GLfloat));
	glEnableVertexArrayAttrib(line_y_vao, 0);
	glVertexArrayAttribFormat(line_y_vao, 0, 3, GL_FLOAT, GL_FALSE, 0);
	
	glCreateVertexArrays(1, &line_z_vao);
	glCreateBuffers(1, &line_z_vbo);
	glNamedBufferData( line_z_vbo, sizeof(line_z_v), line_z_v, GL_STATIC_DRAW);
	
	glVertexArrayAttribBinding(line_z_vao, 0, 0);
	glVertexArrayVertexBuffer(line_z_vao, 0, line_z_vbo, 0, 3 * sizeof(GLfloat));
	glEnableVertexArrayAttrib(line_z_vao, 0);
	glVertexArrayAttribFormat(line_z_vao, 0, 3, GL_FLOAT, GL_FALSE, 0);
	
	// UQ_UV
	glCreateBuffers(1, &prim_uq_uv_vbo);
	glNamedBufferData( prim_uq_uv_vbo, sizeof(uq_uv), uq_uv, GL_STATIC_DRAW);
	
	// UQZN
	glCreateVertexArrays(1, &prim_uqzn_vao);
	glCreateBuffers(1, &prim_uqzn_vbo);
	glNamedBufferData( prim_uqzn_vbo, sizeof(uqzn_v), uqzn_v, GL_STATIC_DRAW);
	
	glVertexArrayAttribBinding(prim_uqzn_vao, 0, 0);
	glVertexArrayVertexBuffer(prim_uqzn_vao, 0, prim_uqzn_vbo, 0, 3 * sizeof(GLfloat));
	glEnableVertexArrayAttrib(prim_uqzn_vao, 0);
	glVertexArrayAttribFormat(prim_uqzn_vao, 0, 3, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribBinding(prim_uqzn_vao, 1, 1);
	glVertexArrayVertexBuffer(prim_uqzn_vao, 1, prim_uq_uv_vbo, 0, 2 * sizeof(GLfloat));
	glEnableVertexArrayAttrib(prim_uqzn_vao, 1);
	glVertexArrayAttribFormat(prim_uqzn_vao, 1, 2, GL_FLOAT, GL_FALSE, 0);
	
	// CUBE
	glCreateVertexArrays(1, &prim_cube_vao);
	glCreateBuffers(1, &prim_cube_vbo);
	glNamedBufferData( prim_cube_vbo, sizeof(cube_v), cube_v, GL_STATIC_DRAW);
	
	glVertexArrayAttribBinding(prim_cube_vao, 0, 0);
	glVertexArrayVertexBuffer(prim_cube_vao, 0, prim_cube_vbo, 0, 3 * sizeof(GLfloat));
	glEnableVertexArrayAttrib(prim_cube_vao, 0);
	glVertexArrayAttribFormat(prim_cube_vao, 0, 3, GL_FLOAT, GL_FALSE, 0);
}

void render::mesh::term() {
	if (line_x_vao) glDeleteVertexArrays(1, &line_x_vao);
	if (line_x_vbo) glDeleteBuffers(1, &line_x_vbo);
	if (line_y_vao) glDeleteVertexArrays(1, &line_y_vao);
	if (line_y_vbo) glDeleteBuffers(1, &line_y_vbo);
	if (line_z_vao) glDeleteVertexArrays(1, &line_z_vao);
	if (line_z_vbo) glDeleteBuffers(1, &line_z_vbo);
	if (prim_uqzn_vao) glDeleteVertexArrays(1, &prim_uqzn_vao);
	if (prim_uqzn_vbo) glDeleteBuffers(1, &prim_uqzn_vbo);
	if (prim_cube_vao) glDeleteVertexArrays(1, &prim_cube_vao);
	if (prim_cube_vbo) glDeleteBuffers(1, &prim_cube_vbo);
	if (prim_uq_uv_vbo) glDeleteBuffers(1, &prim_uq_uv_vbo);
	line_x_vao = 0;
	line_x_vbo = 0;
	prim_uqzn_vao = 0;
	prim_uqzn_vbo = 0;
	prim_cube_vao = 0;
	prim_cube_vbo = 0;
}

static render::mesh::primitive bound;

void render::mesh::bind(primitive prim) {
	switch (prim) {
		case primitive::line_x:
			glBindVertexArray(line_x_vao);
			break;
		case primitive::line_y:
			glBindVertexArray(line_y_vao);
			break;
		case primitive::line_z:
			glBindVertexArray(line_z_vao);
			break;
		case primitive::uqzn:
			glBindVertexArray(prim_uqzn_vao);
			break;
		case primitive::cube:
			glBindVertexArray(prim_cube_vao);
			break;
		default:
			return;
	}
	bound = prim;
}

void render::mesh::draw() {
	switch (bound) {
		case primitive::line_x:
		case primitive::line_y:
		case primitive::line_z:
			glDrawArrays(GL_LINES, 0, 2);
			break;
		case primitive::uqzn:
			glDrawArrays(GL_TRIANGLE_STRIP, 0, sizeof(uqzn_v) / sizeof(GLfloat) / 3);
			break;
		case primitive::cube:
			glDrawArrays(GL_TRIANGLE_STRIP, 0, sizeof(cube_v) / sizeof(GLfloat) / 3);
			break;
		default:
			return;
	}

}
