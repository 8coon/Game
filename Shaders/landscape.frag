uniform float time;
uniform sampler2D tex_map;
uniform sampler2D tex_1;
uniform sampler2D tex_2;
uniform sampler2D tex_3;
uniform float scaling;
varying vec2 tex_coord;

void main()
{
    //vec2 map_coord = vec2(1.0 - tex_coord[0] * scaling, tex_coord[1] * scaling);
    vec2 map_coord = vec2(1.0 - (tex_coord[0]) / scaling, (tex_coord[1]) / scaling);
	vec4 map_color = texture2D(tex_map, map_coord);
	vec4 tex_1_color = texture2D(tex_1, tex_coord);
	vec4 tex_2_color = texture2D(tex_2, tex_coord);
	vec4 tex_3_color = texture2D(tex_3, tex_coord);

	gl_FragColor = vec4(0.0, 0.0, 0.0, 0.0);

	gl_FragColor += tex_1_color * 
		vec4(map_color.r, map_color.r, map_color.r, map_color.r);
	gl_FragColor += tex_2_color * 
		vec4(map_color.g, map_color.g, map_color.g, map_color.g);
	gl_FragColor += tex_3_color *
		vec4(map_color.b, map_color.b, map_color.b, map_color.b);
    
    //gl_FragColor = vec4(abs(sin(time)), abs(cos(time)), abs(sin(time)), 1.0);
    //s, t, p, q
}
