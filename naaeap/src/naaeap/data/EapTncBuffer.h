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
 
#ifndef EAPTNCBUFFER_H_
#define EAPTNCBUFFER_H_

#include "EapTncFragment.h"

#include <tncs/iel/TNCCSData.h>

namespace tncfhh
{

namespace nal
{

/**
 * TODO_Comment
 *
 * @see ~TNCCSBatchBuffer() 
 * 
 * <h3>Changelog:</h3>
 * <ul>
 *   <li>17.02.2008 - create class (ib)</li>
 *   <li>19.02.2008 - some comments, addTNCCSBatchFragment and getter (mbs)</li>
 *   <li>22.07.2009 - redesign 0.6.0 (mbs)</li>
 * </ul>
 *
 * @class TNCCSBatchBuffer
 * @brief TODO_Brief
 * @date 17.02.2008
 * @author Ingo Bente (ib)
 * @author Mike Steinmetz (mbs)
 */
class EapTncBuffer
{
public:
	
	/**
	 * Default constructor
	 */
	EapTncBuffer();
	
	/**
	 * Constructor. Sets the batch to tnccsBatchFragment;
	 * 
	 * @param tnccsBatchFragment The first batch fragment for this buffer.
	 */
	EapTncBuffer(const EapTncFragment &eapTncFragment);
	
	/**
	 * Copy constructor (flat copy).
	 *
	 * @param tnccsBatchBuffer The buffer that shall be copied.
	 */
	EapTncBuffer(const EapTncBuffer &tnccsBatchBuffer);
	
	/**
	 * Destructor. Does not free any memory. The caller must delete tnccsBatch
	 * (normally it is the Defragmentation).
	 * 
	 * @see getTNCCSBatch
	 */
	virtual ~EapTncBuffer();
	
	/**
	 * TODO_Comment
	 *
	 * @param enclosing_method_arguments
	 */
	EapTncBuffer &operator=(const EapTncBuffer &tnccsBatchBuffer);
	
	/**
	 * Adding a EapTncFragment behind at last fragment.
	 *
	 * @param batchFragment the TNCCS-Batch-Fragment
	 * @param isLast true if batchFragment is the last fragment
	 * 
	 * @trow 
	 */
	void addEapTncFragment(const EapTncFragment &fragment, bool isLast) throw (std::domain_error);
	
	/**
	 * Get the TNCCSBatch.
	 *
	 * @return the tnccsBatch
	 * 
	 * @see ~EapTncBuffer()
	 */
	iel::TNCCSData getTNCCSData() const;
	
	/**
	 * Get the current position of tnccsData is filled.
	 *
	 * @return the position
	 */
	TNC_UInt32 getPosition() const;
	
private:
	
	/**
	 * Pointer to the tnccsData
	 */
	iel::TNCCSData tnccsData;
	
	/**
	 * Current position in the tnccsData.
	 */
	TNC_UInt32 position;
};

} // namespace nal

} // namespace tncfhh

#endif /*EAPTNCBUFFER_H_*/
