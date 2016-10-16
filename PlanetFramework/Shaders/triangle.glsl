<VERTEX>
	#version 330 core
	
	layout (location = 0) in vec3 position;
	
	uniform mat4 model;
	uniform mat4 viewProj;
	
	out vec3 Position;
	out vec3 Normal;
	
	void main()
	{
		Normal = normalize((model*vec4(position, 1.0f)).xyz);
		gl_Position = viewProj * model * vec4(position, 1.0f);
	}
</VERTEX>
<FRAGMENT>
	#version 330 core
	
	in vec3 Position;
	in vec3 Normal;
	
	uniform vec3 lightDir = vec3(-1, -1, 1);
	
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
		vec3 lit = DirLighting(diffuse, Normal);
		outColor = vec4(lit+ambient, 1.0f);
	} 
</FRAGMENT>