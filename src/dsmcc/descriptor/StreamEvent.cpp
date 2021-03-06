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

#include "dsmcc/descriptor/StreamEvent.h"

#include <string.h>

namespace br {
namespace pucrio {
namespace telemidia {
namespace mpeg2 {
namespace dsmcc {

StreamEvent::StreamEvent() : MpegDescriptor(0x1A) {
	privateDataPayloadLength = 0;
	privateDataLength = 0;
	privateDataPayload = NULL;
	eventId = 0;
	commandTag = 0;
	sequenceNumber = 0;
	finalFlag = 0;
	fcs = 0;
	descriptorLength = 1;
}

StreamEvent::~StreamEvent() {
	if (privateDataPayload != NULL) {
		delete (privateDataPayload);
	}
}

int StreamEvent::process() {
	int pos = MpegDescriptor::process();

	eventId = ((stream[pos] & 0xFF) << 8) | (stream[pos + 1] & 0xFF);
	pos = pos + 5;

	eventNPT = 0;
	eventNPT = (stream[pos++] & 0x01);
	eventNPT = eventNPT << 8;
	eventNPT = eventNPT | (stream[pos++] & 0xFF);
	eventNPT = eventNPT << 8;
	eventNPT = eventNPT | (stream[pos++] & 0xFF);
	eventNPT = eventNPT << 8;
	eventNPT = eventNPT | (stream[pos++] & 0xFF);
	eventNPT = eventNPT << 8;
	eventNPT = eventNPT | (stream[pos++] & 0xFF);

	privateDataLength = (stream[pos++] & 0xFF);
	commandTag = (stream[pos++] & 0xFF);
	sequenceNumber = (stream[pos] & 0x7F) >> 1;
	finalFlag = (stream[pos++] & 0x01);

	if (privateDataPayload != NULL) {
		delete (privateDataPayload);
	}
	try {
		privateDataPayloadLength = privateDataLength - 3;
		privateDataPayload = new char[privateDataPayloadLength];
		memcpy(privateDataPayload, stream + pos, privateDataPayloadLength);
	} catch(...) {
		return -1;
	}

	pos = pos + privateDataPayloadLength;

	fcs = (stream[pos++] & 0xFF);
	return 0;
}

int StreamEvent::updateStream() {
	int pos = MpegDescriptor::updateStream();

	stream[pos++] = ((eventId >> 8) & 0xFF);
	stream[pos++] = (eventId & 0xFF);
	stream[pos++] = 0xFF;
	stream[pos++] = 0xFF;
	stream[pos++] = 0xFF;
	stream[pos]   = 0xFE;

	stream[pos] = stream[pos] | ((eventNPT >> 32) & 0x01);

	pos++;

	stream[pos++] = (eventNPT >> 24) & 0xFF;
	stream[pos++] = (eventNPT >> 16) & 0xFF;
	stream[pos++] = (eventNPT >> 8) & 0xFF;
	stream[pos++] = eventNPT & 0xFF;

	privateDataLength = calculateDescriptorSize() - 13;

	stream[pos++] = privateDataLength & 0xFF;
	stream[pos++] = commandTag & 0xFF;
	stream[pos] = (sequenceNumber << 1) & 0xFE;
	stream[pos] = stream[pos] | (finalFlag & 0x01);

	pos++;

	memcpy(stream + pos, privateDataPayload, privateDataPayloadLength);
	pos = pos + privateDataPayloadLength;

	fcs = StreamEvent::checksum(stream + 12, privateDataLength - 1);

	stream[pos++] = (fcs & 0xFF);

	return pos;
}

unsigned int StreamEvent::calculateDescriptorSize() {
	int pos = MpegDescriptor::calculateDescriptorSize();
	return pos + 14 + privateDataPayloadLength;
}

unsigned short StreamEvent::getEventId() {
	return eventId;
}

int64_t StreamEvent::getEventNPT() {
	return eventNPT;
}

unsigned char StreamEvent::getPrivateDataLength() {
	return privateDataLength;
}

unsigned char StreamEvent::getCommandTag() {
	return commandTag;
}

unsigned char StreamEvent::getSequenceNumber() {
	return sequenceNumber;
}

unsigned char StreamEvent::getFinalFlag() {
	return finalFlag;
}

int StreamEvent::getPrivateDataPayload(char** dataStream) {
	*dataStream = privateDataPayload;
	return privateDataPayloadLength;
}

unsigned char StreamEvent::getPrivateDataPayloadLength() {
	return privateDataPayloadLength;
}

unsigned char StreamEvent::getFcs() {
	return fcs;
}

void StreamEvent::setEventId(unsigned short id) {
	eventId = id;
}

void StreamEvent::setEventNPT(int64_t npt) {
	eventNPT = npt;
}

void StreamEvent::setCommandTag(unsigned char tag) {
	commandTag = tag;
}

void StreamEvent::setSequenceNumber(unsigned char seqNum) {
	sequenceNumber = (seqNum & 0x7F);
}

void StreamEvent::setFinalFlag(unsigned char flag) {
	finalFlag = (flag & 0x01);
}

int StreamEvent::setPrivateDataPayload(char* data, unsigned char length) {
	if (privateDataPayload != NULL) {
		delete (privateDataPayload);
	}
	try {
		privateDataPayload = new char[length];
	} catch(...) {
		return -1;
	}
	memcpy(privateDataPayload, data, length);
	privateDataPayloadLength = length;
	privateDataLength = privateDataPayloadLength + 3;
	descriptorLength = privateDataLength + 13;
	return privateDataPayloadLength;
}

void StreamEvent::setFcs(unsigned char fcs) {
	this->fcs = fcs;
}

unsigned char StreamEvent::checksum(char* stream, int length) {
	unsigned char cs, old;

	if (length) return 0;

	cs = stream[0];
	old = cs;

	for (int i = 1; i < length; i++) {
		cs += stream[i];
		if (cs < old) ++cs;
		old = cs;
	}

	cs = ~cs;

	if (cs == 0x00) cs = 0xFF;

	return cs;
}

unsigned char StreamEvent::calculateChecksum(char* stream, int length) {
	unsigned char cs = 0;
	for (int i = 0; i < length; i++) {
		cs ^= stream[i];
	}
	cs = ~cs;
	if (cs == 0x00) cs = 0xFF;
	return cs;
}

}
}
}
}
}
