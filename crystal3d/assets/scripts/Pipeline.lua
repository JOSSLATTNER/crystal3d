function Init()

--Lighting Pass
lpc = RenderPassContext.new()
lpc.VertexShader = "Shader\\Base_Deferred.vert"
lpc.FragmentShader = "Shader\\Lighting.frag"
lpc.ViewportWidth = VIEWPORT_WIDTH
lpc.ViewportHeight = VIEWPORT_HEIGHT
lpc.BufferFlags = bit32.bor(BUFFER_GEOMETRY, BUFFER_POINT_LIGHT, BUFFER_DIRECTIONAL_LIGHT, BUFFER_LIGHT_INFO)

CreateRenderPass(RenderPass.new(lpc))

--SSAO Pass
aoc = SSAOPassContext.new()
aoc.VertexShader = "Shader\\Base_Deferred.vert"
aoc.FragmentShader = "Shader\\SSAO.frag"
aoc.ViewportWidth = VIEWPORT_WIDTH
aoc.ViewportHeight = VIEWPORT_HEIGHT
aoc.BufferFlags = bit32.bor(BUFFER_GEOMETRY, BUFFER_PREVIOUS_PASS)

CreateRenderPass(SSAOPass.new(aoc))

--Skybox Pass
spc = SkyboxPassContext.new()
spc.VertexShader = "Shader\\Base_Deferred.vert"
spc.FragmentShader = "Shader\\Skybox.frag"
spc.ViewportWidth = VIEWPORT_WIDTH
spc.ViewportHeight = VIEWPORT_HEIGHT
spc.BufferFlags = bit32.bor(BUFFER_GEOMETRY, BUFFER_PREVIOUS_PASS, BUFFER_FLAG_UTILITY)

spc.Skybox = Skybox.new()
spc.Skybox.Cubemap = "Textures\\clouds-a.jpg"

CreateRenderPass(SkyboxPass.new(spc))

end