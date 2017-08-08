#include "control.hh"
#include "game/game.hh"

#include "bridge/glfw_common.hh"

using namespace cichlid;

bool glfw::initialized = false;
GLFWwindow * glfw::win = nullptr;

static bool cursor_enabled;
static void key_cb(GLFWwindow *, int key, int /*scancode*/, int action, int /*mods*/) {
	switch(key) {
		case GLFW_KEY_ESCAPE:
			if (action == GLFW_PRESS) glfwSetWindowShouldClose(glfw::win, GLFW_TRUE);
			break;
		case GLFW_KEY_LEFT_ALT:
			if (action == GLFW_PRESS) {
				cursor_enabled = !cursor_enabled;
				glfwSetInputMode(glfw::win, GLFW_CURSOR, cursor_enabled ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
			}
			break;
		default:
			break;
	}
}

static double last_x, last_y;
static void mouse_cb(GLFWwindow *, double x, double y) {
	if (!cursor_enabled) {
		game::ci.axial_turn.y() += last_x - x;
		game::ci.axial_turn.x() += last_y - y;
	}
	last_x = x;
	last_y = y;
}

void control::init() {
	if (glfwInit() != GLFW_TRUE) {
		cilogve("GLFW failed to initialize");
		throwe(startup);
	}
	glfw::initialized = true;
	glfw::win = glfwCreateWindow(800, 600, "TEST", nullptr, nullptr);
	if (!glfw::win) {
		cilogve("GLFW failed to create a window");
		throwe(startup);
	}
	
	glfwSetInputMode(glfw::win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	cursor_enabled = false;
	
	glfwGetCursorPos(glfw::win, &last_x, &last_y);
	glfwSetCursorPosCallback(glfw::win, mouse_cb);
	
	glfwSetKeyCallback(glfw::win, key_cb);
}

void control::term() {
	if (glfw::win) {
		glfwDestroyWindow(glfw::win);
		glfw::win = nullptr;
	}
	if (glfw::initialized) {
		glfwTerminate();
		glfw::initialized = false;
	}
}
	
void control::frame() {
	glfwPollEvents();
	
	// TEMP MOVE
	
	if (glfwGetKey(glfw::win, GLFW_KEY_W) == GLFW_PRESS) {
		game::ci.axial_move.z() += 1;
	}
	
	if (glfwGetKey(glfw::win, GLFW_KEY_S) == GLFW_PRESS) {
		game::ci.axial_move.z() -= 1;
	}
	
	if (glfwGetKey(glfw::win, GLFW_KEY_D) == GLFW_PRESS) {
		game::ci.axial_move.x() += 1;
	}
	
	if (glfwGetKey(glfw::win, GLFW_KEY_A) == GLFW_PRESS) {
		game::ci.axial_move.x() -= 1;
	}
	
	if (glfwGetKey(glfw::win, GLFW_KEY_SPACE) == GLFW_PRESS) {
		game::ci.axial_move.y() += 1;
	}
	
	if (glfwGetKey(glfw::win, GLFW_KEY_C) == GLFW_PRESS) {
		game::ci.axial_move.y() -= 1;
	}
	
	// TEMP TURN
	
	if (glfwGetKey(glfw::win, GLFW_KEY_E) == GLFW_PRESS) {
		game::ci.axial_turn.z() += 1;
	}
	
	if (glfwGetKey(glfw::win, GLFW_KEY_Q) == GLFW_PRESS) {
		game::ci.axial_turn.z() -= 1;
	}
	
	// OTHER
	
	if (glfwWindowShouldClose(glfw::win)) com::run_sem.store(false);
}
