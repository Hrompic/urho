#include <Urho3D/Core/CoreEvents.h>
#include <Urho3D/Engine/Application.h>
#include <Urho3D/Engine/Engine.h>
#include <Urho3D/Engine/EngineDefs.h>
#include <Urho3D/Input/Input.h>
#include <Urho3D/Input/InputEvents.h>
#include <Urho3D/UI/Text.h>
#include <Urho3D/UI/Font.h>
#include <Urho3D/UI/UI.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/Scene/SceneEvents.h>
#include <Urho3D/Graphics/Octree.h>
#include <Urho3D/Graphics/Camera.h>
#include <Urho3D/Graphics/Graphics.h>
#include <Urho3D/Graphics/Renderer.h>
#include <Urho3D/Graphics/DebugRenderer.h>
#include <Urho3D/Graphics/Material.h>
#include <Urho3D/Graphics/Model.h>
#include <Urho3D/Graphics/Skybox.h>
#include <Urho3D/Graphics/StaticModel.h>
#include <Urho3D/Graphics/Light.h>
#include <Urho3D/Physics/PhysicsWorld.h>
#include <Urho3D/Physics/RigidBody.h>
#include <Urho3D/Physics/CollisionShape.h>
#include <iostream>

#define MOUSESENS .2
#define BOXES 400

namespace Urho3D
{

class Node;
class Scene;
class Sprite;

}


using namespace Urho3D;
class MyApp : public Application
{
//	URHO3D_OBJECT(MyApp, Application)
public:
	explicit MyApp(Context *context);
	void Start() override;
	void Setup() override;

private:
	void update(StringHash evType, VariantMap &evData );
	void postUpdate(StringHash , VariantMap &);
	void kdHand(StringHash evType, VariantMap &evData );
	SharedPtr<Text> text;
	SharedPtr<Scene> scene;
	SharedPtr<Node> boxNode;
	SharedPtr<Node> cameraNode;
	SharedPtr<Node> playerNode;
	bool draw;
};
URHO3D_DEFINE_APPLICATION_MAIN(MyApp)

MyApp::MyApp(Context *context):
	Application(context)
{

}

void MyApp::Setup()
{
	engineParameters_[EP_FULL_SCREEN] = false;
}


void MyApp::Start()
{
	ResourceCache *cache = GetSubsystem<ResourceCache>();
	//Text
	text = new Text(context_);
	text->SetText("Test");
	text->SetFont(cache->GetResource<Font>("Fonts/Anonymous Pro.ttf", 20));
	text->SetSize(40, 40);
	text->SetColor(Color(.1, .7, .1));
	text->SetHorizontalAlignment(HA_CENTER);
	GetSubsystem<UI>()->GetRoot()->AddChild(text);
	//Scene
	scene = new Scene(context_);
	scene->CreateComponent<Octree>();
	scene->CreateComponent<DebugRenderer>();
	scene->CreateComponent<PhysicsWorld>();
	//Sky
	Node *skyNode = scene->CreateChild("Sky");
	skyNode->SetScale(50.);
	Skybox *skyBox= skyNode->CreateComponent<Skybox>();
	skyBox->SetModel(cache->GetResource<Model>("Models/Box.mdl"));
	skyBox->SetMaterial(cache->GetResource<Material>("Materials/Skybox.xml"));
	//Light
	Node *lightNode = scene->CreateChild("Light");
	lightNode->SetDirection(Vector3::FORWARD);
	lightNode->Yaw(50.);
	lightNode->Pitch(50.);
	Light *light = lightNode->CreateComponent<Light>();
	light->SetLightType(LIGHT_DIRECTIONAL);
	light->SetBrightness(1.1);
	light->SetColor(Color(1.0, .6, .5));
	light->SetCastShadows(true);
	//Ground
	Node *groundNode = scene->CreateChild("Ground");
	groundNode->SetPosition(Vector3(0, -.5, 0));
	groundNode->SetScale(Vector3(500., 2.0, 500.));

	StaticModel *ground = groundNode->CreateComponent<StaticModel>();
	ground->SetModel(cache->GetResource<Model>("Models/Box.mdl"));
	ground->SetMaterial(cache->GetResource<Material>("Materials/StoneTiled.xml"));
	RigidBody *body = groundNode->CreateComponent<RigidBody>();
	body->SetCollisionLayer(2);
	body->SetRestitution(.5);
	groundNode->CreateComponent<CollisionShape>()->SetBox(Vector3::ONE);
	//Box
	for(int i=0; i<BOXES; i++)
	{
		Node *boxNode = scene->CreateChild("Box");
		boxNode->SetPosition(Vector3(0, i*10, 10));
		boxNode->SetScale(3);
		StaticModel *box = boxNode->CreateComponent<StaticModel>();
		box->SetModel(cache->GetResource<Model>("Models/Box.mdl"));
		box->SetMaterial(cache->GetResource<Material>("Materials/StoneEnvMapSmall.xml"));
		box->SetCastShadows(true);
		//Physics
		RigidBody *body = boxNode->CreateComponent<RigidBody>();
		body->SetMass(1.);
		body->SetFriction(.75);
		body->SetRestitution(.5);
		body->SetCollisionLayer(2);
		boxNode->CreateComponent<CollisionShape>()->SetBox(Vector3::ONE);
		printf("Node id: %d\n", boxNode->GetID());
	}
	//Player
	playerNode = scene->CreateChild("Player");
	playerNode->SetPosition(Vector3(0, 10, 0));
	playerNode->SetScale(6);
	StaticModel *player = playerNode->CreateComponent<StaticModel>();
	player->SetModel(cache->GetResource<Model>("Models/Sphere.mdl"));
	player->SetMaterial(cache->GetResource<Material>("Materials/StoneTiled.xml"));
	player->SetShadowMask(true);
	body = playerNode->CreateComponent<RigidBody>();
	body->SetMass(1.);
	body->SetFriction(.75);
	body->SetCollisionLayer(2);
	playerNode->CreateComponent<CollisionShape>()->SetSphere(1);
	//Camera
	cameraNode = scene->CreateChild("Camera");

	Camera *camera = cameraNode->CreateComponent<Camera>();
	//camera->SetZoom(5);
	camera->SetFarClip(5000.);


	Renderer *render = GetSubsystem<Renderer>();
	SharedPtr<Viewport> viewport(new Viewport(context_, scene, cameraNode->GetComponent<Camera>()));
	render->SetViewport(0, viewport);

	cameraNode->SetPosition(Vector3(0, 5., 0));

	SubscribeToEvent(E_KEYDOWN, URHO3D_HANDLER(MyApp, kdHand));
	SubscribeToEvent(E_UPDATE, URHO3D_HANDLER(MyApp, update));
	SubscribeToEvent(E_POSTUPDATE, URHO3D_HANDLER(MyApp, postUpdate));

}

void MyApp::kdHand(StringHash /*evType*/, VariantMap &evData)
{
	using namespace KeyDown;
	int key = evData[P_KEY].GetInt();
	if(key==KEY_ESCAPE) engine_->Exit();
	if(key==KEY_0) draw = !draw;
	if(key==KEY_TAB) GetSubsystem<Input>()->SetMouseVisible(!GetSubsystem<Input>()->IsMouseVisible());
}

void MyApp::update(StringHash/* evType*/, VariantMap &evData)
{
	static float cleanTime=0, time=0;
	static int frameCount = 0;
	frameCount++;
	float timeStep = evData[Update::P_TIMESTEP].GetFloat();
	cleanTime+=timeStep;
	time+=timeStep;

			if(time>1.)
	{
		char tx[128];
		sprintf(tx, "Time:%f - Framerate: %.2fFPS\n"
					"Coord Player x:%.2f y:%.2f z:%.2f\n "
					"Speed Player x:%.2f y:%.2f z:%.2f\n",
				time, frameCount/time,
				playerNode->GetPosition().x_, playerNode->GetPosition().y_, playerNode->GetPosition().z_,
				playerNode->GetOrCreateComponent<RigidBody>()->GetLinearVelocity().x_,
				playerNode->GetOrCreateComponent<RigidBody>()->GetLinearVelocity().y_,
				playerNode->GetOrCreateComponent<RigidBody>()->GetLinearVelocity().z_);
		text->SetText(tx);
		frameCount = 0;
		time = 0;
	}
	float speed = 20.;

	Input *input = GetSubsystem<Input>();

	if(cleanTime>20.)
	{
		auto nodes = scene->GetChildren();
		//std::cout <<nodes[0]->GetID();

		for(auto iter=nodes.Begin(); iter!=nodes.End(); iter++)
		{
			if(iter->Get()->GetName()=="Box")
				if(iter->Get()->GetPosition().y_<-1.)
					iter->Get()->Remove();
		}
		cleanTime = 0;
	}
	if(!GetSubsystem<Input>()->IsMouseVisible())
	{
		static float yaw=0, pitch=0;
		IntVector2 mouseMove = input->GetMouseMove();
		yaw += MOUSESENS*mouseMove.x_;
		pitch += MOUSESENS*mouseMove.y_;
		pitch = Clamp(pitch, -90.f, 90.f);
		cameraNode->SetDirection(Vector3::FORWARD);
		cameraNode->Yaw(yaw);
		cameraNode->Pitch(pitch);
	}
	if(input->GetKeyDown(KEY_SHIFT)) speed*=10;
//	if(input->GetKeyDown(KEY_W)) cameraNode->Translate(Vector3::FORWARD*timeStep*speed);
//	if(input->GetKeyDown(KEY_S)) cameraNode->Translate(Vector3::BACK*timeStep*speed);
//	if(input->GetKeyDown(KEY_A)) cameraNode->Translate(Vector3::LEFT*timeStep*speed);
//	if(input->GetKeyDown(KEY_D)) cameraNode->Translate(Vector3::RIGHT*timeStep*speed);
	if(input->GetKeyDown(KEY_W)) playerNode->GetComponent<RigidBody>()->ApplyImpulse(cameraNode->GetRotation()*Vector3::FORWARD*0.8);//Vector3(0, 0, 5.));
	if(input->GetKeyDown(KEY_S)) playerNode->GetComponent<RigidBody>()->ApplyImpulse(cameraNode->GetRotation()*Vector3::BACK*0.8);
	if(input->GetKeyDown(KEY_A)) playerNode->GetComponent<RigidBody>()->ApplyImpulse(cameraNode->GetRotation()*Vector3::LEFT*0.8);
	if(input->GetKeyDown(KEY_D)) playerNode->GetComponent<RigidBody>()->ApplyImpulse(cameraNode->GetRotation()*Vector3::RIGHT*0.8);
	if(input->GetKeyDown(KEY_SPACE)) playerNode->GetComponent<RigidBody>()->ApplyImpulse(cameraNode->GetRotation()*Vector3::UP*0.8);
	if(input->GetKeyDown(KEY_LCTRL)) playerNode->GetComponent<RigidBody>()->ApplyImpulse(cameraNode->GetRotation()*Vector3::DOWN*0.8);




}

void MyApp::postUpdate(StringHash, VariantMap &)
{
	if(draw) scene->GetComponent<PhysicsWorld>()->DrawDebugGeometry(false);
	cameraNode->SetPosition(playerNode->GetPosition());//+Vector3(0, 10, 10.));
}



