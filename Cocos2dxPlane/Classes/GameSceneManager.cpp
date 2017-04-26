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

////ʵ��HelloWorldSceneManager���е�createScene����
//Scene* GameSceneManager::createScene()
//{
//    //����һ����������
//    auto scene = Scene::create();
//
//    //��ȡ�ɼ�����ߴ�
//    Size visibleSize = Director::getInstance()->getVisibleSize();
//    //���������
//    Camera* camera = Camera::createPerspective
//    (
//    	55, 										//������ӽ� 40-60֮���Ǻ���ֵ
//    	visibleSize.width/visibleSize.height, 		//�ӿڳ����
//    	1, 											//near
//    	1000										//far
//    );
//    camera->setCameraFlag(CameraFlag::USER1);		//�����������ű�־
//    camera->setPosition3D(Vec3(0,150,600));			//���������λ��
//    camera->lookAt(Vec3(0,0,0), Vec3(0,1,0));		//���������Ŀ����Լ�up����
//    scene->addChild(camera);						//���������ӵ�������
//
//    //����һ��3D��������
//    My3DLayer* layer3D = My3DLayer::create();
//    layer3D->camera=camera;
//    //��3D������ӵ�������
//    scene->addChild(layer3D);
//
//    //����һ��2D��������
//    My2DLayer* layer2D = My2DLayer::create();
//    //��2D������ӵ�������
//    scene->addChild(layer2D);
//
//    //���س���
//    return scene;
//}
Camera* GameSceneManager::getCamera()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();	//��ȡ�ɼ��ߴ�
	Camera* camera = Camera::createPerspective
	(
		40, 														//������ӽ� 40-60֮���Ǻ���ֵ
		visibleSize.width/visibleSize.height, 						//�ӿڳ����
		1, 															//near
		4000														//far
	);
	camera->setCameraFlag(CameraFlag::USER1);						//�����������ű�־
	camera->setPosition3D(Vec3(270,-440,1200));					//���������λ��
	camera->lookAt(Vec3(270,480,0), Vec3(0,0.2747,0.9615));			//���������Ŀ����Լ�up����
	return camera;
}
void GameSceneManager::initShadersAnim(Sprite3D *plane,std::string texture)
{
	//....------------------------------------------------------------------
	// ��vsh��fshװ���һ��������Shader�ļ���
	auto glprogram = GLProgram::createWithFilenames("shaders/UVAnimation.vsh", "shaders/UVAnimation.fsh");
	// ��Shader�ļ��������Shader
	auto glprogramstate = GLProgramState::getOrCreateWithGLProgram(glprogram);
	// �������������õ�Shader
	plane->setGLProgramState(glprogramstate);

	//�����������õ���ͼ��
	auto textrue1 = Director::getInstance()->getTextureCache()->addImage(C3B_PATH+texture);
	//����ͼ���ø�Shader�еı���ֵu_texture1
	glprogramstate->setUniformTexture("u_texture1", textrue1);

	//����������ͼ��
	auto textrue2 = Director::getInstance()->getTextureCache()->addImage("c3b/caustics.png");
	//����ͼ���ø�Shader�еı���ֵu_lightTexture
	glprogramstate->setUniformTexture("u_lightTexture", textrue2);

	//ע�⣬���ڲ�����ͼ������ϣ�����ڽ���UV����ʱ�ܲ����ķ�����Ч��������������������UVѰַ��ʽΪGL_REPEAT��
	Texture2D::TexParams		tRepeatParams;
	tRepeatParams.magFilter = GL_LINEAR_MIPMAP_LINEAR;
	tRepeatParams.minFilter = GL_LINEAR;
	tRepeatParams.wrapS = GL_REPEAT;
	tRepeatParams.wrapT = GL_REPEAT;
	textrue2->setTexParameters(tRepeatParams);

	//�������������һ���������ɫ����������Ϊ��ɫ��
	Vec4  tLightColor(1.0,1.0,1.0,1.0);
	glprogramstate->setUniformVec4("v_LightColor",tLightColor);

	//������һ�Σ���Ϊ�˽������Զ����Shader�����ǵ�ģ�Ͷ�����֯��ʽ����ƥ�䡣ģ�͵Ķ�������һ�����λ�ã����ߣ�ɫ�ʣ������Լ���������Ϣ����Shader��Ҫ���ڲ���Ӧ�Ķ�������ͨ����ģ����Ӧ�Ķ����������ݽ��а󶨲�����ȷ��ʾ�����㡣
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
    firstScene->addChild(camera);									//���������ӵ�������

    Web3DLayer* layer3D = Web3DLayer::create();
    layer3D->camera=camera;											//�滻���������
	firstScene->addChild(layer3D);


//	//����һ��2D��������
	Web2DLayer* layer2D = Web2DLayer::create();
	//��2D������ӵ�������
	layer2D->layer3D = layer3D;
	layer2D->gsm = this;
	firstScene->addChild(layer2D,1,0);

	//��shang ��xia ��Ч
//	auto ss = TransitionMoveInT::create(1.0, firstScene);
	Director::getInstance()->replaceScene(firstScene);
}

void GameSceneManager::goFirst()
{
	Director::getInstance()->setDepthTest(true);
	Scene* firstScene = Scene::create();

	Camera *camera=getCamera();
    firstScene->addChild(camera);									//���������ӵ�������

    First3DLayer* layer3D = First3DLayer::create();
    layer3D->camera=camera;											//�滻���������
	firstScene->addChild(layer3D);


	//����һ��2D��������
	My2DLayer* layer2D = My2DLayer::create();
	//��2D������ӵ�������
	layer2D->layer3D = layer3D;
	layer2D->gsm = this;
	firstScene->addChild(layer2D,1,0);

	//��shang ��xia ��Ч
//	auto ss = TransitionMoveInT::create(1.0, firstScene);
	Director::getInstance()->replaceScene(firstScene);
}


void GameSceneManager::goBegin()
{
	Scene* beginScene = Scene::create();
	BeginLayer* layer = BeginLayer::create();
	layer->gsm = this;
	beginScene->addChild(layer);
	//���뵭����Ч
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
	//˳ʱ����Ч
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
	//˳ʱ����Ч
	//auto ss = TransitionProgressRadialCW::create(0.6, weaponScene);
	Director::getInstance()->replaceScene(weaponScene);
}

// ѡ�ؽ���
void GameSceneManager::goSelectGame()
{
	Director::getInstance()->setDepthTest(true);
	Scene* selectScene = Scene::create();
	SelectGameLayer* layer = SelectGameLayer::create();
	selectScene->addChild(layer);
	//����������Ч
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

	//����������Ч
//	auto ss = TransitionProgressInOut::create(1.0, planeScene);
	Director::getInstance()->replaceScene(planeScene);
}
void GameSceneManager::goHelp()
{
	Director::getInstance()->setDepthTest(true);
	Scene* helpScene = Scene::create();
	HelpLayer* layer = HelpLayer::create();
	helpScene->addChild(layer);
	//����������Ч
//	auto ss = TransitionProgressInOut::create(1.0, helpScene);
	Director::getInstance()->replaceScene(helpScene);
}
//���ؽ���
void GameSceneManager::goLoading()
{
	Director::getInstance()->setDepthTest(true);
	Scene* loadScene = Scene::create();
	LoadingLayer* layer = LoadingLayer::create();
	loadScene->addChild(layer);
	//����������Ч
//	auto ss = TransitionProgressInOut::create(0.5, loadScene);
	Director::getInstance()->replaceScene(loadScene);
}
