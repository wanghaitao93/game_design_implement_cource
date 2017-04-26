#include "GameSceneManager.h"
//#include "My3DLayer.h"
//#include "My2DLayer.h"
#include "First3DLayer.h"
#include "My2DLayer.h"
#include "Web3DLayer.h"
#include "Web2DLayer.h"
#include "BeginLayer.h"
#include "HelpLayer.h"
#include "MainMenuScene.h"
//#include "OpenLayer.h"
#include "SelectGameLayer.h"
#include "SelectPlaneLayer.h"
#include "WeaponLayer.h"
#include "LoadingLayer.h"
#include "NetLayer.h"

//USING_NS_CC;
using namespace cocos2d;

////实现HelloWorldSceneManager类中的createScene方法
//Scene* GameSceneManager::createScene()
//{
//    //创建一个场景对象
//    auto scene = Scene::create();
//
//    //获取可见区域尺寸
//    Size visibleSize = Director::getInstance()->getVisibleSize();
//    //创建摄像机
//    Camera* camera = Camera::createPerspective
//    (
//    	55, 										//摄像机视角 40-60之间是合理值
//    	visibleSize.width/visibleSize.height, 		//视口长宽比
//    	1, 											//near
//    	1000										//far
//    );
//    camera->setCameraFlag(CameraFlag::USER1);		//设置摄像机编号标志
//    camera->setPosition3D(Vec3(0,150,600));			//设置摄像机位置
//    camera->lookAt(Vec3(0,0,0), Vec3(0,1,0));		//设置摄像机目标点以及up向量
//    scene->addChild(camera);						//将摄像机添加到场景中
//
//    //创建一个3D布景对象
//    My3DLayer* layer3D = My3DLayer::create();
//    layer3D->camera=camera;
//    //将3D布景添加到场景中
//    scene->addChild(layer3D);
//
//    //创建一个2D布景对象
//    My2DLayer* layer2D = My2DLayer::create();
//    //将2D布景添加到场景中
//    scene->addChild(layer2D);
//
//    //返回场景
//    return scene;
//}
Camera* GameSceneManager::getCamera()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();	//获取可见尺寸
	Camera* camera = Camera::createPerspective
	(
		40, 														//摄像机视角 40-60之间是合理值
		visibleSize.width/visibleSize.height, 						//视口长宽比
		1, 															//near
		4000														//far
	);
	camera->setCameraFlag(CameraFlag::USER1);						//设置摄像机编号标志
	camera->setPosition3D(Vec3(270,-440,1200));					//设置摄像机位置
	camera->lookAt(Vec3(270,480,0), Vec3(0,0.2747,0.9615));			//设置摄像机目标点以及up向量
	return camera;
}
void GameSceneManager::initShadersAnim(Sprite3D *plane,std::string texture)
{
	//....------------------------------------------------------------------
	// 将vsh与fsh装配成一个完整的Shader文件。
	auto glprogram = GLProgram::createWithFilenames("shaders/UVAnimation.vsh", "shaders/UVAnimation.fsh");
	// 由Shader文件创建这个Shader
	auto glprogramstate = GLProgramState::getOrCreateWithGLProgram(glprogram);
	// 给精灵设置所用的Shader
	plane->setGLProgramState(glprogramstate);

	//创建海龟所用的贴图。
	auto textrue1 = Director::getInstance()->getTextureCache()->addImage(C3B_PATH+texture);
	//将贴图设置给Shader中的变量值u_texture1
	glprogramstate->setUniformTexture("u_texture1", textrue1);

	//创建波光贴图。
	auto textrue2 = Director::getInstance()->getTextureCache()->addImage("c3b/caustics.png");
	//将贴图设置给Shader中的变量值u_lightTexture
	glprogramstate->setUniformTexture("u_lightTexture", textrue2);

	//注意，对于波光贴图，我们希望它在进行UV动画时能产生四方连续效果，必须设置它的纹理UV寻址方式为GL_REPEAT。
	Texture2D::TexParams		tRepeatParams;
	tRepeatParams.magFilter = GL_LINEAR_MIPMAP_LINEAR;
	tRepeatParams.minFilter = GL_LINEAR;
	tRepeatParams.wrapS = GL_REPEAT;
	tRepeatParams.wrapT = GL_REPEAT;
	textrue2->setTexParameters(tRepeatParams);

	//在这里，我们设置一个波光的颜色，这里设置为白色。
	Vec4  tLightColor(1.0,1.0,1.0,1.0);
	glprogramstate->setUniformVec4("v_LightColor",tLightColor);

	//下面这一段，是为了将我们自定义的Shader与我们的模型顶点组织方式进行匹配。模型的顶点数据一般包括位置，法线，色彩，纹理，以及骨骼绑定信息。而Shader需要将内部相应的顶点属性通道与模型相应的顶点属性数据进行绑定才能正确显示出顶点。
	long offset = 0;
	auto attributeCount = plane->getMesh()->getMeshVertexAttribCount();
	for (auto k = 0; k < attributeCount; k++) {
		auto meshattribute = plane->getMesh()->getMeshVertexAttribute(k);
		glprogramstate->setVertexAttribPointer(s_attributeNames[meshattribute.vertexAttrib],
											 meshattribute.size,
											 meshattribute.type,
											 GL_FALSE,
											 plane->getMesh()->getVertexSizeInBytes(),
											 (GLvoid*)offset);
		offset += meshattribute.attribSizeBytes;
	}
}

void GameSceneManager::goWeb()
{
	Director::getInstance()->setDepthTest(true);
	Scene* firstScene = Scene::create();

	Camera *camera=getCamera();
    firstScene->addChild(camera);									//将摄像机添加到场景中

    Web3DLayer* layer3D = Web3DLayer::create();
    layer3D->camera=camera;											//替换场景摄像机
	firstScene->addChild(layer3D);


//	//创建一个2D布景对象
	Web2DLayer* layer2D = Web2DLayer::create();
	//将2D布景添加到场景中
	layer2D->layer3D = layer3D;
	layer2D->gsm = this;
	firstScene->addChild(layer2D,1,0);

	//由shang 倒xia 特效
//	auto ss = TransitionMoveInT::create(1.0, firstScene);
	Director::getInstance()->replaceScene(firstScene);
}

void GameSceneManager::goFirst()
{
	Director::getInstance()->setDepthTest(true);
	Scene* firstScene = Scene::create();

	Camera *camera=getCamera();
    firstScene->addChild(camera);									//将摄像机添加到场景中

    First3DLayer* layer3D = First3DLayer::create();
    layer3D->camera=camera;											//替换场景摄像机
	firstScene->addChild(layer3D);


	//创建一个2D布景对象
	My2DLayer* layer2D = My2DLayer::create();
	//将2D布景添加到场景中
	layer2D->layer3D = layer3D;
	layer2D->gsm = this;
	firstScene->addChild(layer2D,1,0);

	//由shang 倒xia 特效
//	auto ss = TransitionMoveInT::create(1.0, firstScene);
	Director::getInstance()->replaceScene(firstScene);
}


void GameSceneManager::goBegin()
{
	Scene* beginScene = Scene::create();
	BeginLayer* layer = BeginLayer::create();
	layer->gsm = this;
	beginScene->addChild(layer);
	//淡入淡出特效
//	auto ss = TransitionFade::create(0.5, beginScene);
	Director::getInstance()->replaceScene(beginScene);
}
//void GameSceneManager::createScene()
//{
//	openScene = Scene::create();
//	OpenLayer* layer = OpenLayer::create();
//	layer->gsm = this;
//	openScene->addChild(layer);
//}

void GameSceneManager::goMainMenu()
{
	Director::getInstance()->setDepthTest(true);
	Scene *mainScene = Scene::create();
	MainMenuScene* layer = MainMenuScene::create();
	mainScene->addChild(layer);
	//顺时针特效
//	auto ss = TransitionProgressRadialCW::create(0.6, mainScene);
	Director::getInstance()->replaceScene(mainScene);
}

void GameSceneManager::goWeapon()
{
	Director::getInstance()->setDepthTest(true);
	Scene *weaponScene = Scene::create();
	WeaponLayer* layer = WeaponLayer::create();
	layer->gsm = this;
	weaponScene->addChild(layer);
	//顺时针特效
	//auto ss = TransitionProgressRadialCW::create(0.6, weaponScene);
	Director::getInstance()->replaceScene(weaponScene);
}

// 选关界面
void GameSceneManager::goSelectGame()
{
	Director::getInstance()->setDepthTest(true);
	Scene* selectScene = Scene::create();
	SelectGameLayer* layer = SelectGameLayer::create();
	selectScene->addChild(layer);
	//由内向外特效
//	auto ss = TransitionProgressInOut::create(1.0, selectScene);
	Director::getInstance()->replaceScene(selectScene);
}
void GameSceneManager::goSelectPlane()
{
	Director::getInstance()->setDepthTest(true);

	Scene* load = Scene::create();
	auto loadScene = TransitionProgressInOut::create(1.0, load);
	Director::getInstance()->replaceScene(loadScene);
	Scene* planeScene = Scene::create();
	SelectPlane* layer = SelectPlane::create();
	planeScene->addChild(layer);

	//由内向外特效
//	auto ss = TransitionProgressInOut::create(1.0, planeScene);
	Director::getInstance()->replaceScene(planeScene);
}
void GameSceneManager::goHelp()
{
	Director::getInstance()->setDepthTest(true);
	Scene* helpScene = Scene::create();
	HelpLayer* layer = HelpLayer::create();
	helpScene->addChild(layer);
	//由内向外特效
//	auto ss = TransitionProgressInOut::create(1.0, helpScene);
	Director::getInstance()->replaceScene(helpScene);
}
//加载界面
void GameSceneManager::goLoading()
{
	Director::getInstance()->setDepthTest(true);
	Scene* loadScene = Scene::create();
	LoadingLayer* layer = LoadingLayer::create();
	loadScene->addChild(layer);
	//由内向外特效
//	auto ss = TransitionProgressInOut::create(0.5, loadScene);
	Director::getInstance()->replaceScene(loadScene);
}
