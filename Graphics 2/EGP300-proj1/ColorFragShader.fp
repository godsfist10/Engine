#version 330

uniform vec4 vColorBombValue;

void main(void)
{
	gl_FragColor = vColorBombValue;
}