#include "commandseq.hh"

using namespace cichlid;

render::command_sequence::command_sequence(protoframe const & proto) {
	
	b::mat4_t<length_t> perspective = mat4_t::perspective(b::deg2rad<length_t>(90), 800, 600, 0.1, 100);
	b::mat4_t<length_t> view = mat4_t::translate(-proto.pers.pos) * mat4_t {proto.pers.vec.inverse()};
	
	for (object::cube const & c : proto.cubes) {
		b::mat4_t<length_t> model = c.trans.to_matrix();
		b::mat4_t<length_t> mvp = model * view * perspective;
		this->basic_seq.push_back( { b::mat4_t<float> {mvp} } );
	}
}

void render::command_sequence::execute() {
	shader::bind(shader::type::basic);
	for (shader::basic const & b : basic_seq) {
		shader::upload(b);
		mesh::draw();
	}
}
