//
// Created by sadettin on 10.07.2025.
//

#ifndef ANIMATEDFRAMES_H
#define ANIMATEDFRAMES_H
#include <vector>
#include "gImage.h"

struct AnimationFrames {
	AnimationFrames() = default;
	~AnimationFrames();
	AnimationFrames(const AnimationFrames&) = delete;
	AnimationFrames& operator=(const AnimationFrames&) = delete;
	AnimationFrames(AnimationFrames&& other) noexcept;
	AnimationFrames& operator=(AnimationFrames&& other) noexcept;

	std::vector<gImage*> frames;
};

AnimationFrames loadAnimatedFrames(const std::vector<std::string>& framePaths);

#endif //ANIMATEDFRAMES_H
