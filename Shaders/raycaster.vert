uniform float time;
varying vec4 pos;


void main()
{
	gl_Position = inverse(gl_ModelViewProjectionMatrix) * gl_Vertex;
    pos = gl_Position;
}
