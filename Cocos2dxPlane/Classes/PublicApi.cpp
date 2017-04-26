#include "PublicApi.h"
#include "AppMacros.h"
#include "Constant.h"
#include <cmath>


using namespace cocos2d;
//根据当前的血量计算百分百，从而计算出当前血条
//可以根据 动作特效进行切分血条  最后写！
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

	if(disX<0&&disY>0)				//目标在敌机右下
	{
		angle=90+angle;
	}
	else if(disX<0&&disY<0)			//目标在敌机右上
	{
		angle=90.0-angle;
	}
	else if(disX>0&&disY>0)			//目标在敌机左下
	{
		angle=-(90.0+angle);
	}
	else if(disX<0&&disY<0)			//目标在敌机左上
	{
		angle=-(90.0-angle);
	}
	else if(disX==0&&disY<0)		//目标在敌机正上方
	{
		angle=0;
	}
	else if(disX==0&&disY>0)		//目标在敌机正下方
	{
		angle=180.0;
	}
	else if(disX>0&&disY==0)		//目标在敌机正左方
	{
		angle=-90.0;
	}
	else if(disX<0&&disY==0)		//目标在敌机正下方
	{
		angle=90.0;
	}

	return angle;
}
/*
 * 2D之间的碰撞
 * 碰撞检测---判断两个物体是否进行了碰撞，采用简单的圆碰撞
 * 根据num的不同去常量里寻找不同的半径
 */
bool PublicApi::meetGoals(Point point1,Point point2,int num1,int num2)
{
	//*****************计算模型半径****************=r*scale
	int r1=Constant::radiusData[num1]*Constant::scaleData[num1];
	int r2=Constant::radiusData[num2]*Constant::scaleData[num2];
	//*****************判断两个目标点的距离************
	int dWidth=abs(point1.x-point2.x);
	int dHeight=abs(point1.y-point2.y);

	int distance=sqrt(dWidth*dWidth+dHeight*dHeight);
	int radius=r1+r2;
	if(distance>radius)
	{
		return false;			//没有碰撞
	}else{
		return true;			//发生碰撞
	}
}
//开启重力感应还是触控方式,1代表开启触控，0代表开启重力感应
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
	// 将vsh与fsh装配成一个完整的Shader文件。
	auto glprogram = GLProgram::createWithFilenames(vsh, fsh);
	// 由Shader文件创建这个Shader
	auto glprogramstate = GLProgramState::getOrCreateWithGLProgram(glprogram);
	// 给精灵设置所用的Shader
	sprite->setGLProgramState(glprogramstate);
	//创建海龟所用的贴图。
	auto textrue1 = Director::getInstance()->getTextureCache()->addImage(texture1);
	//将贴图设置给Shader中的变量值u_texture1
    glprogramstate->setUniformTexture("u_texture1", textrue1);
	//创建波光贴图。
	auto textrue2 = Director::getInstance()->getTextureCache()->addImage(texture2);
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
	Vec4  tLightColor(0,0.8,0.8,0.8);//rgba
    glprogramstate->setUniformVec4("v_LightColor",tLightColor);
	//下面这一段，是为了将我们自定义的Shader与我们的模型顶点组织方式进行匹配。模型的顶点数据一般包括位置，法线，色彩，纹理，以及骨骼绑定信息。而Shader需要将内部相应的顶点属性通道与模型相应的顶点属性数据进行绑定才能正确显示出顶点。
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
	// 将vsh与fsh装配成一个完整的Shader文件。
	auto glprogram = GLProgram::createWithFilenames(
			(SHADER_PATH+std::string("OutLine.vsh")).c_str(),
			(SHADER_PATH+std::string("OutLine.fsh")).c_str());
	// 由Shader文件创建这个Shader
	auto glprogramstate = GLProgramState::getOrCreateWithGLProgram(glprogram);
	// 给精灵设置所用的Shader
	sprite->setGLProgramState(glprogramstate);
	//将贴图设置给Shader中的变量值OutLineColor
    glprogramstate->setUniformVec3("OutLineColor", Vec3(1,1,1));
	//将贴图设置给Shader中的变量值OutlineWidth
	glprogramstate->setUniformFloat("OutlineWidth", 1.0);
	//下面这一段，是为了将我们自定义的Shader与我们的模型顶点组织方式进行匹配。模型的顶点数据一般包括位置，法线，色彩，纹理，以及骨骼绑定信息。而Shader需要将内部相应的顶点属性通道与模型相应的顶点属性数据进行绑定才能正确显示出顶点。
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
