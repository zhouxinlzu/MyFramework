#include <string>
#include <sstream>
using std::string;
using std::ostringstream;
#include "clientSM.h"
#include "type.h"
#include "log.h"
#include "head.h"
#include "commonFuncation.h"

using namespace myframework;
//#include "common/log.h"
//#include <boost/scoped_array.hpp>
//using boost::scoped_array;

ClientSM::ClientSM(NetConnection *conn) : mNetConnection(conn)
{
	//mClientIP = mNetConnection->GetPeerName();
	//SET_HANDLER(this, &ClientSM::HandleRequest);
	mClientIP = mNetConnection->GetClientIp();
	MY_LOG_STREAM_DEBUG << "new ClientSM, client ip: " << mClientIP << MY_LOG_EOM;
}

ClientSM::~ClientSM()
{
	
}

///
/// Send out the response to the client request.  The response is
/// generated by MetaRequest as per the protocol.
/// @param[in] op The request for which we finished execution.
///
/*void
ClientSM::SendResponse(MetaRequest *op)
{
	IOBuffer::OStream os;

	op->response(os);

	if (op->op == META_ALLOCATE) {
		MetaAllocate *alloc = static_cast<MetaAllocate *>(op);
		ostringstream o;

		o << "alloc: " << alloc->chunkId << ' ' << alloc->chunkVersion << ' ';	 

		for (uint32_t i = 0; i < alloc->servers.size(); i++) {
			o << alloc->servers[i]->ServerID() << ' ';
		}
		KFS_LOG_VA_DEBUG("Client = %s, Allocate: %s", mClientIP.c_str(), o.str().c_str());
	}

	if (op->op != META_LOOKUP) {
		KFS_LOG_VA_INFO("Client = %s, Command %s, Status: %d", 
				mClientIP.c_str(), op->Show().c_str(), op->status);
	}

	mNetConnection->Write(&os);
}
*/
/// Generic event handler.  Decode the event that occurred and
/// appropriately extract out the data and deal with the event.
/// @param[in] code: The type of event that occurred
/// @param[in] data: Data being passed in relative to the event that
/// occurred.
/// @retval 0 to indicate successful event handling; -1 otherwise.
///
int ClientSM::HandleRequest(int code, void *data)
{
	IOBuffer *iobuf;
	//MetaRequest *op;
	int bodyLen;
    int hdrsz;
	
	MY_LOG_STREAM_DEBUG << "ClientSM::HandleRequest, code is  " << code <<  MY_LOG_EOM;
	switch (code) {
		case EVENT_NET_READ:
			iobuf = (IOBuffer *) data;

			while(1){
			char *buf = iobuf->GetReadData(sizeof(Head));
			//Head *pHead = (Head *)iobuf->mBuffer;
			if(buf == NULL){
				MY_LOG_STREAM_DEBUG << "read head from buffer error " << MY_LOG_EOM;
				return -1;
			}
			Head *pHead = (Head *)buf;
			MY_LOG_STREAM_DEBUG <<"socketfd: " << iobuf->mSocketFd << ",cmd: " << pHead->mCmd << ",errno: " << pHead->mErrorNo << ",seq: " << pHead->mSeq <<",bodylen: " << pHead->mBodyLen << MY_LOG_EOM;
			if(pHead->mBodyLen <= 0){
				MY_LOG_STREAM_ERROR << "body len " << pHead->mBodyLen << " < " << " 0" << MY_LOG_EOM;
				return -1;
			}
			buf = iobuf->GetReadData(pHead->mBodyLen);
			if(buf == NULL){
				MY_LOG_STREAM_ERROR << "read body from buffer error " << MY_LOG_EOM;
				return -1;
			}
			switch(pHead->mCmd){
				//TODO: process every cmd
				/*case XXX_REQ:
					HandleXXXRequest(buf, pHead->mBodyLen, pHead->mSeq);
					break;
				*/
				default:
					MY_LOG_STREAM_ERROR << " nothing to do for " << pHead->mCmd << MY_LOG_EOM;
					break;
			}
			
		}
		break;
				

	/*
	case EVENT_NET_WROTE:
		// Something went out on the network.  For now, we don't
		// track it. Later, we may use it for tracking throttling
		// and such.
		break;

	case EVENT_CMD_DONE:
		
		break;

	case EVENT_NET_ERROR:

		
		break;

	default:
		
		return -1;*/
	}
	return 0;
}




