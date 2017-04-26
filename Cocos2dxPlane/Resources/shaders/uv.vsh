attribute vec3 a_position;
attribute vec2 a_texCoord;

// Varyings
varying vec2 v_texCoord;

void main()
{
    gl_Position  = CC_MVPMatrix * vec4(a_position,1);
    v_texCoord   = a_texCoord;
	v_texCoord.y = 1.0 - v_texCoord.y;
}
