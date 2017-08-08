#include "render.hh"

#include "bridge/glfw_common.hh"

#include "protoframe.hh"
#include "mesh.hh"
#include "shader.hh"
#include "commandseq.hh"

using namespace cichlid;

static render::protoframe * root_frame = nullptr;

void render::init() {
	glfwMakeContextCurrent(glfw::win);
	if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
		cilogve("GLAD failed to initialize");
		throwe(startup);
	}
	glfwSwapInterval(1);
	glClearColor(0, 0, 0, 1);
	shader::init();
	mesh::init();
}

void render::term() {
	mesh::term();
	shader::term();
	if (root_frame) delete root_frame;
}

void render::preframe() {
	if (root_frame) delete root_frame;
	root_frame = nullptr;
	game::realm const * r = game::get_root_realm();
	if (!r) return;
	root_frame = new protoframe {*r, r->get_default_perspective()};
}

void render::frame() {
	
	render::command_sequence seq {*root_frame};
	seq.execute();
	
	glfwSwapBuffers(glfw::win);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
