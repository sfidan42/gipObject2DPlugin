/*
 * SpriteAnimation.h
 *
 *  Created on: Jul 3, 2025
 *      Author: sadettin
 */

#ifndef SRC_SPRITEANIMATION_H_
#define SRC_SPRITEANIMATION_H_
#include "gImage.h"
#include "../../../../glistplugins/gipObject2DPlugin/src/datatypes/AnimationFrames.h"

class SpriteAnimation {
public:
	SpriteAnimation(const AnimationFrames* animationFrames, int fps = 6);
	~SpriteAnimation();
	void update(float deltaTime);
	void setFps(int fps);
	int getId() const;
	gImage* getCurrentFrame() const;

private:
	int id;
	float frameduration;
	float timer;
	size_t currentframe;
	const AnimationFrames* animatedframes;
};

#endif /* SRC_SPRITEANIMATION_H_ */
