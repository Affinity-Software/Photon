#version 120

attribute vec2 position;
attribute float localCord;

attribute vec2 borderRounding0;
attribute vec2 borderRounding1;
attribute vec2 borderRounding2;
attribute vec2 borderRounding3;

attribute float borderWith0;
attribute float borderWith1;
attribute float borderWith2;
attribute float borderWith3;

attribute vec4 borderColor0;
attribute vec4 borderColor1;
attribute vec4 borderColor2;
attribute vec4 borderColor3;

attribute vec4 color;

varying vec4 fColor;
varying vec2 fLocalCord;
varying vec2 fRound[4];
varying vec2 fRoundIner[4];
varying vec4 fBorderColor[4];
varying float fBorderWith[4];

uniform mat4 mpv;

vec2 ScaleGrid(vec2 point, vec2 scale)
{
	const vec2 scaleOrigin = vec2(1,1);
	return ((point - scaleOrigin) * scale) + scaleOrigin;
}

void main(){

	gl_Position = mpv * vec4(position,0.0, 1.0);
	fColor = color;

	if(localCord == 0.0f) // top left
	{
		fLocalCord = vec2(-1,-1);
		fRound[0] = vec2(1,1);
		fRound[1] = ScaleGrid(vec2(0,1),borderRounding1);
		fRound[2] = ScaleGrid(vec2(0,0),borderRounding2);
		fRound[3] = ScaleGrid(vec2(1,0),borderRounding3);
	}
	else if(localCord == 1.0f) // top right
	{
		fLocalCord = vec2(1,-1);
		fRound[0] = ScaleGrid(vec2(0,1),borderRounding0);
		fRound[1] = vec2(1,1);
		fRound[2] = ScaleGrid(vec2(1,0),borderRounding2);
		fRound[3] = ScaleGrid(vec2(0,0),borderRounding3);
	}
	else if(localCord == 2.0f) // bottom right
	{
		fLocalCord = vec2(1,1);
		fRound[0] = ScaleGrid(vec2(0,0),borderRounding0);
		fRound[1] = ScaleGrid(vec2(1,0),borderRounding1);
		fRound[2] = vec2(1,1);
		fRound[3] = ScaleGrid(vec2(0,1),borderRounding3);
	}
	else if(localCord == 3.0f) // bottom left
	{
		fLocalCord = vec2(-1,1);
		fRound[0] = ScaleGrid(vec2(1,0),borderRounding0);
		fRound[1] = ScaleGrid(vec2(0,0),borderRounding1);
		fRound[2] = ScaleGrid(vec2(0,1),borderRounding2);
		fRound[3] = vec2(1,1);
	}


	fRoundIner[0] = fRound[0] / (vec2(1) - vec2(borderWith1,borderWith2) * borderRounding0);
	fRoundIner[1] = fRound[1] / (vec2(1) - vec2(borderWith3,borderWith2) * borderRounding1);
	fRoundIner[2] = fRound[2] / (vec2(1) - vec2(borderWith3,borderWith0) * borderRounding2);
	fRoundIner[3] = fRound[3] / (vec2(1) - vec2(borderWith1,borderWith0) * borderRounding3);

	fBorderColor[0] = borderColor0;
	fBorderColor[1] = borderColor1;
	fBorderColor[2] = borderColor2;
	fBorderColor[3] = borderColor3;
	fBorderWith[0] = 1.0 - borderWith0*2;
	fBorderWith[1] = 1.0 - borderWith1*2;
	fBorderWith[2] = 1.0 - borderWith2*2;
	fBorderWith[3] = 1.0 - borderWith3*2;
}