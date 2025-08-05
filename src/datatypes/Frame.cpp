//
// Created by sadettin on 8/5/25.
//

#include "Frame.h"

Frame::~Frame() {
	if(frame != nullptr) {
		delete frame;
	}
}

Frame::Frame(Frame&& other) noexcept {
	frame = other.frame;
	other.frame = nullptr;
}

Frame& Frame::operator=(Frame&& other) noexcept {
	if(this != &other) {
		frame = other.frame;
		other.frame = nullptr;
	}
	return *this;
}

Frame loadFrame(const std::string& path) {
	Frame ret;
	ret.frame = new gImage();
	if(!ret.frame->loadImage(path)) {
		gLoge("Frame::loadFrame") << "Failed to load image from: " << path;
		delete ret.frame;
		ret.frame = nullptr;
	}
	return ret;
}
