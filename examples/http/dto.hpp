#pragma once

#include <boost/beast/http.hpp>

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>

#include <goblin-engineer/http.hpp>


class http_dto final  {
public:
    ///websocket_raw()=default;
    http_dto(std::uintptr_t session_id, goblin_engineer::http::request_t request);
    //~websocket_raw() =default;
    goblin_engineer::http::request_t request_;
    std::uintptr_t session_id_ ;
};