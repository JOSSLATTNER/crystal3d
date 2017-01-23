--GLOBALS
total = 0

function Init()
	--Camera
	cameraTransform = Transform.new(Vector3.new(0,50,50), Vector3.new(0,180,0), Vector3.new(0,0,0))
	this.Camera = CreateCameraNode(cameraTransform)
	this.Camera.Frustum.Width = 1920
	this.Camera.Frustum.Height = 1080
	this.Camera.Frustum.FOV = dVector3.new(45.0,45.0,0.0)
	
	--Player
	model1 = Resources:LoadModel("Meshes\\gunslinger.obj", nil)
	playerNode = CreateSceneNode(Transform.new(Vector3.new(150,120,150), Vector3.new(0,0,0), Vector3.new(3,3,3)))

	--Player Script
	sc01 = CreateScriptComponent()
	playerScript = Resources:LoadScript("Scripts\\Test.lua", nil)
	sc01:SetScript(playerScript)
	playerNode:AddComponent(sc01)
	this:AddNode(playerNode)


	--print(#model1.Meshes.." Meshes and "..#model1.Materials.." Materials.")

	for i=1,#model1.Meshes do
		local meshNode = CreateMeshNode(Transform.new())

		local mesh = model1.Meshes[i]
		local material = model1.Materials[mesh.MaterialIndex + 1] --lua index is 1 based

		meshNode:SetMesh(mesh)
		meshNode:SetMaterial(material)
		meshNode.Transform:SetParent(playerNode.Transform)
	
		this:AddNode(meshNode)
	end
	
	--waterPlane = Primitives.Make_Plane(10,10)
	--meshNode02 = MeshNode.new(Transform.new(Vector3.new(0,0,0), Vector3.new(0,0,0), Vector3.new(50,1,50)))
	--meshNode02:SetRenderMode(1)
	--meshNode02:SetMesh(waterPlane)
	--
	--waterMat = Material.new("Shader\\base.vert", "Shader\\baseWater.frag")
	--waterMat.Textures["tDiffuse"] = "Textures\\uvcheck.png"
	--meshNode02:SetMaterial(waterMat)
	--
	--this:AddNode(meshNode02)
	
	--Terrain
	terrainNode01 = CreateTerrainNode(Transform.new(Vector3.new(-200,0,-200), Vector3.new(0,0,0), Vector3.new(5,1,5)))
	
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
	
	--Lights
	dirLight = DirectionalLight.new(Vector3.new(0,-1,0))
	dirLight.Specular = 0.0
	dirLight.Ambient = 0.8
	this:SetDirectionalLight(dirLight)
	
	pointLight1 = PointLight.new(Vector3.new(0,120,0))
	pointLight1.Radius = 50
	pointLight1.Diffuse = Vector3.new(1,0,0)
	this:AddPointLight(pointLight1)
end

function Update(delta)
	total = total + 0.1
	
	pointLight1.Position.X = math.sin(total)*80
	pointLight1.Position.Z = math.cos(total)*80

	--this.Camera.Transform.Rotation.Y = playerNode.Transform.Rotation.Y - 180
	--this.Camera.Transform.Rotation.X = 0
	--this.Camera.Transform.Translation.X = playerNode.Transform.Translation.X
	--this.Camera.Transform.Translation.Y = playerNode.Transform.Translation.Y + 30
	--this.Camera.Transform.Translation.Z = playerNode.Transform.Translation.Z - 250

	cameraMovementSpeed = delta * 50
	cameraRotationSpeed = delta * 100
	treshold = 0.2

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