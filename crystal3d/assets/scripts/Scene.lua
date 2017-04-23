function Init()
	--Camera
	cameraTransform = Transform.new(Vector3.new(0,0,-10), Vector3.new(0,0,0), Vector3.new(0,0,0))
	dimensions = GameWindow:GetSize()

	cameraNode = CameraNode.Create(cameraTransform)
	cameraNode.Frustum.Width = dimensions.X
	cameraNode.Frustum.Height = dimensions.Y
	cameraNode.Frustum.FOV = dVector3.new(45.0,45.0,0.0)

	this:AddNode(cameraNode)

	--Player
	model1 = ImportModel("Meshes\\gunslinger_triangle.obj")
	playerTransform = Transform.new(Vector3.new(0,80,0), Vector3.new(0,0,0), Vector3.new(3,3,3))
	playerNode = SceneNode.Create(playerTransform)

	for i=1,#model1.Meshes do
		local meshNode = MeshNode.Create(Transform.new())

		local mesh = model1.Meshes[i]
		local material = model1.Materials[mesh.MaterialIndex + 1] --lua index is 1 based

		meshNode:SetMesh(mesh)
		meshNode:SetMaterial(material)

		meshNode.Transform:SetParent(playerNode.Transform)
		this:AddNode(meshNode)
	end

	this:AddNode(playerNode)
	
	--Terrain
	terrainNode01 = TerrainNode.Create(Transform.new(Vector3.new(-200,0,-200), Vector3.new(0,0,0), Vector3.new(5,1,5)))
	
	terrain = Terrain.new()
	terrain.Heightmap = "Textures\\Heightmaps\\heightmap.tga"
	terrain.NormalMap = "Textures\\NormalMap.tga"
	terrain.DisplacementScale = 120
	terrain.Size = iVector2.new(500,500)
	
	terrain.DiffuseTextures[1] = "Textures\\stone02.tga"
	terrain.DiffuseTextures[2] = "Textures\\Seamless-Rock-Wall-2.tga"
	terrain.NormalMaps[1] = "Textures\\stone02_normal.tga"
	terrain.NormalMaps[2] = "Textures\\Seamless-Rock-Wall-2_normal.tga"
	
	terrain.GrassTexture = "Textures\\d3cb5d5d2cfa938f42677906370fecba.tga"
	terrain.GrassMap = "Textures\\grassMap.tga"
	
	terrainNode01:SetTerrain(terrain)
	this:AddNode(terrainNode01)
end

function Update(delta)
	local cameraMovementSpeed = 0.8 --delta * 50
	local cameraRotationSpeed = 0.8 --delta * 100
	local treshold = 0.2

	if Input:IsKeyPressed(87) or Input:GetAxis(0,1) > treshold then --W
		cameraNode.Transform.Translation.X = cameraNode.Transform.Translation.X - cameraNode:GetLook().X * cameraMovementSpeed
		cameraNode.Transform.Translation.Y = cameraNode.Transform.Translation.Y - cameraNode:GetLook().Y * cameraMovementSpeed
		cameraNode.Transform.Translation.Z = cameraNode.Transform.Translation.Z - cameraNode:GetLook().Z * cameraMovementSpeed
	end
	
	if Input:IsKeyPressed(83) or Input:GetAxis(0,1) < -treshold then --S
		cameraNode.Transform.Translation.X = cameraNode.Transform.Translation.X + cameraNode:GetLook().X * cameraMovementSpeed
		cameraNode.Transform.Translation.Y = cameraNode.Transform.Translation.Y + cameraNode:GetLook().Y * cameraMovementSpeed
		cameraNode.Transform.Translation.Z = cameraNode.Transform.Translation.Z + cameraNode:GetLook().Z * cameraMovementSpeed
	end
	
	if Input:IsKeyPressed(65) or Input:GetAxis(0,2) < -treshold then --A
		cameraNode.Transform.Rotation.Y = cameraNode.Transform.Rotation.Y - cameraRotationSpeed
	end
	
	if Input:IsKeyPressed(68) or Input:GetAxis(0,2) > treshold then --D
		cameraNode.Transform.Rotation.Y = cameraNode.Transform.Rotation.Y + cameraRotationSpeed
	end
	
	if Input:IsKeyPressed(81) then --Q
		cameraNode.Transform.Translation.Y = cameraNode.Transform.Translation.Y - cameraMovementSpeed
	end
	
	if Input:IsKeyPressed(69) then --E
		cameraNode.Transform.Translation.Y = cameraNode.Transform.Translation.Y + cameraMovementSpeed
	end
end