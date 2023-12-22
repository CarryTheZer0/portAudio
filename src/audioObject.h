/*
 * audioObject.h
 *
 *  Created on: 24 Oct 2022
 *      Author: mchlp
 */

#ifndef AUDIOOBJECT_H_
#define AUDIOOBJECT_H_

class AudioObject
{
public:
	AudioObject() = default;
	virtual ~AudioObject() = default;

	virtual int generateBlock(unsigned int frameCount, float* m_buffer) = 0;  // todo number of channels
};

#endif /* AUDIOOBJECT_H_ */
