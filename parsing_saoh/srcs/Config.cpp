#include "Config.hpp"
#include <iostream> // 지워야함 테스트용

/*
    get_conf_stream() - conf_name 을 이용하여 파일스트림 을 연다.
    fs : 파일스트림
    conf_name : 알규먼트로 들어왔거나 default config 파일의 이름이나 절대경로
    설명 : 절대 경로로 들어왔을 경우를 상정하여 파일스트림을 열어보고 존재하지 않는다면,
          현재 웹서버 디렉토리의 위치의 config 디렉토리의 conf_name 파일을 연다.
          그래도 존재하지 않는다면 에러
*/

void Configure::get_conf_stream(std::fstream &fs, const std::string &conf_name)
{
    fs.open(conf_name);
    if (!fs.is_open())
    {
        std::string conf_root = getcwd(nullptr, 1000);
        conf_root += "/config/" + conf_name;
        fs.open(conf_root);
        if (!fs.is_open())
            throw std::invalid_argument("Conf file does not exist");
    }
}

/*
    check_and_set_flag_location() - key 가 location 일 경우
    iss : buf string stream
    server_falg : 서버 문단이 시작 됬는지 확인하는 플래그
    buf string steam 에 남은 것이 있는지 확인, 남아있고 그것이 '#' 이 아니면 에러
    설명 : 서버 플래그 확인  ok -> 서버 브래킷 확인  ok -> 로케이션 플래그 확인   ok -> error
                        no -> error
                                            no -> error     
                                                                    no -> 로케이션 플래그 on
*/

void Configure::check_and_set_flag_location(std::istringstream& iss, bool& server_flag, bool& location_flag, bool& server_bracket)
{
    std::string sub;
    iss >> std::ws >> sub;
    if (!sub.empty() && sub[0] != '#')
        std::runtime_error("The syntax of the conf file is wrong.");
    if (server_flag)
    {
        if (server_bracket)
        {
            if (location_flag)
                throw std::runtime_error("The syntax of the conf file is wrong.");
            else
                location_flag = true;
        }
        else
            throw std::runtime_error("The syntax of the conf file is wrong.");
    }
    else
        throw std::runtime_error("The syntax of the conf file is wrong.");
}

/*
    check_and_set_flag_server() - key 가 server 일 경우
    server_falg : 서버 문단이 시작 됬는지 확인하는 플래그
    설명 : 서버 플래그 확인  ok -> error
                        no -> server flag on
*/

void Configure::check_and_set_flag_server(std::istringstream& iss, bool& server_flag)
{
    std::string sub;
    iss >> std::ws >> sub;
    if (!sub.empty() && sub[0] != '#')
        std::runtime_error("The syntax of the conf file is wrong.");
    if (server_flag)
        throw std::runtime_error("The syntax of the conf file is wrong.");
    else
        server_flag = true;
}

/*
    check_and_unset_flag_bracket() - key 의 첫 문자가 '}'
    server_falg : 서버 문단이 시작 됬는지 확인하는 플래그
    location_flag : 로케이션 문단이 시작 됬는지 확인하는 플래그
    server_bracket : 서버의 '{' 가 있는지 확인하는 플래그
    location_bracket : 로케이션의 '{' 가 있는지 확인하는 플래그
    설명 : 서버 플래그 확인  ok -> 서버 브래킷 확인  ok -> 로케이션 플래그 확인   ok -> 로케이션 브래킷 확인   ok -> 로케이션 브래킷, 플래그 off
                        no -> error
                                            no -> error     
                                                                    no, 서버 브래킷, 플래그 off
                                                                                            no -> error
*/

void Configure::check_and_unset_flag_bracket(std::istringstream& iss, bool& server_flag, bool& location_flag, bool& server_bracket, bool& location_bracket)
{
    std::string sub;
    iss >> std::ws >> sub;
    if (!sub.empty() && sub[0] != '#')
        std::runtime_error("The syntax of the conf file is wrong.");
    if (server_flag)
    {
        if (server_bracket)
        {
            if (location_flag)
            {
                if (location_bracket)
                {
                    location_bracket = false;
                    location_flag = false;
                }
                else
                    throw std::runtime_error("The syntax of the conf file is wrong.");
            }
            else
            {
                server_bracket = false;
                server_flag = false;
            }
        }
        else
            throw std::runtime_error("The syntax of the conf file is wrong.");
    }
    else
        throw std::runtime_error("The syntax of the conf file is wrong.");
}

/*
    check_and_set_flag_bracket() - key 의 첫 문자가 '{'
    server_falg : 서버 문단이 시작 됬는지 확인하는 플래그
    location_flag : 로케이션 문단이 시작 됬는지 확인하는 플래그
    server_bracket : 서버의 '{' 가 있는지 확인하는 플래그
    location_bracket : 로케이션의 '{' 가 있는지 확인하는 플래그
    설명 : 서버 플래그 확인  ok -> 서버 브래킷 확인  ok -> 로케이션 플래그 확인   ok -> 로케이션 브래킷 확인   ok -> error
                        no -> error
                                            no -> 서버 브래킷 on     
                                                                    no, error
                                                                                            no -> 로케이션 브래킷 on
*/

void Configure::check_and_set_flag_bracket(std::istringstream& iss, bool& server_flag, bool& location_flag, bool& server_bracket, bool& location_bracket)
{
    std::string sub;
    iss >> std::ws >> sub;
    if (!sub.empty() && sub[0] != '#')
        std::runtime_error("The syntax of the conf file is wrong.");
    if (server_flag)
    {
        if (server_bracket)
        {
            if (location_flag)
            {
                if (location_bracket)
                    throw std::runtime_error("The syntax of the conf file is wrong.");
                else
                    location_bracket = true;
            }
            else
                throw std::runtime_error("The syntax of the conf file is wrong.");
        }
        else
            server_bracket = true;
    }
    else
        throw std::runtime_error("The syntax of the conf file is wrong.");
}

void Configure::parsing_listen(std::istringstream& iss)
{

}

/*
    start_parsing() - 파일스트림을 가지고 파일을 읽고 파싱
    fs : 파일스트림
    key : getline 으로 받아온 버프의 첫번째 단어
    설명 : 첫번째 단어를 key로 지정하여 파싱한다. fs >> std::ws 로 앞부분의 화이트 스페이스는 자동으로 제거되고 빈 줄도 제거된다.
*/

void Configure::start_parsing(std::fstream &fs)
{
    std::string buf, key, value;
    bool        server_flag, location_flag;
    bool        server_bracket, location_bracket;

    while (!fs.eof())
    {
        getline(fs >> std::ws, buf);
        std::istringstream iss(buf);
        iss >> key;
        if (key[0] == '#')
            continue;
        else if (key == "{")
            check_and_set_flag_bracket(iss, server_flag, location_flag, server_bracket, location_bracket);
        else if (key == "}")
            check_and_unset_flag_bracket(iss, server_flag, location_flag, server_bracket, location_bracket);
        else if (key == "server")
            check_and_set_flag_server(iss, server_flag);
        else if (key == "location")
            check_and_set_flag_location(iss, server_flag, location_flag, server_bracket);
        else if (key == "listen" && server_flag && server_bracket && !location_flag)
            parsing_listen(iss);
        else if (key == "server_name")
            parsing_server_name();
        else if (key == "root")
            parsing_root();
        else if (key == "index")
            parsing_index();
        else if (key == "allow_methods")
            parsing_allow_methods();
        else if (key == "error_page")
            parsing_error_page();
        else if (key == "client_max_body_size")
            parsing_client_max_body_size();
        else if (key == "auto_index")
            parsing_auto_index();
        else
            std::runtime_error("The syntax of the conf file is wrong.");
    }
}

Configure::Configure(std::string conf_name) : v_server(1)
{
    std::fstream fs;

    if (conf_name.size() < 6 || conf_name.substr(conf_name.size() - 5) != ".conf")
        throw std::invalid_argument("Conf file name is incorrect");
    get_conf_stream(fs, conf_name);
    start_parsing(fs);
    
}

Configure::~Configure() {}
