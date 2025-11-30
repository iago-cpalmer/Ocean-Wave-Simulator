#ifndef MESHES_H
#define MESHES_H

/// FILE TO CREATE MESH DATA ARRAYS TO AVOID HAVING THEM IN THE MAIN.CPP OCCUPYING LOTS OF SPACE

// ------------------------------------
// CUBE
// ------------------------------------
float VERTICES_CUBE[] = {
	// Front face	
	// Position        // Normal         // Texture   
	 0.0f, 0.0f,  0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
	 0.0f, 1.0f,  0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
	 1.0f, 0.0f,  0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
	 1.0f, 1.0f,  0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
						
	 // Back face		
	 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
	 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
	 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
	 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
						
	 // Top face		
	 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
	 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
	 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
	 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
						
	 // Bottom face		
	 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
	 0.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
	 1.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
	 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
						
	 // Left face		
	 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
	 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
	 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
	 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
						
	 // Right face		
	 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
	 0.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
	 0.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
	 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
};

unsigned int INDICES_CUBE[] = {
	// Front face
	0, 3, 2, 0, 1, 3,

	// Back face
	4 , 7 , 5 , 4 ,6 , 7,

	// Top face
	8, 10, 11, 8, 9, 10,

	// Bottom face
	14,  13, 12, 12, 15, 14,

	// Left face
	16, 18, 19, 16, 17, 18,

	// Right face
	20, 23, 22, 20, 22, 21
};

#endif // !MESHES_H

// ------------------------------------
// PLANE
// ------------------------------------
float VERTICES_PLANE[] = {
	 0.0f, 0.0f,  0.0f,  0.0f, 0.0f,
	 0.0f, 1.0f,  0.0f,  0.0f, 1.0f,
	 1.0f, 0.0f,  0.0f,  1.0f, 0.0f,
	 1.0f, 1.0f,  0.0f,  1.0f, 1.0f
};

unsigned int INDICES_PLANE[] = {
	0, 3, 2, 0, 1, 3,
	1, 0, 3, 0, 2, 3
};


void create_mesh_grid(Mesh *pMesh, int nx, int nz)
{
	std::vector<Vertex> vertices(nx*nz);
	std::vector<unsigned int> indices(vertices.size()+2);

	for(int x = 0; x < nx; ++x)
	{
		for(int z = 0; z < nz; ++z)
		{
			vertices[x * nz + z] = Vertex{glm::vec3(x, 0.0f, z), 
				glm::vec3(0.0f, 1.0f, 0.0f), 
				glm::vec2(0.0f)};
		}	
	}

	for(int x = 0; x < nx-1; ++x)
	{
		for(int z = 0; z < nz-1; ++z)
		{
			int a = x * nz + z;
			int b = a + 1;
			int c = a + nz;
			int d = c + 1;
			indices.push_back(a);
			indices.push_back(b);
			indices.push_back(d);

			indices.push_back(a);
			indices.push_back(d);
			indices.push_back(c);
		}	
	}

	VertexAttribute vertexAttributes[] =
    {
        VertexAttribute{VertexAttributeType::FLOAT, 3},	    // Position
        VertexAttribute{VertexAttributeType::FLOAT, 3},	    // Normal
        VertexAttribute{VertexAttributeType::FLOAT, 2}		// UV
    };

	create_mesh(pMesh, vertexAttributes, 3, nullptr, vertices.size(), indices.data(), indices.size(), 1, GL_STATIC_DRAW);

    vbo_set_data(pMesh->Vbo, vertices.data(), sizeof(Vertex) * vertices.size(), 0);

    pMesh->Submeshes[0] = Submesh{ 0, (unsigned int)indices.size() };
}