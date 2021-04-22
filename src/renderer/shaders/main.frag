#version 120
varying vec4 fColor;
varying vec2 fLocalCord;
varying vec2 fRound[4];
varying vec2 fRoundIner[4];
varying vec4 fBorderColor[4];
varying float fBorderWith[4];

vec4 getBorderColor()
{
	if(abs(fLocalCord.x) > abs(fLocalCord.y))
		if(fLocalCord.x > 0)
			return fBorderColor[3];
		else
			return fBorderColor[1];
	else
		if(fLocalCord.y > 0)
			return fBorderColor[0];
		else
			return fBorderColor[2];
}

vec4 isBorder(int id)
{
	float dist = pow(fRound[id].x,2) + pow(fRound[id].y,2);
	float inerDist = pow(fRoundIner[id].x,2) + pow(fRoundIner[id].y,2);
	//return vec4(inerDist,0,0,1);
	if(dist > 1)
		return vec4(0);
	else if (inerDist > 1)
		return getBorderColor();
	else
		return fColor;
}

void main()
{
	if(fRound[0].x > 0 && fRound[0].y > 0)
		gl_FragColor = isBorder(0);
	else if(fRound[1].x > 0 && fRound[1].y > 0)
		gl_FragColor = isBorder(1);
	else if(fRound[2].x > 0 && fRound[2].y > 0)
		gl_FragColor = isBorder(2);
	else if(fRound[3].x > 0 && fRound[3].y > 0)
		gl_FragColor = isBorder(3);
	else
	{
		if(
			fLocalCord.y > fBorderWith[0] ||
			-fLocalCord.x > fBorderWith[1] ||
			-fLocalCord.y > fBorderWith[2] ||
			fLocalCord.x > fBorderWith[3]
		)
			gl_FragColor = getBorderColor();
		else
			gl_FragColor = fColor;
	}
}