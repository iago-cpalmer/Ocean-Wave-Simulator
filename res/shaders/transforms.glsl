uniform mat4x4 Model;
uniform mat4x4 View;
uniform mat4x4 Projection;

struct VertexPosition
{
	vec4 CS_Position; // Clip space
	vec3 WS_Position; // World space
	vec3 VS_Position; // View space
};

VertexPosition get_vertex_positions(vec3 position)
{
	VertexPosition vPos;
	vPos.WS_Position = vec3(Model * vec4(position, 1.0));
	vPos.VS_Position = vec3(View * vec4(vPos.WS_Position, 1.0));
	vPos.CS_Position = Projection * vec4(vPos.VS_Position, 1.0);
	return vPos;
}

struct NormalPosition
{
	vec3 WS_Normal;
};

NormalPosition get_normal_positions(vec3 normal)
{
	NormalPosition normals;
	normals.WS_Normal = mat3(transpose(inverse(Model))) * normal;

	return normals;
}