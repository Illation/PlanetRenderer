<VERTEX>
	#version 330 core
	
	layout (location = 0) in vec3 position;
	
	uniform mat4 model;
	uniform mat4 viewProj;
	
	out vec3 Position;
	out vec3 Tex3;
	out vec3 Normal;
	
	void main()
	{
		Normal = normalize((model*vec4(position, 1.0f)).xyz);
		gl_Position = viewProj * model * vec4(position, 1.0f);
		Tex3 = normalize(position);
	}
</VERTEX>
<FRAGMENT>
	#version 330 core
	
	in vec3 Position;
	in vec3 Tex3;
	in vec3 Normal;
	
	uniform sampler2D texDiffuse;
	
	uniform vec3 lightDir = vec3(-1, -0.3, 1);
	
	uniform vec3 diffuse = vec3(1.0f, 0.5f, 0.2f);
	uniform vec3 ambient = vec3(0.05f, 0.05f, 0.08f);

	out vec4 outColor;
	
	float Lambert(vec3 norm, vec3 lightDir)
	{
		return max(dot(norm, -lightDir), 0);
	}
	vec3 DirLighting(vec3 dif, vec3 norm)
	{
		vec3 diffuse = (dif) * Lambert(norm, lightDir);
		//vec3 specular = (spec * light.Color) * Blinn(norm, light.Direction, viewDir, specPow);
		return diffuse;// + specular;
	}
	void main()
	{
		vec3 tc3 = normalize(Tex3);
		vec2 uv;
		uv.x = atan( tc3.z, tc3.x );
		uv.y = acos( tc3.y );
		uv /= vec2( 2.0f * 3.14159265359f, 3.14159265359f );
		vec3 dif = texture(texDiffuse, uv).rgb;
	
		vec3 lit = DirLighting(dif, Normal);
		lit*=0.8f;
		outColor = vec4(lit+ambient*dif, 1.0f);
	} 
</FRAGMENT>