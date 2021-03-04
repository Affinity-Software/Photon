#version 120

varying vec4 fragColor;
varying vec2 fragLocalCord;

void main()
{

	// Output color = red 
	const float borderWith = 0.98;
	if(abs(fragLocalCord.x) > borderWith || 
	   abs(fragLocalCord.y) > borderWith)
		gl_FragColor = vec4(0,0,0,1);
	else
		gl_FragColor = fragColor;
	
	
}