#include "dto.hpp"

http_dto::http_dto(std::uintptr_t session_id, goblin_engineer::http::request_type request)
    : request_(std::move(request))
    , session_id_(session_id) {}