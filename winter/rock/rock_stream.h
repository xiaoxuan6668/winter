#ifndef __WINTER_ROCK_ROCK_STREAM_H__
#define __WINTER_ROCK_ROCK_STREAM_H__

#include "winter/streams/async_socket_stream.h"
#include "rock_protocol.h"

namespace winter {

struct RockResult {
   typedef std::shared_ptr<RockResult> ptr; 
   RockResult(uint32_t _result, RockResponse::ptr rsp)
    :result(_result)
    ,response(rsp) {
   }
   uint32_t result;
   RockResponse::ptr response;
};

class RockStream : public winter::AsyncSocketStream {
public:
    typedef std::shared_ptr<RockStream> ptr;
    typedef std::function<bool(winter::RockRequest::ptr
                               ,winter::RockResponse::ptr
                               ,winter::RockStream::ptr)> request_handler;
    typedef std::function<bool(winter::RockNotify::ptr
                               ,winter::RockStream::ptr)> notify_handler;

    RockStream(Socket::ptr sock);

    int32_t sendMessage(Message::ptr msg);
    RockResult::ptr request(RockRequest::ptr req, uint32_t timeout_ms);

    request_handler getRequestHandler() const { return m_requestHandler;}
    notify_handler getNotifyHandler() const { return m_notifyHandler;}

    void setRequestHandler(request_handler v) { m_requestHandler = v;}
    void setNotifyHandler(notify_handler v) { m_notifyHandler = v;}
protected:
    struct RockSendCtx : public SendCtx {
        typedef std::shared_ptr<RockSendCtx> ptr;
        Message::ptr msg;

        virtual bool doSend(AsyncSocketStream::ptr stream) override;
    };

    struct RockCtx : public Ctx {
        typedef std::shared_ptr<RockCtx> ptr;
        RockRequest::ptr request;
        RockResponse::ptr response;

        virtual bool doSend(AsyncSocketStream::ptr stream) override;
    };

    virtual Ctx::ptr doRecv() override;

    void handleRequest(winter::RockRequest::ptr req);
    void handleNotify(winter::RockNotify::ptr nty);
private:
    RockMessageDecoder::ptr m_decoder;
    request_handler m_requestHandler;
    notify_handler m_notifyHandler;
};

class RockSession : public RockStream {
public:
    typedef std::shared_ptr<RockSession> ptr;
    RockSession(Socket::ptr sock);
};

class RockConnection : public RockStream {
public:
    typedef std::shared_ptr<RockConnection> ptr;
    RockConnection();
    bool connect(winter::Address::ptr addr);
};

}

#endif