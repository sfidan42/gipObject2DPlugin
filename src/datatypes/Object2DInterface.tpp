#pragma once

template <Pos2D P, Tex2D TX>
Object2D<Type2D::INTERFACE, P, TX>::Object2D(typename Texture2DTraits<TX>::type* _texture)
	: texture(_texture) {
}

template <Pos2D P, Tex2D TX>
Object2D<Type2D::NODE, Pos2D::MOVING, TX>* Object2D<Type2D::INTERFACE, P, TX>::getMovable() {
	return dynamic_cast<Object2D<Type2D::NODE, Pos2D::MOVING, TX>*>(this);
}

template <Pos2D P, Tex2D TX>
Object2D<Type2D::NODE, P, Tex2D::SPRITE>* Object2D<Type2D::INTERFACE, P, TX>::getAnimated() {
	return dynamic_cast<Object2D<Type2D::NODE, P, Tex2D::SPRITE>*>(this);
}

template <Pos2D P, Tex2D TX>
bool Object2D<Type2D::INTERFACE, P, TX>::collision(float& outMarginLen, const glm::vec2& clickPos) const {
	outMarginLen = glm::length(clickPos - getMidPosition());
	return (outMarginLen <= glm::length(getSize() * 0.5f));
}

template <Pos2D P, Tex2D TX>
bool Object2D<Type2D::INTERFACE, P, TX>::collision(const glm::vec4& rect) {
	glm::vec4 objRect = getRect();
	return (rect.x < objRect.x + objRect.z && rect.x + rect.z > objRect.x &&
	        rect.y < objRect.y + objRect.w && rect.y + rect.w > objRect.y);
}

template <Pos2D P, Tex2D TX>
bool Object2D<Type2D::INTERFACE, P, TX>::collision(const glm::vec2& minBoundary, const glm::vec2& maxBoundary) {
	glm::vec2 midPos = getMidPosition();
	return (minBoundary.x < midPos.x && midPos.x < maxBoundary.x &&
	        minBoundary.y < midPos.y && midPos.y < maxBoundary.y);
}
