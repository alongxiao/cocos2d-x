/****************************************************************************
Copyright (c) 2013 cocos2d-x.org

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/

#ifndef __CCARMATUREDATAMANAGER_H__
#define __CCARMATUREDATAMANAGER_H__

#include "CCArmatureDefine.h"
#include "../datas/CCDatas.h"


NS_CC_EXT_ARMATURE_BEGIN

/**
 *	@brief	format and manage armature configuration and armature animation
 */
class  ArmatureDataManager : public Object
{
public:
	/** @deprecated Use getInstance() instead */
    CC_DEPRECATED_ATTRIBUTE static ArmatureDataManager *sharedArmatureDataManager() { return ArmatureDataManager::getInstance(); }

    static ArmatureDataManager *getInstance();

    static void purge();
private:
	/**
     * @js ctor
     */
    ArmatureDataManager(void);
    /**
     * @js NA
     * @lua NA
     */
    ~ArmatureDataManager(void);

public:

    /**
     * Init ArmatureDataManager
     */
    virtual bool init();

    /**
     * Add armature data
     * @param id The id of the armature data
     * @param armatureData ArmatureData *
     */
    void addArmatureData(const char *id, ArmatureData *armatureData);

    /**
     *	@brief	get armature data
     *	@param	id the id of the armature data you want to get
     *  @return	ArmatureData *
     */
    ArmatureData *getArmatureData(const char *id);

    /**
     *	@brief	remove armature data
     *	@param	id the id of the armature data you want to get
     */
    void removeArmatureData(const char *id);

    /**
     *	@brief	add animation data
     *	@param 	id the id of the animation data
     *  @return AnimationData *
     */
    void addAnimationData(const char *id, AnimationData *animationData);

    /**
     *	@brief	get animation data from _animationDatas(Dictionary)
     *	@param 	id the id of the animation data you want to get
     *  @return AnimationData *
     */
    AnimationData *getAnimationData(const char *id);

    /**
     *	@brief	remove animation data
     *	@param 	id the id of the animation data
     */
    void removeAnimationData(const char *id);

    /**
     *	@brief	add texture data
     *	@param 	id the id of the texture data
     *  @return TextureData *
     */
    void addTextureData(const char *id, TextureData *textureData);

    /**
     *	@brief	get texture data
     *	@param 	id the id of the texture data you want to get
     *  @return TextureData *
     */
    TextureData *getTextureData(const char *id);

    /**
     *	@brief	remove texture data
     *	@param 	id the id of the texture data you want to get
     */
    void removeTextureData(const char *id);

    /**
     *	@brief	Add ArmatureFileInfo, it is managed by ArmatureDataManager.
     */
    void addArmatureFileInfo(const char *configFilePath);

    /**
     *	@brief	Add ArmatureFileInfo, it is managed by ArmatureDataManager.
     *			It will load data in a new thread
     */
    void addArmatureFileInfoAsync(const char *configFilePath, Object *target, SEL_SCHEDULE selector);

    /**
     *	@brief	Add ArmatureFileInfo, it is managed by ArmatureDataManager.
     */
    void addArmatureFileInfo(const char *imagePath, const char *plistPath, const char *configFilePath);

    /**
     *	@brief	Add ArmatureFileInfo, it is managed by ArmatureDataManager.
     *			It will load data in a new thread
     */
    void addArmatureFileInfoAsync(const char *imagePath, const char *plistPath, const char *configFilePath, Object *target, SEL_SCHEDULE selector);

    /**
     *	@brief	Add sprite frame to CCSpriteFrameCache, it will save display name and it's relative image name
     */
    void addSpriteFrameFromFile(const char *plistPath, const char *imagePath);


    /**
     *	@brief	Clear the data in the _armarureDatas and _animationDatas, and set _armarureDatas and _animationDatas to NULL
     */
    void removeAll();

    /**
     *	@brief	Juge whether or not need auto load sprite file
     */
    bool isAutoLoadSpriteFile();


    Dictionary *getArmatureDatas() const;
    Dictionary *getAnimationDatas() const;
    Dictionary *getTextureDatas() const;
private:
    /**
     *	@brief	save amature datas
     *  @key	std::string
     *  @value	ArmatureData *
     */
    Dictionary *_armarureDatas;

    /**
     *	@brief	save animation datas
     *  @key	std::string
     *  @value	AnimationData *
     */
    Dictionary *_animationDatas;

    /**
     *	@brief	save texture datas
     *  @key	std::string
     *  @value	TextureData *
     */
    Dictionary *_textureDatas;

    bool _autoLoadSpriteFile;
};


NS_CC_EXT_ARMATURE_END

#endif/*__CCARMATUREDATAMANAGER_H__*/
