
#include "Precompiled.h"
#include <random>
using namespace CK::DD;

// 메시
const std::size_t GameEngine::QuadMesh = std::hash<std::string>()("SM_Quad");

// 게임 오브젝트
const std::string GameEngine::SunGo("Sun");
const std::string GameEngine::EarthGo("Earth");
const std::string GameEngine::MoonGo("Moon");

//
const std::string GameEngine::BodyGo("01Body");
const std::string GameEngine::ShoulderGo("02Shoulder");
const std::string GameEngine::ShoulderLGo("02ShoulderL");
const std::string GameEngine::ForeArmGo("03ForeArm");
const std::string GameEngine::ForeArmLGo("03ForeArmL");
const std::string GameEngine::HandGo("04Hand");
const std::string GameEngine::HandLGo("04HandL");
//

// 텍스쳐
const std::size_t GameEngine::DiffuseTexture = std::hash<std::string>()("Diffuse");
const std::string GameEngine::SteveTexturePath("Steve.png");

struct GameObjectCompare
{
	bool operator()(const std::unique_ptr<GameObject>& lhs, std::size_t rhs)
	{
		return lhs->GetHash() < rhs;
	}
};

void GameEngine::OnScreenResize(const ScreenPoint& InScreenSize)
{
	// 화면 크기의 설정
	_ScreenSize = InScreenSize;
	_MainCamera.SetViewportSize(_ScreenSize);
}

bool GameEngine::Init()
{
	// 이미 초기화되어 있으면 초기화 진행하지 않음.
	if (_IsInitialized)
	{
		return true;
	}

	// 화면 크기가 올바로 설정되어 있는지 확인
	if (_ScreenSize.HasZero())
	{
		return false;
	}

	if (!_InputManager.IsInputReady())
	{
		return false;
	}

	if (!LoadResources())
	{
		return false;
	}

	if (!LoadScene())
	{
		return false;
	}

	_IsInitialized = true;
	return true;
}

bool GameEngine::LoadResources()
{
	// 메시 데이터 로딩
	Mesh& quadMesh = CreateMesh(GameEngine::QuadMesh);

	constexpr float squareHalfSize = 0.5f;
	constexpr int vertexCount = 4;
	constexpr int triangleCount = 2;
	constexpr int indexCount = triangleCount * 3;

	auto& v = quadMesh.GetVertices();
	auto& i = quadMesh.GetIndices();
	auto& uv = quadMesh.GetUVs();

	v = {
		Vector2(-squareHalfSize, -squareHalfSize),
		Vector2(-squareHalfSize, squareHalfSize),
		Vector2(squareHalfSize, squareHalfSize),
		Vector2(squareHalfSize, -squareHalfSize)
	};

	uv = {
		Vector2(0.125f, 0.75f),
		Vector2(0.125f, 0.875f),
		Vector2(0.25f, 0.875f),
		Vector2(0.25f, 0.75f)
	};

	i = {
		0, 2, 1, 0, 3, 2
	};

	quadMesh.CalculateBounds();

	// 텍스쳐 로딩
	Texture& diffuseTexture = CreateTexture(GameEngine::DiffuseTexture, GameEngine::SteveTexturePath);
	if (!diffuseTexture.IsIntialized())
	{
		return false;
	}

	return true;
}

bool GameEngine::LoadScene()
{
	static const float sunScale = 30.f;
	static const float earthScale = 20.f;
	static const float moonScale = 10.f;
	static const Vector2 earthOffset(200.f, 0.0f);
	static const Vector2 moonOffset(250.f, 0.0f);

	// 태양계
	//GameObject& goSun = CreateNewGameObject(GameEngine::SunGo);
	//goSun.SetMesh(GameEngine::QuadMesh);
	//goSun.GetTransform().SetWorldScale(Vector2::One * sunScale);

	//GameObject& goEarth = CreateNewGameObject(GameEngine::EarthGo);
	//goEarth.SetMesh(GameEngine::QuadMesh);
	//goEarth.GetTransform().SetWorldPosition(earthOffset);
	//goEarth.GetTransform().SetWorldScale(Vector2::One * earthScale);
	//goEarth.SetParent(goSun);

	//GameObject& goMoon = CreateNewGameObject(GameEngine::MoonGo);
	//goMoon.SetMesh(GameEngine::QuadMesh);
	//goMoon.GetTransform().SetWorldPosition(moonOffset);
	//goMoon.GetTransform().SetWorldScale(Vector2::One * moonScale);
	//goMoon.SetParent(goEarth);

	static const Vector2 bodyScale = Vector2::UnitX * 200.0f + Vector2::UnitY * 10.0f;
	static const Vector2 shoulderScale = Vector2::UnitX * 10.0f + Vector2::UnitY * 100.0f;
	static const Vector2 forearmScale = Vector2::UnitX * 50.0f + Vector2::UnitY * 10.0f;
	static const Vector2 handScale = Vector2::UnitX * 10.0f + Vector2::UnitY * 20.0f;
	static const Vector2 bodyOffset(0.0f, 0.0f);
	static const Vector2 shoulderOffset(Vector2(100.0f, 0.0f));
	static const Vector2 forearmOffset(Vector2(shoulderOffset.X, shoulderOffset.Y+50.0f));
	static const Vector2 handOffset(Vector2(forearmOffset.X * 10.0f, forearmOffset.Y));

	// 팔
	GameObject& goBody = CreateNewGameObject(GameEngine::BodyGo);
	goBody.SetMesh(GameEngine::QuadMesh);
	goBody.GetTransform().SetWorldPosition(bodyOffset);
	goBody.GetTransform().SetWorldScale(bodyScale);

	GameObject& goShoulder = CreateNewGameObject(GameEngine::ShoulderGo);
	goShoulder.SetMesh(GameEngine::QuadMesh);
	goShoulder.GetTransform().SetWorldPosition(shoulderOffset);
	goShoulder.GetTransform().SetWorldScale(shoulderScale);
	goShoulder.SetParent(goBody);

	GameObject& goShoulderL = CreateNewGameObject(GameEngine::ShoulderLGo);
	goShoulderL.SetMesh(GameEngine::QuadMesh);
	goShoulderL.GetTransform().SetWorldPosition(-shoulderOffset);
	goShoulderL.GetTransform().SetWorldScale(shoulderScale);
	goShoulderL.SetParent(goBody);

	GameObject& goForearm = CreateNewGameObject(GameEngine::ForeArmGo);
	goForearm.SetMesh(GameEngine::QuadMesh);
	goForearm.GetTransform().SetWorldPosition(forearmOffset);
	goForearm.GetTransform().SetWorldScale(forearmScale);
	goForearm.SetParent(goShoulder);
	
	GameObject& goForearmL = CreateNewGameObject(GameEngine::ForeArmLGo);
	goForearmL.SetMesh(GameEngine::QuadMesh);
	goForearmL.GetTransform().SetWorldPosition(Vector2(forearmOffset.X * -1.0f, forearmOffset.Y));
	goForearmL.GetTransform().SetWorldScale(forearmScale);
	goForearmL.SetParent(goShoulderL);

	GameObject& goHand = CreateNewGameObject(GameEngine::HandGo);
	goHand.SetMesh(GameEngine::QuadMesh);
	goHand.GetTransform().SetWorldPosition(handOffset);
	goHand.GetTransform().SetWorldScale(handScale);
	goHand.SetParent(goForearm);

	GameObject& goHandL = CreateNewGameObject(GameEngine::HandLGo);
	goHandL.SetMesh(GameEngine::QuadMesh);
	goHandL.GetTransform().SetWorldPosition(Vector2(handOffset.X * -1.0f, handOffset.Y));
	goHandL.GetTransform().SetWorldScale(handScale);
	goHandL.SetParent(goForearmL);


	return true;
}


Mesh& GameEngine::CreateMesh(const std::size_t& InKey)
{
	auto meshPtr = std::make_unique<Mesh>();
	_Meshes.insert({ InKey, std::move(meshPtr) });
	return *_Meshes.at(InKey).get();
}

Texture& GameEngine::CreateTexture(const std::size_t& InKey, const std::string& InTexturePath)
{
	auto texturePtr = std::make_unique<Texture>(InTexturePath);
	_Textures.insert({ InKey, std::move(texturePtr) });
	return *_Textures.at(InKey).get();
}

GameObject& GameEngine::CreateNewGameObject(const std::string& InName)
{
	std::size_t inHash = std::hash<std::string>()(InName);
	const auto it = std::lower_bound(SceneBegin(), SceneEnd(), inHash, GameObjectCompare());

	auto newGameObject = std::make_unique<GameObject>(InName);
	if (it != _Scene.end())
	{
		std::size_t targetHash = (*it)->GetHash();
		if (targetHash == inHash)
		{
			// 중복된 키 발생. 무시.
			assert(false);
			return GameObject::Invalid;
		}
		else if (targetHash < inHash)
		{
			_Scene.insert(it + 1, std::move(newGameObject));
		}
		else
		{
			_Scene.insert(it, std::move(newGameObject));
		}
	}
	else
	{
		_Scene.push_back(std::move(newGameObject));
	}

	return GetGameObject(InName);
}

GameObject& GameEngine::GetGameObject(const std::string& InName)
{
	std::size_t targetHash = std::hash<std::string>()(InName);
	const auto it = std::lower_bound(SceneBegin(), SceneEnd(), targetHash, GameObjectCompare());

	return (it != _Scene.end()) ? *(*it).get() : GameObject::Invalid;
}
