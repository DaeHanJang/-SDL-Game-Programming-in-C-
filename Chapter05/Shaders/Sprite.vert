// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

// Request GLSL 3.3
#version 330

//세계 변환 및 뷰-투영 행렬에 대한 uniform
// Uniforms for world transform and view-proj
uniform mat4 uWorldTransform;
uniform mat4 uViewProj;

//속성 0은 위치, 1은 텍스처 좌표
// Attribute 0 is position, 1 is tex coords.
layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 inTexCoord;

//텍스처 좌표를 출력에 추가
// Add texture coordinate as output
out vec2 fragTexCoord;

void main()
{
	//위치를 동차 좌표로 변환
	// Convert position to homogeneous coordinates
	vec4 pos = vec4(inPosition, 1.0);
	//위치를 세계 공간으로 변환한 뒤 클립 공간으로 변환
	// Transform position to world space, then clip space
	gl_Position = pos * uWorldTransform * uViewProj;

	//텍스처 좌표를 프래그먼트 셰이더에 전달
	// Transform
	// Pass along the texture coordinate to frag shader
	fragTexCoord = inTexCoord;
}
