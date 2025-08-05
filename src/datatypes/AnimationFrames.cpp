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

AnimationFrames loadAnimationFrames(const std::string& fmt, const int iBeg, const int iEnd) {
	AnimationFrames ret;
	char buffer[256];
	ret.frames.reserve(iEnd - iBeg + 1);
	for (int i = iBeg; i <= iEnd; ++i) {
		std::snprintf(buffer, sizeof(buffer), fmt.c_str(), i);
		std::string path(buffer);
		auto* frame = new gImage();
		if(!frame->loadImage(path)) {
			gLoge("AnimationFrames::loadAnimationFrames") << "Failed to load image from: " << path;
			delete frame;
			continue;
		}
		ret.frames.push_back(frame);
	}
	return ret;
}
