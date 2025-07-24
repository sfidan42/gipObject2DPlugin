#pragma once
#include "datatypes/Object2D.h"

template <Pos2D P, Tex2D TX>
template <Pos2D p, typename std::enable_if<p == Pos2D::FIXED, int>::type>
Object2D<Type2D::NODE, P, TX>::Object2D(typename Texture2DTraits<TX>::type* texture,
	const glm::vec2& _pos, float _angle, const glm::vec2& _size, float _scale)
	: Object2D<Type2D::INTERFACE, P, TX>(texture), pos(_pos), size(_size * _scale), angle(_angle), speed(0.0f) {
	static int sid;
	this->id = sid++;
	if (TX == Tex2D::IMAGE) {
		this->animator = nullptr;
	} else {
		this->animator = new SpriteAnimator();
		const auto* animatedFrames = dynamic_cast<const AnimationFrames*>(texture);
		auto* anim = new SpriteAnimation(animatedFrames);
		this->animator->addAnimation(anim);
	}
	gLogi("Object2D<Type2D::NODE, Pos2D::" + gToStr(static_cast<int>(P)) + ", Tex2D::" + gToStr(static_cast<int>(TX)) + ">::Object2D")
		<< "Created Object2D with id: " << this->id
		<< ", pos: " << pos.x << ", " << pos.y
		<< ", size: " << size.x << ", " << size.y
		<< ", angle: " << angle;
}

template <Pos2D P, Tex2D TX>
template <Pos2D p, typename std::enable_if<p == Pos2D::MOVING, int>::type>
Object2D<Type2D::NODE, P, TX>::Object2D(typename Texture2DTraits<TX>::type* texture,
	const glm::vec2& _pos, const glm::vec2& _speed, float _angle, const glm::vec2& _size, float _scale)
    : Object2D<Type2D::INTERFACE, P, TX>(texture), pos(_pos), size(_size * _scale), angle(_angle), speed(_speed) {
	static int sid;
	this->id = sid++;
	if (TX == Tex2D::IMAGE) {
		this->animator = nullptr;
	} else {
		this->animator = new SpriteAnimator();
		const auto* animatedFrames = dynamic_cast<const AnimationFrames*>(texture);
		auto* anim = new SpriteAnimation(animatedFrames);
		this->animator->addAnimation(anim);
	}
	gLogi("Object2D<Type2D::NODE, Pos2D::" + gToStr(static_cast<int>(P)) + ", Tex2D::" + gToStr(static_cast<int>(TX)) + ">::Object2D")
		<< "Created Object2D with id: " << this->id
		<< ", pos: " << pos.x << ", " << pos.y
		<< ", size: " << size.x << ", " << size.y
		<< ", draw angle: " << angle
		<< ", speed: " << speed.x << ", " << speed.y;
}

template<Pos2D P, Tex2D TX>
template<Type2D TP2, Pos2D P2>
Object2D<Type2D::NODE, P, TX>::Object2D(
	const Object2D<TP2, P2, TX>& object,
	std::enable_if_t<P == Pos2D::MOVING && (TP2 == Type2D::INTERFACE || TP2 == Type2D::NODE), int>*
)
	: Object2D<Type2D::INTERFACE, P, TX>(object.getTexture()), angle(object.getAngle()) {
	static int sid;
	this->id = sid++;
	this->rect = object.getRect();
	this->animator = new SpriteAnimator();
	auto* anim = new SpriteAnimation(Object2D<Type2D::INTERFACE, P, TX>::texture);
	this->animator->addAnimation(anim);
	gLogi("Object2D<Type2D::NODE, Pos2D::" + gToStr(static_cast<int>(P)) + ", Tex2D::" + gToStr(static_cast<int>(TX)) + ">::Object2D")
		<< "Copied Object2D with id: " << id
		<< ", pos: " << pos.x << ", " << pos.y
		<< ", size: " << size.x << ", " << size.y
		<< ", angle: " << angle
		<< ", speed: " << speed.x << ", " << speed.y;
}

template <Pos2D P, Tex2D TX>
Object2D<Type2D::NODE, P, TX>::~Object2D() {
    if(TX == Tex2D::IMAGE) {
        delete this->animator;
    }
}

template <Pos2D P, Tex2D TX>
template <Pos2D p, typename std::enable_if<p == Pos2D::MOVING, int>::type>
void Object2D<Type2D::NODE, P, TX>::move(const glm::vec2& stepSize, const glm::vec2& minBoundary, const glm::vec2& maxBoundary) {
	pos = glm::clamp(pos + stepSize, minBoundary - size * 0.5f, maxBoundary - size * 0.5f);
}

template <Pos2D P, Tex2D TX>
template <Pos2D P2, Tex2D T2>
void Object2D<Type2D::NODE, P, TX>::move(const Object2D<Type2D::NODE, P2, T2>* targetObject) {
	if (targetObject) {
		pos = targetObject->getMidPosition() - size * 0.5f;
		angle = targetObject->getAngle();
	} else {
		gLoge("Object2D<Type2D::NODE, Pos2D::" + gToStr(static_cast<int>(P)) +", Tex2D::" + gToStr(static_cast<int>(TX)) + ">::move")
			<< "Target object is null!";
	}
}

template <Pos2D P, Tex2D TX>
void Object2D<Type2D::NODE, P, TX>::rotate(float angle) {
	this->angle = fmodf(this->angle + angle, 360.0f);
}

template <Pos2D P, Tex2D TX>
template <Tex2D tx, std::enable_if_t<tx == Tex2D::SPRITE, int>>
void Object2D<Type2D::NODE, P, TX>::update(float deltaTime) {
	if (animator) {
		animator->update(deltaTime);
	}
}

template <Pos2D P, Tex2D TX>
template <Pos2D p, std::enable_if_t<p == Pos2D::MOVING, int>>
void Object2D<Type2D::NODE, P, TX>::update(float deltaTime, const glm::vec2& minBoundary, const glm::vec2& maxBoundary) {
	glm::vec2 stepSize = speed * deltaTime;
	move(stepSize, minBoundary, maxBoundary);
}

// Draw with tag dispatch
template <Pos2D P, Tex2D TX>
void Object2D<Type2D::NODE, P, TX>::draw() {
	drawImpl(std::integral_constant<Tex2D, TX>());
}

// IMAGE
template <Pos2D P, Tex2D TX>
void Object2D<Type2D::NODE, P, TX>::drawImpl(std::integral_constant<Tex2D, Tex2D::IMAGE>) {
	if (Object2D<Type2D::INTERFACE, P, TX>::texture) {
		Object2D<Type2D::INTERFACE, P, TX>::texture->draw(pos.x, pos.y, size.x, size.y, angle);
	}
}

// SPRITE
template <Pos2D P, Tex2D TX>
void Object2D<Type2D::NODE, P, TX>::drawImpl(std::integral_constant<Tex2D, Tex2D::SPRITE>) {
	if (animator) {
		animator->draw(pos, size, angle);
	}
}
