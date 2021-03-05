#version 120
// Input vertex data, different for all executions of this shader.
attribute vec2 position;
attribute vec4 color;
attribute vec2 localCord;

varying vec4 fragColor;
varying vec2 fragLocalCord;

uniform mat4 mpv;

void main(){

	gl_Position = vec4(position,0.0, 1.0) * mpv;
	fragColor = color;
	fragLocalCord = localCord;

}