#version 130

uniform sampler2D tex;
 varying float vRotation;
void main(void)
{

 	float mid = 0.5;
 	//float vRotation = (gl_Normal.y*3.14)/(180*4);
    vec2 rotated = vec2(cos(vRotation) * (gl_PointCoord.x - mid) + sin(vRotation) * (gl_PointCoord.y - mid) + mid,
                        cos(vRotation) * (gl_PointCoord.y - mid) - sin(vRotation) * (gl_PointCoord.x - mid) + mid);


	//vec2 texCoord = (rotMatrix * vec4(gl_PointCoord, 0, 1)).xy;
	//gl_FragColor = gl_Color * texture2D(s_texture, texCoord);
    
    
    //gl_FragColor =gl_Color*texture2D(tex, gl_TexCoord[0].xy);
    vec4 rotatedTexture=texture2D(tex, rotated);
    //gl_FragColor =gl_Color*texture2D(tex, gl_TexCoord[0].xy);
    gl_FragColor =  gl_Color * rotatedTexture;
    
    //gl_FragColor =gl_Color*texture2D(tex, gl_TexCoord[0].xy);
}