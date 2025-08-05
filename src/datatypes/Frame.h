//
// Created by sadettin on 8/5/25.
//

#ifndef FRAME_H
#define FRAME_H
#include "gImage.h"


struct Frame {
	Frame() = default;
	~Frame();
	Frame(const Frame&) = delete;
	Frame& operator=(const Frame&) = delete;
	Frame(Frame&& other) noexcept;
	Frame& operator=(Frame&& other) noexcept;

	gImage* frame;
};

Frame loadFrame(const std::string& path);


#endif //FRAME_H
