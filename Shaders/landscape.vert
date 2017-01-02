uniform float time;
uniform float scaling;
varying vec2 tex_coord;


void main()
{
	gl_Position    = gl_ModelViewProjectionMatrix * gl_Vertex;
	gl_FrontColor  = gl_Color;
	gl_TexCoord[0] = gl_MultiTexCoord0;
	
	tex_coord = gl_MultiTexCoord0.st;
    tex_coord[0] *= scaling;
    tex_coord[1] *= scaling;

	//gl_Position[1] += 1.0 * sin(time + 5.0 * gl_TexCoord[0][0]);
}
