#include "StudentManager.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <iomanip>
#include <filesystem>
#include <cctype>
#include <algorithm>
#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
using socket_t = SOCKET;
#define CLOSESOCKET closesocket
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
using socket_t = int;
#define CLOSESOCKET close
#endif

static std::string decode(const std::string& s){std::string o; for(size_t i=0;i<s.size();++i){if(s[i]=='+')o+=' ';else if(s[i]=='%'&&i+2<s.size()){try{o+=(char)std::stoi(s.substr(i+1,2),nullptr,16);i+=2;}catch(...){o+=s[i];}}else o+=s[i];}return o;}
static std::map<std::string,std::string> parseForm(const std::string& s){std::map<std::string,std::string> m;std::stringstream ss(s);std::string p;while(std::getline(ss,p,'&')){auto n=p.find('=');m[decode(p.substr(0,n))]=n==std::string::npos?"":decode(p.substr(n+1));}return m;}
static std::string esc(const std::string& s){std::string o;for(unsigned char c:s){switch(c){case '"':o+="\\\"";break;case '\\':o+="\\\\";break;case '\n':o+="\\n";break;case '\r':o+="\\r";break;case '\t':o+="\\t";break;default:if(c<32){ }else o+=(char)c;}}return o;}
static std::string jsonStudent(const Student&s){std::ostringstream o;o<<"{\"id\":\""<<esc(s.id)<<"\",\"name\":\""<<esc(s.name)<<"\",\"email\":\""<<esc(s.email)<<"\",\"phone\":\""<<esc(s.phone)<<"\",\"department\":\""<<esc(s.department)<<"\",\"semester\":"<<s.semester<<",\"cgpa\":"<<s.cgpa<<",\"attendance\":"<<s.attendance<<",\"status\":\""<<esc(s.status)<<"\"}";return o.str();}
static std::string jsonList(const std::vector<Student>& v){std::string o="[";for(size_t i=0;i<v.size();++i){if(i)o+=",";o+=jsonStudent(v[i]);}return o+"]";}
static std::string response(const std::string& body,const std::string& type="application/json; charset=utf-8",int code=200){std::string label=code==200?"OK":code==201?"Created":code==400?"Bad Request":code==404?"Not Found":"Internal Server Error";std::ostringstream o;o<<"HTTP/1.1 "<<code<<" "<<label<<"\r\nContent-Type: "<<type<<"\r\nAccess-Control-Allow-Origin: *\r\nAccess-Control-Allow-Methods: GET, POST, PUT, DELETE, OPTIONS\r\nAccess-Control-Allow-Headers: Content-Type\r\nContent-Length: "<<body.size()<<"\r\nConnection: close\r\n\r\n"<<body;return o.str();}
static std::string readFile(const std::string&p){std::ifstream f(p,std::ios::binary);if(!f)return "";std::ostringstream s;s<<f.rdbuf();return s.str();}
static std::string mime(const std::string&p){if(p.size()>=5&&p.substr(p.size()-5)==".html")return "text/html; charset=utf-8";if(p.size()>=4&&p.substr(p.size()-4)==".css")return "text/css; charset=utf-8";if(p.size()>=3&&p.substr(p.size()-3)==".js")return "application/javascript; charset=utf-8";return "application/octet-stream";}
int main(){
#ifdef _WIN32
 WSADATA w; if(WSAStartup(MAKEWORD(2,2),&w)!=0){std::cerr<<"Winsock startup failed\n";return 1;}
#endif
 StudentManager db("data/students.db");std::string err;if(!db.load(err)){std::cerr<<err<<"\n";return 1;}
 socket_t server=socket(AF_INET,SOCK_STREAM,0);if(server<0){std::cerr<<"Could not create server socket\n";return 1;}
 int opt=1;setsockopt(server,SOL_SOCKET,SO_REUSEADDR,(const char*)&opt,sizeof(opt));sockaddr_in addr{};addr.sin_family=AF_INET;addr.sin_addr.s_addr=htonl(INADDR_ANY);addr.sin_port=htons(8080);
 if(bind(server,(sockaddr*)&addr,sizeof(addr))<0||listen(server,10)<0){std::cerr<<"Could not listen on port 8080\n";return 1;}
 std::cout<<"CampusCore Web is running: http://localhost:8080\nPress Ctrl+C to stop.\n";
 while(true){sockaddr_in client{};socklen_t len=sizeof(client);socket_t s=accept(server,(sockaddr*)&client,&len);if(s<0)continue;std::string req;char buf[8192];int n=0;while((n=recv(s,buf,sizeof(buf),0))>0){req.append(buf,n);auto h=req.find("\r\n\r\n");if(h!=std::string::npos){size_t cl=0;auto cp=req.find("Content-Length:");if(cp!=std::string::npos){auto e=req.find("\r\n",cp);cl=(size_t)std::stoul(req.substr(cp+15,e-cp-15));}if(req.size()>=h+4+cl)break;}}
 auto lineEnd=req.find("\r\n");std::istringstream first(req.substr(0,lineEnd));std::string method,path,ver;first>>method>>path>>ver;auto h=req.find("\r\n\r\n");std::string body=h==std::string::npos?"":req.substr(h+4);std::string out;
 if(method=="OPTIONS")out=response("{}");
 else if(path=="/api/students"&&method=="GET"){auto q=path.find('?');(void)q;out=response(jsonList(db.all()));}
 else if(path=="/api/stats"&&method=="GET"){std::ostringstream j;j<<std::fixed<<std::setprecision(2)<<"{\"total\":"<<db.total()<<",\"active\":"<<db.activeCount()<<",\"averageCgpa\":"<<db.averageCgpa()<<",\"averageAttendance\":"<<db.averageAttendance()<<"}";out=response(j.str());}
 else if(path=="/api/students"&&(method=="POST"||method=="PUT")){auto f=parseForm(body);Student st;st.id=method=="POST"?db.generateId():f["id"];st.name=f["name"];st.email=f["email"];st.phone=f["phone"];st.department=f["department"];st.status=f["status"].empty()?"Active":f["status"];try{st.semester=std::stoi(f["semester"]);st.cgpa=std::stod(f["cgpa"]);st.attendance=std::stod(f["attendance"]);}catch(...){out=response("{\"error\":\"Please enter valid numeric values.\"}","application/json",400);goto send;}
 if(st.name.empty()||st.email.empty()||st.department.empty()||st.semester<1||st.semester>12||st.cgpa<0||st.cgpa>4||st.attendance<0||st.attendance>100){out=response("{\"error\":\"Please check required fields and value ranges.\"}","application/json",400);}
 else {bool ok=method=="POST"?db.add(st,err):db.update(st.id,st,err);out=ok?response(jsonStudent(st),"application/json",method=="POST"?201:200):response("{\"error\":\""+esc(err)+"\"}","application/json",400);}}
 else if(path.rfind("/api/students/",0)==0&&method=="DELETE"){std::string id=decode(path.substr(13));bool ok=db.remove(id,err);out=ok?response("{\"success\":true}"):response("{\"error\":\""+esc(err)+"\"}","application/json",404);}
 else {std::string file=path=="/"?"public/index.html":"public"+path;if(file.find("..")!=std::string::npos)file="";std::string content=readFile(file);if(content.empty())out=response("Not found", "text/plain",404);else out=response(content,mime(file));}
 send: send(s,out.c_str(),(int)out.size(),0);CLOSESOCKET(s);
 }
 CLOSESOCKET(server);
#ifdef _WIN32
 WSACleanup();
#endif
 return 0;
}
