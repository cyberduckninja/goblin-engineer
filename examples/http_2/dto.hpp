#pragma once

#include <boost/beast/http.hpp>

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>

using request_type = http::request<http::dynamic_body>;
using response_type = http::response<http::dynamic_body>;


class http_dto final  {
public:
    ///websocket_raw()=default;
    http_dto(std::uintptr_t session_id, request_type request);
    //~websocket_raw() =default;
    request_type request_;
    std::uintptr_t session_id_ ;
};

http_dto::http_dto(std::uintptr_t session_id, request_type request)
    : request_(std::move(request))
    , session_id_(session_id) {}