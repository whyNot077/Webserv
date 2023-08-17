
#include "Request.hpp"
#include <iostream>

Request::Request() {}

Request::Request(const std::string& request) 
:requestParser(request) {
	std::string line;
	while (std::getline(requestParser, line)) {
		if (line.back() == '\r')
			line.pop_back();
		requestLines.push(line);
	}
	parseRequestLine();
	parseHeaders();
	parseBody();
}

Request::Request(const Request& copy) {
	static_cast<void>(copy);
}

Request& Request::operator=(const Request& copy) {
	static_cast<void>(copy);
	return *this;
}

// 'GET /index.html HTTP/1.1' <- example
void Request::parseMethod(std::string& line) {
    size_t space = line.find(' ');
    if (space == std::string::npos)
        throw std::out_of_range("invalid http, request line!1");
    std::string methodString = line.substr(0, space);
    line = line.substr(space + 1);

    if (methodString == "GET")
        method = GET;
    else if (methodString == "POST")
        method = POST;
    else if (methodString == "DELETE")
        method = DELETE;
    else
        throw std::out_of_range("invalid http, request line!2");
}

void Request::parseURL(std::string& line) {
    size_t space = line.find(' ');
    if (space == std::string::npos)
        throw std::out_of_range("invalid http, request line!3");
    url = line.substr(0, space);
}

void Request::parseVersion(std::string& line, size_t space) {
    version = line.substr(space + 1);
    if (version != "HTTP/1.1")
        throw std::out_of_range("invalid http, request line!4");
}

void Request::parseRequestLine() {
    if (requestLines.empty())
        throw std::out_of_range("invalid http, empty request line!");
    std::string line = requestLines.front();
    requestLines.pop();

    parseMethod(line);
    size_t space = line.find(' ');
    parseURL(line);
    parseVersion(line, space);
}

void Request::parseHeaders(void) {
	while (!requestLines.empty()) {
		std::string line = requestLines.front();
		requestLines.pop();
		if (line.size() == 0)
			break;
		size_t index = line.find(':');
		if (index == std::string::npos || index + 2 >= line.size())
			throw std::out_of_range("invalid http, header!");
		headers[line.substr(0, index)] = line.substr(index + 2);
	}
}

void Request::parseBody(void) {
	while (!requestLines.empty()) {
		body += requestLines.front() + '\n';
		requestLines.pop();
	}
}

Request::METHOD Request::getMethod() {
	return method;
}
const std::string& Request::getUrl() {
	return url;
}

const std::string& Request::getHeaderValue(const std::string& headerName) const {
    std::unordered_map<std::string, std::string>::const_iterator it = headers.find(headerName);
    if (it != headers.end()) {
        return it->second;
    }
    static const std::string emptyString = "";
    return emptyString;
}

const std::string& Request::getBody() const {
	return body;
}
