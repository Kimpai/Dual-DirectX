#include "GoodaDriver.h"

GoodaDriver::GoodaDriver()
{
	m_Direct3D = nullptr;
	m_Shader = nullptr;
}

GoodaDriver::GoodaDriver(const GoodaDriver& other)
{

}

GoodaDriver::~GoodaDriver()
{
	
}

void GoodaDriver::Initialize(HWND hwnd, Camera* camera)
{
	//Create the Direct3D object
	m_Direct3D = new Direct3D(SCREEN_HEIGHT, SCREEN_WIDTH, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	assert(m_Direct3D);

	//Create the Color Shader object
	m_Shader = new Shader();
	assert(&m_Shader);

	//Create the Model object
	m_Models.push_back(new Cube(XMFLOAT3(3.0f, 3.0f, 5.0f)));
	//m_Models.push_back(new Terrain("Resource Files/heightmap.bmp", 257, 257, 12.0f));
	assert(&m_Models);

	//Create the light object
	m_Lights.push_back(new DirectionalLight(XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f)));
	assert(&m_Lights);

	m_Shader->SetRootParameter(0, D3D12_ROOT_PARAMETER_TYPE_CBV, D3D12_SHADER_VISIBILITY_VERTEX);
	m_Shader->SetRootParameter(1, D3D12_ROOT_PARAMETER_TYPE_CBV, D3D12_SHADER_VISIBILITY_PIXEL);
	m_Shader->CreateRootSignature(m_Direct3D->GetDevice());
	m_Shader->CreatPipelineState({ { ShaderType::Type::VS, L"Shader Files/ColorVertexShader.hlsl" },{ ShaderType::Type::PS, L"Shader Files/ColorPixelShader.hlsl" } }, 
		m_Direct3D->GetDevice(), SCREEN_WIDTH, SCREEN_HEIGHT, ShaderPipelineType::COLOR);

	//Initialize the model object
	for (auto model : m_Models)
		model->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetCommandList(), camera->GetViewMatrix(), XMFLOAT4(0.0f, 0.0f, 0.5f, 1.0f));

	//Initialize the light object
	for (auto light : m_Lights)
		light->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetCommandList());

	//Close the command list now that all the commands have been recorded
	m_Direct3D->CloseCommandList();

	//Execute the command list
	m_Direct3D->ExecuteCommandList();
}

void GoodaDriver::Shutdown()
{
	//Release the Model object
	for (auto model : m_Models)
		if (model)
		{
			delete model;
			model = nullptr;
		}

	//Release the Color Shader object
	if (m_Shader)
	{
		delete m_Shader;
		m_Shader = nullptr;
	}

	//Release the Direct3D object
	if (m_Direct3D)
	{
		delete m_Direct3D;
		m_Direct3D = nullptr;
	}

	for (auto light : m_Lights)
		if (light)
		{
			delete light;
			light = nullptr;
		}
}

void GoodaDriver::Frame(Camera* camera)
{
	//Update constant buffer
	m_Shader->Frame(m_Direct3D->GetCurrentFrame());

	for (auto model : m_Models)
		model->Frame(m_Direct3D->GetCurrentFrame(), camera->GetViewMatrix());

	for (auto light : m_Lights)
		light->Frame(m_Direct3D->GetCurrentFrame());

	//Render the graphics scene
	Render();
}

void GoodaDriver::Render()
{
	//Reset the command list and put it in a recording state
	m_Direct3D->BeginScene(m_Shader);

	for (auto light : m_Lights)
		light->Render(m_Direct3D->GetCommandList(), m_Direct3D->GetCurrentFrame());

	for (auto model : m_Models)
		model->Render(m_Direct3D->GetCommandList(), m_Direct3D->GetCurrentFrame());

	//Close the command list and execute the commands
	m_Direct3D->EndScene();
}
