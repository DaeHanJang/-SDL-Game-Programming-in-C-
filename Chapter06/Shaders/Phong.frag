// Request GLSL 3.3
#version 330

// Inputs from vertex shader
// Tex coord
in vec2 fragTexCoord;
//���� ���������� ����
in vec3 fragNormal;
//���� ���������� ��ġ
in vec3 fragWorldPos;

// This corresponds to the output color to the color buffer
out vec4 outColor;

// This is used for the texture sampling
uniform sampler2D uTexture;

//���Ɽ�� ���� ����ü ����
struct DirectionalLight
{
	//���Ɽ
	vec3 mDirection;
	//���ݻ� ����
	vec3 mDiffuseColor;
	//���ݻ� ����
	vec3 mSpecColor;
};

//������ ���� uniform
//���� ���������� ī�޶� ��ġ
uniform vec3 uCameraPos;
//�ֺ���
uniform float uSpecPower;
//ǥ�鿡 ���� ���ݻ� ����
uniform vec3 uAmbientLight;
//���Ɽ
uniform DirectionalLight uDirLight;

void main()
{
	//ǥ�� ����
	vec3 N = normalize(fragNormal);
	//ǥ�鿡�� ���������� ����
	vec3 L = normalize(-uDirLight.mDirection);
	//ǥ�鿡�� ī�޶�� ���ϴ� ����
	vec3 V = normalize(uCameraPos - fragWorldPos);
	//N�� ���� -L�� �ݻ�
	vec3 R = normalize(reflect(-L, N));

	//�� �ݻ� ���
	vec3 Phong = uAmbientLight;
	float NdotL = dot(N, L);
	if (NdotL > 0)
	{
		vec3 Diffuse = uDirLight.mDiffuseColor * NdotL;
		vec3 Specular = uDirLight.mSpecColor * pow(max(0.0, dot(R, V)), uSpecPower);
		Phong += Diffuse + Specular;
	}

	//�������� �ؽ�ó ���� ���ϱ� �� ����(���İ� = 1)
    outColor = texture(uTexture, fragTexCoord) * vec4(Phong, 1.0f);
}
