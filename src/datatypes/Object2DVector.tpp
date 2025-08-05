#pragma once
#include <glm/gtx/rotate_vector.hpp>

template <Pos2D P, Tex2D TX>
Object2D<Type2D::VECTOR, P, TX>::Object2D() {
	minboundary = glm::vec2(0.0f);
	maxboundary = glm::vec2(0.0f);
}

template <Pos2D P, Tex2D TX>
Object2D<Type2D::VECTOR, P, TX>::~Object2D() {
	for(auto* child : children) {
		delete child;
	}
}

template <Pos2D P, Tex2D TX>
template <Pos2D p, std::enable_if_t<p == Pos2D::MOVING, int>>
void Object2D<Type2D::VECTOR, P, TX>::setup(const glm::vec2& minBoundary, const glm::vec2& maxBoundary, bool dieInBorder) {
	dieinborder = dieInBorder;
	minboundary = minBoundary;
	maxboundary = maxBoundary;
	gLogi("Object2D<Type2D::VECTOR, P, TX>::setup")
		<< "Min boundary: " << minboundary.x << ", " << minboundary.y << " | "
		<< "Max boundary: " << maxboundary.x << ", " << maxboundary.y;
}

template <Pos2D P, Tex2D TX>
template <Pos2D p, Tex2D tx, std::enable_if_t<p == Pos2D::MOVING || tx == Tex2D::SPRITE, int>>
void Object2D<Type2D::VECTOR, P, TX>::update(float deltaTime) {
	for(auto it = children.begin(); it != children.end(); it++) {
		auto* child = *it;
		auto* animatedChild = child->getAnimated();
		if(animatedChild) {
			animatedChild->update(deltaTime);
		}
		auto* movableChild = child->getMovable();
		if(movableChild) {
			glm::vec2 stepSize = movableChild->getSpeed() * deltaTime;
			movableChild->move(stepSize, minboundary - 1.0f, maxboundary + 1.0f); // Allow some margin for movement
			if (dieinborder) {
				if (!movableChild->collision(minboundary, maxboundary)) {
					delete child;
					*it = children.back();
					children.pop_back();
					--it;
				}
			}
		}
	}
}

template <Pos2D P, Tex2D TX>
void Object2D<Type2D::VECTOR, P, TX>::draw() const {
	for(auto* child : children) {
		child->draw();
	}
}

template <Pos2D P, Tex2D TX>
void Object2D<Type2D::VECTOR, P, TX>::addTexture(typename Texture2DTraits<TX>::type&& texture) {
	texturelist.push_back(std::move(texture));
	gLogi("Object2D<Type2D::VECTOR, P, TX>::addTexture") << "Total textures: " << texturelist.size();
}

template <Pos2D P, Tex2D TX>
template <Tex2D tx, typename std::enable_if<tx == Tex2D::SPRITE, int>::type>
void Object2D<Type2D::VECTOR, P, TX>::addTexture(const std::string& fmt, const int iBeg, const int iEnd) {
	using Texture = typename Texture2DTraits<TX>::type;
	Texture texture = loadAnimationFrames(fmt, iBeg, iEnd);
	if (texture.frames.empty()) {
		gLoge("Object2D<Type2D::VECTOR, P, TX>::addTexture")
			<< "Failed to load animation frames from: " << fmt;
		return;
	}
	texturelist.push_back(std::move(texture));
}

template <Pos2D P, Tex2D TX>
template <Tex2D tx, typename std::enable_if<tx == Tex2D::IMAGE, int>::type>
void Object2D<Type2D::VECTOR, P, TX>::addTexture(const std::string& path) {
	using Texture = typename Texture2DTraits<TX>::type;
	Texture* texture = loadFrame(path);
	texturelist.push_back(texture);
}

template <Pos2D P, Tex2D TX>
template <Pos2D p, typename std::enable_if<p == Pos2D::FIXED, int>::type>
void Object2D<Type2D::VECTOR, P, TX>::addObject2D(size_t textureIndex, const glm::vec2& pos,
	float angle, const glm::vec2& size, float scale) {
	using TextureType = typename Texture2DTraits<TX>::type;
	if (textureIndex >= texturelist.size()) {
		gLoge("Object2D<Type2D::VECTOR, P, TX>::addObject2D") << "Index out of bounds: " << textureIndex;
		return;
	}
	auto* child = new Object2D<Type2D::NODE, P, TX>(&texturelist[textureIndex], pos, angle, size, scale);
	children.push_back(child);
}

template <Pos2D P, Tex2D TX>
template <Pos2D p, typename std::enable_if<p == Pos2D::MOVING, int>::type>
void Object2D<Type2D::VECTOR, P, TX>::addObject2D(size_t textureIndex, const glm::vec2& pos,
	const glm::vec2& speed, float angle, const glm::vec2& size, float scale) {
	using TextureType = typename Texture2DTraits<TX>::type;
	if (textureIndex >= texturelist.size()) {
		gLoge("Object2D<Type2D::VECTOR, P, TX>::addObject2D") << "Index out of bounds: " << textureIndex;
		return;
	}
	TextureType* texture = texturelist[textureIndex];
	auto* child = new Object2D<Type2D::NODE, P, TX>(texture, pos, speed, angle, size, scale);
	children.push_back(child);
}

template <Pos2D P, Tex2D TX>
typename std::vector<Object2D<Type2D::INTERFACE, P, TX>*>::iterator
Object2D<Type2D::VECTOR, P, TX>::selectObject2D(const glm::vec2& clickPos) {
	auto selectedIt = children.end();
	float minMargin = std::numeric_limits<float>::max();
	for (auto it = children.begin(); it != children.end(); ++it) {
		float marginLen;
		if ((*it)->collision(marginLen, clickPos)) {
			if (marginLen < minMargin) {
				minMargin = marginLen;
				selectedIt = it;
			}
		}
	}
	return selectedIt;
}
