#version 120

varying vec4 fragColor;
varying vec2 fragLocalCord;
varying vec4 fragBorderColor;

void main()
{

	// Output color = red 
	const float borderWith = 0.9;
	float dist = pow(fragLocalCord.x,2)+pow(fragLocalCord.y,2);
	if(dist >1)
		gl_FragColor = vec4(0,0,0,0);
	else if(dist>0.7)
		gl_FragColor = fragBorderColor;
	else
		gl_FragColor = fragColor;
	
	//gl_FragColor = vec4(0,0,0,1);

}