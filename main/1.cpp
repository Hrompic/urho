#include <Urho3D/Engine/Application.h>
#include <Urho3D/Engine/Engine.h>
#include <Urho3D/Input/Input.h>

using namespace Urho3D;
class MyApp : public Application
{
public:
	explicit MyApp(Context *context);

};
URHO3D_DEFINE_APPLICATION_MAIN(MyApp)
MyApp::MyApp(Context *context)
	:Application(context)
{

}


