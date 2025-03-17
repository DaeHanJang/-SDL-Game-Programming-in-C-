// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

// Request GLSL 3.3
#version 330

//버텍스 셰이더에서 얻은 텍스처 좌표 입력값
// Tex coord input from vertex shader
in vec2 fragTexCoord;

//출력 색상
// This corresponds to the output color to the color buffer
out vec4 outColor;

//텍스처 샘플링을 위해 필요
// This is used for the texture sampling
uniform sampler2D uTexture;

void main()
{
	//텍스처로부터 색상을 샘플링
	// Sample color from texture
	outColor = texture(uTexture, fragTexCoord);
}
