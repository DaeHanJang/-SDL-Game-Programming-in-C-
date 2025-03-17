// Request GLSL 3.3
#version 330

// Inputs from vertex shader
// Tex coord
in vec2 fragTexCoord;
//세계 공간에서의 법선
in vec3 fragNormal;
//세계 공간에서의 위치
in vec3 fragWorldPos;

// This corresponds to the output color to the color buffer
out vec4 outColor;

// This is used for the texture sampling
uniform sampler2D uTexture;

//방향광을 위한 구조체 정의
struct DirectionalLight
{
	//방향광
	vec3 mDirection;
	//난반사 색상
	vec3 mDiffuseColor;
	//정반사 색상
	vec3 mSpecColor;
};

//조명을 위한 uniform
//세계 공간에서의 카메라 위치
uniform vec3 uCameraPos;
//주변광
uniform float uSpecPower;
//표면에 대한 정반사 지수
uniform vec3 uAmbientLight;
//방향광
uniform DirectionalLight uDirLight;

void main()
{
	//표면 법선
	vec3 N = normalize(fragNormal);
	//표면에서 광원으로의 벡터
	vec3 L = normalize(-uDirLight.mDirection);
	//표면에서 카메라로 향하는 벡터
	vec3 V = normalize(uCameraPos - fragWorldPos);
	//N에 대한 -L의 반사
	vec3 R = normalize(reflect(-L, N));

	//퐁 반사 계산
	vec3 Phong = uAmbientLight;
	float NdotL = dot(N, L);
	if (NdotL > 0)
	{
		vec3 Diffuse = uDirLight.mDiffuseColor * NdotL;
		vec3 Specular = uDirLight.mSpecColor * pow(max(0.0, dot(R, V)), uSpecPower);
		Phong += Diffuse + Specular;
	}

	//최종색은 텍스처 색상 곱하기 퐁 광원(알파값 = 1)
    outColor = texture(uTexture, fragTexCoord) * vec4(Phong, 1.0f);
}
