// Request GLSL 3.3
#version 330

// Uniforms for world transform and view-proj
uniform mat4 uWorldTransform;
uniform mat4 uViewProj;

// Attribute 0 is position, 1 is normal, 2 is tex coords.
layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;

// Any vertex outputs (other than position)
out vec2 fragTexCoord;
//세계 공간에서의 법선
out vec3 fragNormal;
//세계 공간에서의 위치
out vec3 fragWorldPos;

void main()
{
	//위치를 동차 좌표로 변환
	vec4 pos = vec4(inPosition, 1.0);
	//위치를 세계 공간상의 위치로 변환
	pos = pos * uWorldTransform;
	//세계 공간상의 위치를 저장
	fragWorldPos = pos.xyz;
	//위치를 클립 공간 좌표로 변환
	gl_Position = pos * uViewProj;

	//법선을 세계 공간상의 법선으로 변환(w = 0)
	fragNormal = (vec4(inNormal, 0.0f) * uWorldTransform).xyz;

	//텍스처 좌표를 프래그먼트 셰이더로 전달
	fragTexCoord = inTexCoord;
}
