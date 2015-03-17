#version 330

uniform sampler2D myTexture;

in vec2 texCoordPass;

// Output fragment color

void main(void)
{
	gl_FragColor = texture2D(myTexture, texCoordPass);
}