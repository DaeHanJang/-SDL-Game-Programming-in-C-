// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

// Request GLSL 3.3
#version 330

//���ؽ� ���̴����� ���� �ؽ�ó ��ǥ �Է°�
// Tex coord input from vertex shader
in vec2 fragTexCoord;
in vec3 fragVertColor;

//��� ����
// This corresponds to the output color to the color buffer
out vec4 outColor;

//�ؽ�ó ���ø��� ���� �ʿ�
// This is used for the texture sampling
uniform sampler2D uTexture;

void main()
{
	//�ؽ�ó�κ��� ������ ���ø�
	// Sample color from texture
	vec4 texColor = texture(uTexture, fragTexCoord);
	vec3 finalColor = texColor.rgb * fragVertColor;
	outColor = vec4(finalColor, texColor.a);
}
