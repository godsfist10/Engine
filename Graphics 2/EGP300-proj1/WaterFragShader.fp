#version 330

uniform sampler2D myTexture;
// Make geometry solid
uniform vec4 vColorValue;

in vec2 texCoordPass;

// Output fragment color
out vec4 vFragColor;

void main(void)
{
	gl_FragColor = texture2D(myTexture, texCoordPass);
   //vFragColor = vColorValue;
}