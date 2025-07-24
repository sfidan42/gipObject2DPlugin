/*
 * SpriteAnimator.cpp
 *
 *  Created on: Jul 3, 2025
 *      Author: sadettin
 */

#include "animation/SpriteAnimator.h"

void SpriteAnimator::addAnimation(SpriteAnimation* anim) {
	if(!anim) {
		return;
	}
	animations.push_back(anim);
	if(!currentanim) {
		currentanim = anim;
	}
}

void SpriteAnimator::changeAnimation(int id) {
	for(SpriteAnimation* anim : animations) {
		if(anim->getId() == id) {
			currentanim = anim;
			return;
		}
	}
	gLoge("SpriteAnimator::changeAnimation") << id << " not found";
}

void SpriteAnimator::changeCurrentFps(int fps) {
	if(!currentanim) {
		return;
	}
	int id = currentanim->getId();
	for(SpriteAnimation* anim : animations) {
		if(anim->getId() == id) {
			anim->setFps(fps);
			return;
		}
	}
	gLoge("SpriteAnimator::changeCurrentFps") << id << " not found";
}

void SpriteAnimator::update(float deltaTime) {
	if(!currentanim) {
		return;
	}
	currentanim->update(deltaTime);
}

void SpriteAnimator::draw(int x, int y, float angle) {
	if(!currentanim) {
		return;
	}
	gImage* currentframe = currentanim->getCurrentFrame();
	if(currentframe) {
		int w = currentframe->getWidth();
		int h = currentframe->getHeight();
		currentframe->draw(x, y, w, h, angle);
	}
}

void SpriteAnimator::draw(int x, int y, int w, int h, float angle) {
	if(!currentanim) {
		return;
	}
	gImage* currentframe = currentanim->getCurrentFrame();
	if(currentframe) {
		currentframe->draw(x, y, w, h, angle);
	}
}

void SpriteAnimator::draw(const glm::vec2& pos) {
	if(!currentanim) {
		return;
	}
	gImage* currentframe = currentanim->getCurrentFrame();
	if(currentframe) {
		currentframe->draw(pos.x, pos.y);
	}
}

void SpriteAnimator::draw(const glm::vec2& pos, const glm::vec2& size, float angle) {
	if(!currentanim) {
		return;
	}
	gImage* currentframe = currentanim->getCurrentFrame();
	if(currentframe) {
		currentframe->draw(pos.x, pos.y, size.x, size.y, angle);
	}
}

const gImage* SpriteAnimator::getCurrentFrame() const {
	return currentanim->getCurrentFrame();
}
