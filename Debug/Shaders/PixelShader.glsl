#version 430

out vec4 fragColor;

in vec2 texCoord;
uniform sampler2D tex;

in vec3 L;
in vec3 E;
in vec3 H;
in vec3 N;

void main()
{
	vec3 ambientProduct = vec3(.1);
	vec3 diffuseProduct = vec3(.8);
	vec3 specularProduct = vec3(.1);

	float gloss = 75.0;
	
	vec4 hue = texture(tex, texCoord);

	float Kd = max(dot(L,N), 0.0);
	float Ks = pow(max(dot(N,H), 0.0), gloss);

	vec3 ambient = hue.rgb * ambientProduct;
	vec3 diffuse = hue.rgb * Kd * diffuseProduct;
	vec3 specular = vec3(1) * Ks * specularProduct;

	if(dot(L,N) < 0.0)
		specular = vec3(0.0, 0.0, 0.0);

	vec4 color = vec4((ambient + diffuse + specular), hue.a);

    fragColor = color;
}
