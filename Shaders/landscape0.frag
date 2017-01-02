uniform float time;
uniform sampler2D tex_map;
uniform sampler2D tex_1;
uniform sampler2D tex_2;
uniform sampler2D tex_3;
uniform float scaling;
varying vec2 tex_coord;


vec4 get_color(in sampler2D texture, in float x, in float y)
{
    if (x < 0.0) x = 0.0;
    if (y < 0.0) y = 0.0;
    if (x > 1.0) x = 1.0;
    if (y > 1.0) y = 1.0;
    
    vec2 coord = vec2(x, y);
    return texture2D(texture, coord);
}


vec4 blend_color(in sampler2D texture, in vec2 center)
{
    vec4 colors[4];
    float d = 0.0001;
    
    colors[0] = get_color(texture, center[0] - d, center[1]    );
    colors[1] = get_color(texture, center[0],     center[1] - d);
    colors[2] = get_color(texture, center[0] + d, center[1]    );
    colors[3] = get_color(texture, center[0],     center[1] + d);
    
    vec4 mid = vec4(0.0, 0.0, 0.0, 0.0);
    
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            mid[j] += colors[i][j];
        }
    }
    
    vec4 color = get_color(texture, center[0], center[1]);
    
    for (int i = 0; i < 4; i++) {
        color[i] = 0.5 * (mid[i] / 4.0) + 0.5 * color[i];
    }
    
    return color;
}


void main()
{
    //vec2 map_coord = vec2(1.0 - tex_coord[0] * scaling, tex_coord[1] * scaling);
    vec2 map_coord = vec2(1.0 - (tex_coord[0]) / scaling, (tex_coord[1]) / scaling);
	/*vec4 map_color = texture2D(tex_map, map_coord);
	vec4 tex_1_color = texture2D(tex_1, tex_coord);
	vec4 tex_2_color = texture2D(tex_2, tex_coord);
	vec4 tex_3_color = texture2D(tex_3, tex_coord);*/
    vec4 map_color = blend_color(tex_map, map_coord);
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
