/*
 * SpriteAnimation.cpp
 *
 *  Created on: Jul 3, 2025
 *      Author: sadettin
 */

#include "animation/SpriteAnimation.h"

SpriteAnimation::SpriteAnimation(const AnimationFrames* animationFrames, int fps) {
	if(animationFrames == nullptr || animationFrames->frames.empty()) {
		gLoge("SpriteAnimation::SpriteAnimation") << "Animated frames are empty!";
		return;
	}
	static int sid;
	this->animatedframes = animationFrames;
	this->id = sid++;
	this->frameduration = 1.0f / static_cast<float>(fps);
	this->timer = 0.0f;
	this->currentframe = 0;
}

SpriteAnimation::~SpriteAnimation() {
	for(gImage* frame : animatedframes->frames) {
		delete frame;
	}
}

void SpriteAnimation::update(float deltaTime) {
	static int i;
	timer += deltaTime;
	while(timer >= frameduration) {
		currentframe = (currentframe + 1) % animatedframes->frames.size();
		timer -= frameduration;
	}
}

void SpriteAnimation::setFps(int fps) {
	this->frameduration = 1.0f / static_cast<float>(fps);
}

int SpriteAnimation::getId() const {
	return (id);
}

gImage* SpriteAnimation::getCurrentFrame() const {
	if(animatedframes->frames.empty()) {
		return nullptr;
	}
	return animatedframes->frames[currentframe % animatedframes->frames.size()];
}
