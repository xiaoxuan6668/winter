#include "rock_stream.h"
#include "winter/log.h"

namespace winter {

static winter::Logger::ptr g_logger = WINTER_LOG_NAME("system");

RockStream::RockStream(Socket::ptr sock)
    :AsyncSocketStream(sock, true)
    ,m_decoder(new RockMessageDecoder) {
}

int32_t RockStream::sendMessage(Message::ptr msg) {
    RockSendCtx::ptr ctx(new RockSendCtx);
    ctx->msg = msg;
    enqueue(ctx);
    return 1;
}

RockResult::ptr RockStream::request(RockRequest::ptr req, uint32_t timeout_ms) {
    RockCtx::ptr ctx(new RockCtx);
    ctx->request = req;
    ctx->sn = req->getSn();
    ctx->timeout = timeout_ms;
    ctx->scheduler = winter::Scheduler::GetThis();
    ctx->fiber = winter::Fiber::GetThis();
    addCtx(ctx);
    //ctx->timer = winter::IOManager::GetThis()->addTimer(timeout_ms,
    //        std::bind(&AsyncSocketStream::onTimeOut, shared_from_this(), ctx));
    enqueue(ctx);
    winter::Fiber::YieldToHold();
    return std::make_shared<RockResult>(ctx->result, ctx->response);
}

bool RockStream::RockSendCtx::doSend(AsyncSocketStream::ptr stream) {
    return std::dynamic_pointer_cast<RockStream>(stream)
                ->m_decoder->serializeTo(stream, msg) > 0;
}

bool RockStream::RockCtx::doSend(AsyncSocketStream::ptr stream) {
    return std::dynamic_pointer_cast<RockStream>(stream)
                ->m_decoder->serializeTo(stream, request) > 0;
}

AsyncSocketStream::Ctx::ptr RockStream::doRecv() {
    auto msg = m_decoder->parseFrom(shared_from_this());
    if(!msg) {
        innerClose();
        return nullptr;
    }

    int type = msg->getType();
    if(type == Message::RESPONSE) {
        auto rsp = std::dynamic_pointer_cast<RockResponse>(msg);
        if(!rsp) {
            WINTER_LOG_WARN(g_logger) << "RockStream doRecv response not RockResponse: "
                << msg->toString();
            return nullptr;
        }
        RockCtx::ptr ctx = getAndDelCtxAs<RockCtx>(rsp->getSn());
        if(!ctx) {
            WINTER_LOG_INFO(g_logger) << "RockStream request timeout reponse="
                << rsp->toString() << " - request=" << ctx->request->toString();
            return nullptr;
        }
        ctx->result = rsp->getResult();
        ctx->response = rsp;
        return ctx;
    } else if(type == Message::REQUEST) {
        auto req = std::dynamic_pointer_cast<RockRequest>(msg);
        if(!req) {
            WINTER_LOG_WARN(g_logger) << "RockStream doRecv request not RockRequest: "
                << msg->toString();
            return nullptr;
        }
        if(m_requestHandler) {
            m_iomanager->schedule(std::bind(&RockStream::handleRequest,
                        std::dynamic_pointer_cast<RockStream>(shared_from_this()),
                        req));
        }
    } else if(type == Message::NOTIFY) {
        auto nty = std::dynamic_pointer_cast<RockNotify>(msg);
        if(!nty) {
            WINTER_LOG_WARN(g_logger) << "RockStream doRecv notify not RockNotify: "
                << msg->toString();
            return nullptr;
        }

        if(m_notifyHandler) {
            m_iomanager->schedule(std::bind(&RockStream::handleNotify,
                        std::dynamic_pointer_cast<RockStream>(shared_from_this()),
                        nty));
        }
    } else {
        WINTER_LOG_WARN(g_logger) << "RockStream recv unknow type=" << type
            << " msg: " << msg->toString();
    }
    return nullptr;
}

void RockStream::handleRequest(winter::RockRequest::ptr req) {
    winter::RockResponse::ptr rsp = req->createResponse();
    if(!m_requestHandler(req, rsp
        ,std::dynamic_pointer_cast<RockStream>(shared_from_this()))) {
        innerClose();
    }
}

void RockStream::handleNotify(winter::RockNotify::ptr nty) {
    if(!m_notifyHandler(nty
        ,std::dynamic_pointer_cast<RockStream>(shared_from_this()))) {
        innerClose();
    }
}

RockSession::RockSession(Socket::ptr sock)
    :RockStream(sock) {
    m_autoConnect = false;
}

RockConnection::RockConnection()
    :RockStream(nullptr) {
    m_autoConnect = true;
}

bool RockConnection::connect(winter::Address::ptr addr) {
    m_socket = winter::Socket::CreateTCP(addr);
    return m_socket->connect(addr);
}


}