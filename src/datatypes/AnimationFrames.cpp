//
// Created by sadettin on 10.07.2025.
//

#include "AnimationFrames.h"

AnimationFrames::~AnimationFrames() {
	for(auto frame : frames) {
		delete frame;
	}
}

AnimationFrames::AnimationFrames(AnimationFrames&& other) noexcept {
	frames = std::move(other.frames);
	other.frames.clear();
}

AnimationFrames& AnimationFrames::operator=(AnimationFrames&& other) noexcept {
	if(this != &other) {
		frames = std::move(other.frames);
		other.frames.clear();
	}
	return *this;
}

AnimationFrames loadAnimatedFrames(const std::vector<std::string>& framePaths) {
	AnimationFrames animatedFrames;
	animatedFrames.frames.reserve(framePaths.size());
	for(const auto& path : framePaths) {
		auto img = new gImage();
		img->loadImage(path);
		animatedFrames.frames.push_back(img);
	}
	return animatedFrames;
}
