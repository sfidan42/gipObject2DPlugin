/*
 * SpriteAnimator.h
 *
 *  Created on: Jul 3, 2025
 *      Author: sadettin
 */

#ifndef SRC_SPRITEANIMATOR_H_
#define SRC_SPRITEANIMATOR_H_
#include "animation/SpriteAnimation.h"
#include <vector>
#include <glm/glm.hpp>

class SpriteAnimator {
public:
	SpriteAnimator() = default;
	~SpriteAnimator() = default;
public:
	void addAnimation(SpriteAnimation *anim);
	void changeAnimation(int id);
	void changeCurrentFps(int fps);
	void update(float deltaTime);
	void draw(int x, int y, float angle = 0.0f) const;
	void draw(int x, int y, int w, int h, float angle = 0.0f) const;
	void draw(const glm::vec2& pos) const;
	void draw(const glm::vec2& pos, const glm::vec2& size, float angle = 0.0f) const;
	const gImage* getCurrentFrame() const;
private:
	std::vector<SpriteAnimation*> animations;
	SpriteAnimation* currentanim = nullptr;
};

#endif /* SRC_SPRITEANIMATOR_H_ */
