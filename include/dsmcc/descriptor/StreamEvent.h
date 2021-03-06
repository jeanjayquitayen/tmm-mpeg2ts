/******************************************************************************
TeleMídia Multiplexer MPEG-2 Transport Stream library
Copyright (C) 2016 TeleMídia Lab/PUC-Rio
https://github.com/TeleMidia/tmm-mpeg2ts/graphs/contributors

This program is free software: you can redistribute it and/or modify it under
the terms of the GNU Affero General Public License as published by the Free
Software Foundation, either version 3 of the License, or (at your option) any
later version. This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License
for more details. You should have received a copy of the GNU Affero General
Public License along with this program. If not, see http://www.gnu.org/licenses/.

*******************************************************************************/

#ifndef STREAMEVENT_H_
#define STREAMEVENT_H_

#include "MpegDescriptor.h"

using namespace std;

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {
namespace dsmcc {

class StreamEvent : public MpegDescriptor {
	#define SE_OPEN_BASE						0x00
	#define SE_ACTIVATE_BASE					0x01
	#define SE_DEACTIVATE_BASE					0x02
	#define SE_SAVE_BASE						0x03
	#define SE_CLOSE_BASE						0x04
	#define SE_ADD_DOCUMENT						0x05
	#define SE_REMOVE_DOCUMENT					0x06
	#define SE_START_DOCUMENT					0x07
	#define SE_STOP_DOCUMENT					0x08
	#define SE_PAUSE_DOCUMENT					0x09
	#define SE_RESUME_DOCUMENT					0x0A
	#define SE_SAVE_DOCUMENT					0x2E
	#define SE_ADD_REGION_BASE					0x0D
	#define SE_REMOVE_REGION_BASE				0x0E
	#define SE_ADD_RULE							0x0F
	#define SE_REMOVE_RULE						0x10
	#define SE_ADD_RULE_BASE					0x11
	#define SE_REMOVE_RULE_BASE					0x12
	#define SE_ADD_CONNECTOR					0x13
	#define SE_REMOVE_CONNECTOR					0x14
	#define SE_ADD_CONNECTOR_BASE				0x15
	#define SE_REMOVE_CONNECTOR_BASE			0x16
	#define SE_ADD_DESCRIPTOR					0x17
	#define SE_REMOVE_DESCRIPTOR				0x18
	#define SE_ADD_DESCRIPTOR_SWITCH			0x19
	#define SE_REMOVE_DESCRIPTOR_SWITCH			0x1A
	#define SE_ADD_DESCRIPTOR_BASE				0x1B
	#define SE_REMOVE_DESCRIPTOR_BASE			0x1C
	#define SE_ADD_TRANSITION					0x1D
	#define SE_REMOVE_TRANSITION				0x1E
	#define SE_ADD_TRANSITION_BASE				0x1F
	#define SE_REMOVE_TRANSITION_BASE			0x20
	#define SE_ADD_IMPORT_BASE					0x21
	#define SE_REMOVE_IMPORT_BASE				0x22
	#define SE_ADD_IMPORTED_DOCUMENT_BASE		0x23
	#define SE_REMOVE_IMPORTED_DOCUMENT_BASE	0x24
	#define SE_ADD_IMPORT_NCL					0x25
	#define SE_REMOVE_IMPORT_NCL				0x26
	#define SE_ADD_NODE							0x27
	#define SE_REMOVE_NODE						0x28
	#define SE_ADD_INTERFACE					0x29
	#define SE_REMOVE_INTERFACE					0x2A
	#define SE_ADD_LINK							0x2B
	#define SE_REMOVE_LINK						0x2C
	#define SE_SET_PROPERTY_VALUE				0x2D

	private:

	protected:
		unsigned short eventId;
		int64_t eventNPT;
		unsigned char privateDataLength;
		unsigned char commandTag;
		unsigned char sequenceNumber;
		unsigned char finalFlag;
		char* privateDataPayload;
		unsigned char privateDataPayloadLength;
		unsigned char fcs;

		int process();
		int updateStream();

		unsigned int calculateDescriptorSize();

	public:
		StreamEvent();
		StreamEvent(char* dataStream, int length);
		virtual ~StreamEvent();

		unsigned short getEventId();
		int64_t getEventNPT();
		unsigned char getPrivateDataLength();
		unsigned char getCommandTag();
		unsigned char getSequenceNumber();
		unsigned char getFinalFlag();
		int getPrivateDataPayload(char** dataStream);
		unsigned char getPrivateDataPayloadLength();
		unsigned char getFcs();

		void setEventId(unsigned short id);
		void setEventNPT(int64_t npt);
		void setCommandTag(unsigned char tag);
		void setSequenceNumber(unsigned char seqNum);
		void setFinalFlag(unsigned char flag);
		int setPrivateDataPayload(char* data, unsigned char length);
		void setFcs(unsigned char fcs);

		static unsigned char checksum(char* stream, int length);
		static unsigned char calculateChecksum(char* stream, int length);

};

}
}
}
}
}


#endif /* STREAMEVENT_H_ */
