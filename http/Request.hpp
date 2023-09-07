#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <sstream>
#include <queue>
#include <vector>
#include <map>
#include <algorithm>
#include <cctype>
#include <string>

class Request {
public:
	enum METHOD {
		GET,
		POST,
		DELETE,
		PUT,
		OTHER
	};
private:
	std::vector<char>					requestStr;
	std::queue<std::string>				requestLines;
	METHOD								method;
	std::map<std::string, std::string>	values;
	std::vector<char>					body;
	size_t								bodyPos;
	size_t								chunkSize;
	size_t								chunkStart;
	bool								haveHeader;
	bool								haveBody;
//Reqeust_parse.cpp
private:
    void	parseMethod(std::string& line);
    void	parseURL(const std::string& line);
    void	parseVersion(const std::string& line);
	void	parseBody(void);
    void	addRequestLines(void);
	void	skipCRLF(void);
	size_t	hexToDecimal(char digit) const;
	bool	parseChunkSize(void);
    void	parseChunkedBody(void);
	void	parseRequestLine(void);
	void	parseKeyValues(void);
	void	parseHeader(void);
public:
    void	parse(void);
//Request.cpp
public:
	Request();
	~Request();
	Request(const Request& copy);
	Request&	operator=(const Request& copy);
private:
	size_t	findHeaderEnd(void) const;
public:
	Request::METHOD				getMethod(void) const;
	const std::vector<char>&	getRequestStr(void) const;
	const std::string&			getStrMethod(void) const;
	const std::string&			getURL(void) const;
	const std::vector<char>&	getBody(void) const;
	const std::string&			getVersion(void) const;
	char*						getRequestBuffer(void);
	const std::string&			findValue(const std::string& headerName) const;
	void						addRequest(const std::vector<char>& input);
    bool						isHeaderEnd(void);
	bool						isRequestEnd(void) const;
};

#endif
