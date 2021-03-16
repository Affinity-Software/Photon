#version 120
// Input vertex data, different for all executions of this shader.
attribute vec2 position;
attribute vec4 color;
attribute vec2 localCord;
attribute vec2 topLeftRound;
attribute vec2 topRightRound;
attribute vec2 bottomLeftRound;
attribute vec2 bottomRightRound;
attribute vec4 borderColor;

varying vec4 fragColor;
varying vec2 fragLocalCord;
varying vec2 fragTopLeftRound;
varying vec2 fragTopRightRound;
varying vec2 fragBottomLeftRound;
varying vec2 fragBottomRightRound;
varying vec4 fragBorderColor;

uniform mat4 mpv;

void main(){

	gl_Position = mpv * vec4(position,0.0, 1.0);
	fragColor = color;
	fragLocalCord = localCord;
	fragBorderColor = borderColor;
	fragTopLeftRound = topLeftRound;
	fragTopRightRound = topRightRound;
	fragBottomLeftRound = bottomLeftRound;
	fragBottomRightRound = bottomRightRound;

}