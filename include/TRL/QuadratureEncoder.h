/*
 * QuadratureEncoder.h
 *
 *  Created on: Feb 4, 2015
 *      Author: cwenck
 */

#ifndef QUADRATUREENCODER_H_
#define QUADRATUREENCODER_H_

#include "main.h"

namespace TRL {

	class QuadratureEncoder : public GenericEncoder {
	private:
		DigitalPort top;
		DigitalPort bottom;
		bool inverted;
		PROS_Encoder encoder;
	public:
		QuadratureEncoder(DigitalPort top, DigitalPort bottom);
		QuadratureEncoder(DigitalPort top, DigitalPort bottom, bool inverted);
		virtual ~QuadratureEncoder();
		int getValue();
		void resetValue();	//resets the value to 0
		EncoderType getEncoderType();
		SensorType getSensorType();

	};
}

#endif /* QUADRATUREENCODER_H_ */
