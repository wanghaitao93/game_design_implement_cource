#include "PublicApi.h"
#include "AppMacros.h"
#include "Constant.h"
#include <cmath>


using namespace cocos2d;
//���ݵ�ǰ��Ѫ������ٷְ٣��Ӷ��������ǰѪ��
//���Ը��� ������Ч�����з�Ѫ��  ���д��
float PublicApi::hpPercent(float hp){
    float percent = 100.0f;
    if(hp>=100){
        percent = 100.f;
    }else if(83<=hp && hp<=99){
        percent = 100.0/7*6;
    }else if(65<=hp && hp<=82){
        percent = 100.0/7*5;
    }else if(47<=hp && hp<=64){
        percent = 100.0/7*4;
    }else if(29<=hp && hp<=46){
        percent = 100.0/7*3;
    }else if(11<=hp && hp<=28){
        percent = 100.0/7*2;
    }else if(1<=hp && hp<=10){
        percent = 100.0/7*1;
    }else if(hp<=0){
        percent = 0;
    }
    return percent;
}
PublicApi::PublicApi()
{
}
float PublicApi::turnAngle(Point start,Point goal)
{
	float angle=0.0f;

	float disX=start.x-goal.x;
	float disY=start.y-goal.y;
	angle=atan(fabs(disY)/fabs(disX))/PI*180;//(float)CC_RADIANS_TO_DEGREES

	if(disX<0&&disY>0)				//Ŀ���ڵл�����
	{
		angle=90+angle;
	}
	else if(disX<0&&disY<0)			//Ŀ���ڵл�����
	{
		angle=90.0-angle;
	}
	else if(disX>0&&disY>0)			//Ŀ���ڵл�����
	{
		angle=-(90.0+angle);
	}
	else if(disX<0&&disY<0)			//Ŀ���ڵл�����
	{
		angle=-(90.0-angle);
	}
	else if(disX==0&&disY<0)		//Ŀ���ڵл����Ϸ�
	{
		angle=0;
	}
	else if(disX==0&&disY>0)		//Ŀ���ڵл����·�
	{
		angle=180.0;
	}
	else if(disX>0&&disY==0)		//Ŀ���ڵл�����
	{
		angle=-90.0;
	}
	else if(disX<0&&disY==0)		//Ŀ���ڵл����·�
	{
		angle=90.0;
	}

	return angle;
}
/*
 * 2D֮�����ײ
 * ��ײ���---�ж����������Ƿ��������ײ�����ü򵥵�Բ��ײ
 * ����num�Ĳ�ͬȥ������Ѱ�Ҳ�ͬ�İ뾶
 */
bool PublicApi::meetGoals(Point point1,Point point2,int num1,int num2)
{
	//*****************����ģ�Ͱ뾶****************=r*scale
	int r1=Constant::radiusData[num1]*Constant::scaleData[num1];
	int r2=Constant::radiusData[num2]*Constant::scaleData[num2];
	//*****************�ж�����Ŀ���ľ���************
	int dWidth=abs(point1.x-point2.x);
	int dHeight=abs(point1.y-point2.y);

	int distance=sqrt(dWidth*dWidth+dHeight*dHeight);
	int radius=r1+r2;
	if(distance>radius)
	{
		return false;			//û����ײ
	}else{
		return true;			//������ײ
	}
}
//����������Ӧ���Ǵ��ط�ʽ,1���������أ�0������������Ӧ
int PublicApi::setWhichMethods()
{
	if(Constant::touchFlag)
	{
		return 1;
	}else{
		return 0;
	}
}

void PublicApi::init3DShader(Sprite3D *sprite,const char *texture,const char *vsh,const char *fsh)
{
	auto glprogram = GLProgram::createWithFilenames(vsh, fsh);
	auto glprogramstate = GLProgramState::getOrCreateWithGLProgram(glprogram);
	sprite->setGLProgramState(glprogramstate);
	auto textrue = Director::getInstance()->getTextureCache()->addImage(texture);
    glprogramstate->setUniformTexture("u_texture", textrue);
	long offset = 0;
	auto attributeCount = sprite->getMesh()->getMeshVertexAttribCount();
    for (auto k = 0; k < attributeCount; k++) {
        auto meshattribute = sprite->getMesh()->getMeshVertexAttribute(k);
        glprogramstate->setVertexAttribPointer(s_attributeNames[meshattribute.vertexAttrib],
                                             meshattribute.size,
                                             meshattribute.type,
                                             GL_FALSE,
                                             sprite->getMesh()->getVertexSizeInBytes(),
                                             (GLvoid*)offset);
        offset += meshattribute.attribSizeBytes;
    }
}

void PublicApi::init3DTexShader(Sprite3D *sprite,const char *texture1,
		const char *texture2,const char *vsh,const char *fsh)
{
	// ��vsh��fshװ���һ��������Shader�ļ���
	auto glprogram = GLProgram::createWithFilenames(vsh, fsh);
	// ��Shader�ļ��������Shader
	auto glprogramstate = GLProgramState::getOrCreateWithGLProgram(glprogram);
	// �������������õ�Shader
	sprite->setGLProgramState(glprogramstate);
	//�����������õ���ͼ��
	auto textrue1 = Director::getInstance()->getTextureCache()->addImage(texture1);
	//����ͼ���ø�Shader�еı���ֵu_texture1
    glprogramstate->setUniformTexture("u_texture1", textrue1);
	//����������ͼ��
	auto textrue2 = Director::getInstance()->getTextureCache()->addImage(texture2);
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
	Vec4  tLightColor(0,0.8,0.8,0.8);//rgba
    glprogramstate->setUniformVec4("v_LightColor",tLightColor);
	//������һ�Σ���Ϊ�˽������Զ����Shader�����ǵ�ģ�Ͷ�����֯��ʽ����ƥ�䡣ģ�͵Ķ�������һ�����λ�ã����ߣ�ɫ�ʣ������Լ���������Ϣ����Shader��Ҫ���ڲ���Ӧ�Ķ�������ͨ����ģ����Ӧ�Ķ����������ݽ��а󶨲�����ȷ��ʾ�����㡣
	long offset = 0;
	auto attributeCount = sprite->getMesh()->getMeshVertexAttribCount();
    for (auto k = 0; k < attributeCount; k++) {
        auto meshattribute = sprite->getMesh()->getMeshVertexAttribute(k);
        glprogramstate->setVertexAttribPointer(s_attributeNames[meshattribute.vertexAttrib],
                                             meshattribute.size,
                                             meshattribute.type,
                                             GL_FALSE,
                                             sprite->getMesh()->getVertexSizeInBytes(),
                                             (GLvoid*)offset);
        offset += meshattribute.attribSizeBytes;
    }
}

void PublicApi::init3DOutLineShader(Sprite3D *sprite)
{
	// ��vsh��fshװ���һ��������Shader�ļ���
	auto glprogram = GLProgram::createWithFilenames(
			(SHADER_PATH+std::string("OutLine.vsh")).c_str(),
			(SHADER_PATH+std::string("OutLine.fsh")).c_str());
	// ��Shader�ļ��������Shader
	auto glprogramstate = GLProgramState::getOrCreateWithGLProgram(glprogram);
	// �������������õ�Shader
	sprite->setGLProgramState(glprogramstate);
	//����ͼ���ø�Shader�еı���ֵOutLineColor
    glprogramstate->setUniformVec3("OutLineColor", Vec3(1,1,1));
	//����ͼ���ø�Shader�еı���ֵOutlineWidth
	glprogramstate->setUniformFloat("OutlineWidth", 1.0);
	//������һ�Σ���Ϊ�˽������Զ����Shader�����ǵ�ģ�Ͷ�����֯��ʽ����ƥ�䡣ģ�͵Ķ�������һ�����λ�ã����ߣ�ɫ�ʣ������Լ���������Ϣ����Shader��Ҫ���ڲ���Ӧ�Ķ�������ͨ����ģ����Ӧ�Ķ����������ݽ��а󶨲�����ȷ��ʾ�����㡣
	long offset = 0;
	auto attributeCount = sprite->getMesh()->getMeshVertexAttribCount();
    for (auto k = 0; k < attributeCount; k++) {
        auto meshattribute = sprite->getMesh()->getMeshVertexAttribute(k);
        glprogramstate->setVertexAttribPointer(s_attributeNames[meshattribute.vertexAttrib],
                                             meshattribute.size,
                                             meshattribute.type,
                                             GL_FALSE,
                                             sprite->getMesh()->getVertexSizeInBytes(),
                                             (GLvoid*)offset);
        offset += meshattribute.attribSizeBytes;
    }
}
