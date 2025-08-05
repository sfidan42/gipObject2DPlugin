//
// Created by sadettin on 7/13/25.
//

#ifndef OBJECT2D_H
#define OBJECT2D_H

#include <vector>
#include <glm/glm.hpp>
#include "animation/SpriteAnimator.h"
#include "Frame.h"
#include "AnimationFrames.h"

enum class Type2D { INTERFACE, NODE, VECTOR };

enum class Pos2D { FIXED, MOVING };

enum class Tex2D { IMAGE, SPRITE };

template <Pos2D>
struct Position2DTraits;

template <>
struct Position2DTraits<Pos2D::FIXED> {
	using type2d = const glm::vec2;
	using type4d = const glm::vec4;
};

template <>
struct Position2DTraits<Pos2D::MOVING> {
	using type2d = glm::vec2;
	using type4d = glm::vec4;
};

template <Tex2D>
struct Texture2DTraits;

template <>
struct Texture2DTraits<Tex2D::IMAGE> {
	using type = Frame;
};

template <>
struct Texture2DTraits<Tex2D::SPRITE> {
	using type = AnimationFrames;
};

template <Type2D TP, Pos2D P, Tex2D TX>
class Object2D;

template <Pos2D P, Tex2D TX>
class Object2D<Type2D::INTERFACE, P, TX> {
	using TextureType = typename Texture2DTraits<TX>::type;

public:
	Object2D(const TextureType* texturePtr);
	virtual ~Object2D() = default;
	virtual glm::vec2 getPosition() const = 0;
	virtual glm::vec2 getMidPosition() const = 0;
	virtual glm::vec2 getSize() const = 0;
	virtual glm::vec4 getRect() const = 0;
	virtual float getAngle() const = 0;
	virtual int getId() const = 0;
	virtual void draw() = 0;
	virtual void setSpeed(const glm::vec2& speed) = 0;
	virtual glm::vec2 getSpeed() const = 0;

	Object2D<Type2D::NODE, Pos2D::MOVING, TX>* getMovable();
	Object2D<Type2D::NODE, P, Tex2D::SPRITE>* getAnimated();

	const TextureType* getTexture() const { return textureptr; }

	bool collision(float& outMarginLen, const glm::vec2& clickPos) const;
	bool collision(const glm::vec4& rect);
	bool collision(const glm::vec2& minBoundary, const glm::vec2& maxBoundary);

protected:
	const TextureType* textureptr;
};

#include "datatypes/Object2DInterface.tpp"

template <Pos2D P, Tex2D TX>
class Object2D<Type2D::NODE, P, TX> : public Object2D<Type2D::INTERFACE, P, TX> {
	using TextureType = typename Texture2DTraits<TX>::type;
	using Position2DType = typename Position2DTraits<P>::type2d;
	using Position4DType = typename Position2DTraits<P>::type4d;

public:
	template <Pos2D p = P, std::enable_if_t<p == Pos2D::FIXED, int> = 0>
	Object2D(TextureType* texture, const glm::vec2& _pos,
	         float _angle, const glm::vec2& _size, float _scale = 1.0f);

	template <Pos2D p = P, std::enable_if_t<p == Pos2D::MOVING, int> = 0>
	Object2D(TextureType* texture, const glm::vec2& _pos, const glm::vec2& _speed,
	         float _angle, const glm::vec2& _size, float _scale = 1.0f);

	template <Type2D TP2, Pos2D P2>
	explicit Object2D(
		const Object2D<TP2, P2, TX>& object,
		std::enable_if_t<P == Pos2D::MOVING && (TP2 == Type2D::INTERFACE || TP2 == Type2D::NODE), int>* = nullptr
	);

	~Object2D();

	glm::vec2 getPosition() const { return pos; }
	glm::vec2 getMidPosition() const { return pos + size * 0.5f; }
	glm::vec2 getSize() const { return size; }
	glm::vec4 getRect() const { return rect; }
	float getAngle() const { return angle; }
	int getId() const { return id; }
	void setSpeed(const glm::vec2& speed) { this->speed = speed; }
	glm::vec2 getSpeed() const { return speed; }

	template <Pos2D p = P, std::enable_if_t<p == Pos2D::MOVING, int>  = 0>
	void move(const glm::vec2& stepSize, const glm::vec2& minBoundary, const glm::vec2& maxBoundary);

	template <Pos2D P2, Tex2D T2>
	void move(const Object2D<Type2D::NODE, P2, T2>* targetObject);

	void rotate(float angle);

	template <Tex2D tx = TX, std::enable_if_t<tx == Tex2D::SPRITE, int>  = 0>
	void update(float deltaTime);

	template <Pos2D p = P, std::enable_if_t<p == Pos2D::MOVING, int>  = 0>
	void update(float deltaTime, const glm::vec2& minBoundary, const glm::vec2& maxBoundary);

	void draw();

private:
	void Object2DImpl(std::integral_constant<Tex2D, Tex2D::IMAGE>, TextureType* texture) { animator = nullptr; }
	void Object2DImpl(std::integral_constant<Tex2D, Tex2D::SPRITE>, TextureType* texture);

	void drawImpl(std::integral_constant<Tex2D, Tex2D::IMAGE>);
	void drawImpl(std::integral_constant<Tex2D, Tex2D::SPRITE>);

	int id;

	union {
		Position4DType rect;

		struct {
			Position2DType pos;
			Position2DType size;
		};
	};

	float angle;
	glm::vec2 speed = glm::vec2(0.0f);

	SpriteAnimator* animator;
};

#include "datatypes/Object2DNode.tpp"

template <Pos2D P, Tex2D TX>
class Object2D<Type2D::VECTOR, P, TX> {
	using TextureType = typename Texture2DTraits<TX>::type;

public:
	Object2D();
	virtual ~Object2D();

	template <Pos2D p = P, std::enable_if_t<p == Pos2D::MOVING, int>  = 0>
	void setup(const glm::vec2& minBoundary, const glm::vec2& maxBoundary, bool dieInBorder = false);

	template <Pos2D p = P, Tex2D tx = TX, std::enable_if_t<p == Pos2D::MOVING || tx == Tex2D::SPRITE, int>  = 0>
	void update(float deltaTime);

	void draw() const;

	void addTexture(TextureType&& texture);

	template <Tex2D tx = TX, std::enable_if_t<tx == Tex2D::SPRITE, int>  = 0>
	void addTexture(const std::string& fmt, const int iBeg, const int iEnd);

	template <Tex2D tx = TX, std::enable_if_t<tx == Tex2D::IMAGE, int>  = 0>
	void addTexture(const std::string& path);

	template <Pos2D p = P, std::enable_if_t<p == Pos2D::FIXED, int>  = 0>
	void addObject2D(size_t textureIndex, const glm::vec2& pos,
	                 float angle, const glm::vec2& size, float scale = 1.0f);

	template <Pos2D p = P, std::enable_if_t<p == Pos2D::MOVING, int>  = 0>
	void addObject2D(size_t textureIndex, const glm::vec2& pos, const glm::vec2& speed,
	                 float angle, const glm::vec2& size, float scale = 1.0f);

	using it = typename std::vector<Object2D<Type2D::INTERFACE, P, TX>*>::iterator;
	it begin() { return children.begin(); }
	it end() { return children.end(); }
	Object2D<Type2D::INTERFACE, P, TX>* back() { return children.back(); }
	Object2D<Type2D::INTERFACE, P, TX>* front() { return children.front(); }
	void push_back(Object2D<Type2D::INTERFACE, P, TX>* obj) { children.push_back(obj); }
	void pop_back() { children.pop_back(); }
	size_t size() const { return children.size(); }

	it selectObject2D(const glm::vec2& clickPos);

private:
	glm::vec2 minboundary;
	glm::vec2 maxboundary;
	bool dieinborder = false;

	std::vector<Object2D<Type2D::INTERFACE, P, TX>*> children;
	std::vector<TextureType> texturelist;
};

#include "datatypes/Object2DVector.tpp"

#endif //OBJECT2D_H
