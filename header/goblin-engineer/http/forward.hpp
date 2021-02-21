#pragma once

#include <boost/beast/http.hpp>


namespace goblin_engineer { namespace http {

        namespace beast = boost::beast;
        namespace http = beast::http;
        using method_t = http::verb;

        using request_t = http::request<http::string_body>;
        using response_t = http::response<http::string_body>;
        using status_t = http::status;


        inline status_t status(response_t &response ){
            return response.result();
        }

        inline  void method(request_t&request,  method_t method) {
            request.method(method);
        }

        inline  method_t method(request_t&request) {
            return request.method();
        }

        inline  void header(request_t&request,boost::beast::string_view name, boost::beast::string_view const& value  ) {
            request.set(name,value);
        }

        inline request_t::body_type::value_type & body(request_t&request ){
            return request.body();
        }

        inline void body(request_t&request, request_t::body_type::value_type body ){
            request.body() = std::move(body);
        }

        inline void body(request_t&request, request_t::body_type::value_type& body ){
            request.body() = std::move(body);
        }


}}
