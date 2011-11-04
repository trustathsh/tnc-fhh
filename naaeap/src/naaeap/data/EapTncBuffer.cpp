/* 
 * Copyright (C) 2006-2011 Fachhochschule Hannover
 * (University of Applied Sciences and Arts, Hannover)
 * Faculty IV, Dept. of Computer Science
 * Ricklinger Stadtweg 118, 30459 Hannover, Germany
 * 
 * Email: trust@f4-i.fh-hannover.de
 * Website: http://trust.inform.fh-hannover.de/
 * 
 * This file is part of tnc@fhh, an open source 
 * Trusted Network Connect implementation by the Trust@FHH
 * research group at the Fachhochschule Hannover.
 * 
 * tnc@fhh is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * tnc@fhh is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with tnc@fhh; if not, see <http://www.gnu.org/licenses/>
 */
 
#include "EapTncBuffer.h"

#include <log4cxx/logger.h>
#include <tncs/iel/string_helper.h>
#include <string.h>

namespace tncfhh
{

namespace nal
{

/* for logging */
static log4cxx::LoggerPtr logger(log4cxx::Logger::getLogger("NAA-EAP.EapTncBuffer"));

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *        * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *        *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
EapTncBuffer::EapTncBuffer()
	: tnccsData(0), position(0)
{
	//bzzzz...
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *        * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *        *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
EapTncBuffer::EapTncBuffer(const EapTncFragment &tnccsFragment)
	:tnccsData(tnccsFragment.getTNCCSDataLength()), position(0)
{
	addEapTncFragment(tnccsFragment, false);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *        * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *        *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
EapTncBuffer::EapTncBuffer(const EapTncBuffer &eapTncBuffer)
	:tnccsData(eapTncBuffer.tnccsData), position(eapTncBuffer.position)
{
	throw std::runtime_error("TODO ore not? That's the Question!");
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *        * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *        *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
EapTncBuffer::~EapTncBuffer()
{
	//Flower
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *        * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *        *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
EapTncBuffer &EapTncBuffer::operator=(const EapTncBuffer &tnccsBuffer)
{
	throw std::runtime_error("TODO ore not? That's the other Question!");

//
//	if(this == &tnccsBatchBuffer){
//		return *this;
//	}
//	this->tnccsBatch = tnccsBatchBuffer.tnccsBatch;
//	this->position = tnccsBatchBuffer.position;
//	return *this;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *        * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *        *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void EapTncBuffer::addEapTncFragment(const EapTncFragment &fragment, bool isLast) throw (std::domain_error)
{
	LOG4CXX_TRACE(logger, "Add a EapTncFragment (" << fragment.getDataLength() << " Bytes) at position " << this->position << " of " << this->tnccsData.getLength());

	TNC_UInt32 free = this->tnccsData.getLength() - this->getPosition();
	/* is dataFragment has to much data */
	if (free < fragment.getDataLength())
		throw std::domain_error(std::string("EapTncFragment has to much data (") + fragment.getDataLength() + " Bytes), but only " + free + " Bytes free.");


	/* copy DataFragment into tnccsData */
	memcpy(this->tnccsData.getData() + this->position, fragment.getData(), sizeof(TNC_Buffer)*fragment.getDataLength());
	this->position += fragment.getDataLength();

	/* check if the last fragment, the data must completely */
	if (isLast && this->position != this->tnccsData.getLength())
		throw std::domain_error(std::string("addEapTncFragment: EapTncFragment hasn't enough data. position = ") + (this->position) + ", dataLength = " + (this->tnccsData.getLength()) + ". I'am missing " + (this->tnccsData.getLength() - this->position) + " byte data.");
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *        * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *        *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
iel::TNCCSData EapTncBuffer::getTNCCSData() const
{
	//TODO - exception schmeißen wenn er nicht voll ist
	return this->tnccsData;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *        * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *        *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
TNC_UInt32 EapTncBuffer::getPosition() const
{
	return this->position;
}

} // namespace nal

} // namespace tncfhh
