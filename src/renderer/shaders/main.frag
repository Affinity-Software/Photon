#version 120

varying vec4 fragColor;
varying vec2 fragLocalCord;
varying vec2 fragTopLeftRound;
varying vec2 fragTopRightRound;
varying vec2 fragBottomLeftRound;
varying vec2 fragBottomRightRound;
varying vec4 fragBorderColor;

vec4 isBorder(vec2 corddinate, vec4 bc, vec4 fc)
{
	float dist = pow(corddinate.x,2) + pow(corddinate.y,2);
	if(dist > 1)
		return vec4(0,0,0,1);
	else if (dist > .8)
		return bc;
	else
		return fc;
}

void main()
{

	if(fragTopLeftRound.x > 0 && fragTopLeftRound.y > 0)
		gl_FragColor = isBorder(fragTopLeftRound,fragBorderColor,fragColor);
	else if(fragTopRightRound.x > 0 && fragTopRightRound.y > 0)
		gl_FragColor = isBorder(fragTopRightRound,fragBorderColor,fragColor);
	else if(fragBottomRightRound.x > 0 && fragBottomRightRound.y > 0)
		gl_FragColor = isBorder(fragBottomRightRound,fragBorderColor,fragColor);
	else if(fragBottomLeftRound.x > 0 && fragBottomLeftRound.y > 0)
		gl_FragColor = isBorder(fragBottomLeftRound,fragBorderColor,fragColor);
	else
	{
		if(fragLocalCord.x < .95 || fragLocalCord.y < .95)
			gl_FragColor = fragColor;
		else
			gl_FragColor = fragBorderColor;
	}
	
	//gl_FragColor = vec4(fragBottomRightRound,0,1);

}