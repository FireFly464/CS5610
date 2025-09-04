#version 330 core


layout(location = 0) out vec4 color;

void main()
{
	float depth = (gl_FragCoord.z - 0.996)*300;
	color = vec4(depth,depth,depth,1);
}