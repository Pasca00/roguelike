#version 330

in vec2 tex_coord;

layout(location = 0) out vec4 out_color;

uniform sampler2D in_texture;

void main()
{    
	out_color = texture(in_texture, vec2(tex_coord.x, 1 - tex_coord.y));
	out_color = vec4(1.0, 1.0, 1.0, out_color.r);
}  