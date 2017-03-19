function Init()
	--Camera
	cameraTransform = Transform.new(Vector3.new(0,50,50), Vector3.new(0,180,0), Vector3.new(0,0,0))
	dimensions = GameWindow:GetSize()

	cameraNode = CameraNode.Create(cameraTransform)
	cameraNode.Frustum.Width = dimensions.X
	cameraNode.Frustum.Height = dimensions.Y
	cameraNode.Frustum.FOV = dVector3.new(45.0,45.0,0.0)

	this:AddNode(cameraNode)

	--Player
	model1 = Resources:LoadModel("Meshes\\gunslinger.obj", nil)
	playerTransform = Transform.new(Vector3.new(150,120,150), Vector3.new(0,0,0), Vector3.new(3,3,3))
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
	terrain.Heightmap = "Textures\\Heightmaps\\heightmap01.png"
	terrain.NormalMap = "Textures\\NormalMap.png"
	terrain.DisplacementScale = 120
	terrain.Size = iVector2.new(500,500)
	
	terrain.DiffuseTextures[1] = "Textures\\stone02.png"
	terrain.DiffuseTextures[2] = "Textures\\Seamless-Rock-Wall-2.jpg"
	terrain.NormalMaps[1] = "Textures\\stone02_normal.png"
	terrain.NormalMaps[2] = "Textures\\Seamless-Rock-Wall-2_normal.jpg"
	
	terrain.GrassTexture = "Textures\\d3cb5d5d2cfa938f42677906370fecba.png"
	terrain.GrassMap = "Textures\\grassMap.png"
	
	terrainNode01:SetTerrain(terrain)
	this:AddNode(terrainNode01)
end

function Update(delta)
	local cameraMovementSpeed = delta * 50
	local cameraRotationSpeed = delta * 100
	local treshold = 0.2

	if Input:IsKeyPressed(87) or Input:GetAxis(0,1) > treshold then --W
		this.Camera.Transform.Translation.X = this.Camera.Transform.Translation.X - this.Camera:GetLook().X * cameraMovementSpeed
		this.Camera.Transform.Translation.Y = this.Camera.Transform.Translation.Y - this.Camera:GetLook().Y * cameraMovementSpeed
		this.Camera.Transform.Translation.Z = this.Camera.Transform.Translation.Z - this.Camera:GetLook().Z * cameraMovementSpeed
	end
	
	if Input:IsKeyPressed(83) or Input:GetAxis(0,1) < -treshold then --S
		this.Camera.Transform.Translation.X = this.Camera.Transform.Translation.X + this.Camera:GetLook().X * cameraMovementSpeed
		this.Camera.Transform.Translation.Y = this.Camera.Transform.Translation.Y + this.Camera:GetLook().Y * cameraMovementSpeed
		this.Camera.Transform.Translation.Z = this.Camera.Transform.Translation.Z + this.Camera:GetLook().Z * cameraMovementSpeed
	end
	
	if Input:IsKeyPressed(65) or Input:GetAxis(0,2) < -treshold then --A
		this.Camera.Transform.Rotation.Y = this.Camera.Transform.Rotation.Y - cameraRotationSpeed
	end
	
	if Input:IsKeyPressed(68) or Input:GetAxis(0,2) > treshold then --D
		this.Camera.Transform.Rotation.Y = this.Camera.Transform.Rotation.Y + cameraRotationSpeed
	end
	
	if Input:IsKeyPressed(81) then --Q
		this.Camera.Transform.Translation.Y = this.Camera.Transform.Translation.Y - cameraMovementSpeed
	end
	
	if Input:IsKeyPressed(69) then --E
		this.Camera.Transform.Translation.Y = this.Camera.Transform.Translation.Y + cameraMovementSpeed
	end
end