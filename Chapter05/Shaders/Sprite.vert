// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

// Request GLSL 3.3
#version 330

//���� ��ȯ �� ��-���� ��Ŀ� ���� uniform
// Uniforms for world transform and view-proj
uniform mat4 uWorldTransform;
uniform mat4 uViewProj;

//�Ӽ� 0�� ��ġ, 1�� �ؽ�ó ��ǥ
// Attribute 0 is position, 1 is tex coords.
layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 inTexCoord;

//�ؽ�ó ��ǥ�� ��¿� �߰�
// Add texture coordinate as output
out vec2 fragTexCoord;

void main()
{
	//��ġ�� ���� ��ǥ�� ��ȯ
	// Convert position to homogeneous coordinates
	vec4 pos = vec4(inPosition, 1.0);
	//��ġ�� ���� �������� ��ȯ�� �� Ŭ�� �������� ��ȯ
	// Transform position to world space, then clip space
	gl_Position = pos * uWorldTransform * uViewProj;

	//�ؽ�ó ��ǥ�� �����׸�Ʈ ���̴��� ����
	// Transform
	// Pass along the texture coordinate to frag shader
	fragTexCoord = inTexCoord;
}
