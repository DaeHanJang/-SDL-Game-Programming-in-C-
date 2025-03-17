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

struct PointLight {
	vec3 mPosition; //��ġ
	vec3 mDiffuseColor; //���ݻ� ����
	vec3 mSpecColor; //���ݻ� ����
	vec3 mSpecPower; //���ݻ� ����
	float mRange; //����
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
uniform PointLight uPotLight[4];

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

	for (int i = 0; i < 4; i++) {
		vec3 L2 = normalize(uPotLight[i].mPosition - fragWorldPos);
		vec3 R2 = normalize(reflect(-L2, N));

		float NdotL2 = dot(N, L2);
		if (NdotL2 > 0)
		{
			vec3 Diffuse2 = uPotLight[i].mDiffuseColor * NdotL2;
			vec3 Specular2 = uPotLight[i].mSpecColor * pow(max(0.0, dot(R2, V)), uSpecPower);

			float len = pow(uPotLight[i].mPosition.x - fragWorldPos.x, 2.0f) +
						pow(uPotLight[i].mPosition.y - fragWorldPos.y, 2.0f) + 
						pow(uPotLight[i].mPosition.z - fragWorldPos.z, 2.0f);
			if (pow(uPotLight[i].mRange, 2.0f) >= len) Phong += Diffuse2 + Specular2;
		}
	}

	//�������� �ؽ�ó ���� ���ϱ� �� ����(���İ� = 1)
    outColor = texture(uTexture, fragTexCoord) * vec4(Phong, 1.0f);
}
