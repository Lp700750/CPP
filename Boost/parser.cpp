#include<iostream>
#include<string>
#include<vector>
#include<boost/filesystem.hpp>
#include"until.hpp"
const std::string original="data/input/";
const std::string result="data/raw_html/raw_txt";
typedef struct DocInfo
{
	std::string title;
	std::string content;
	std::string url;
}docinfo_t;
//第一步：将带有html的文件保存在文件夹里面
bool TransFunc1(const std::string& original,std::vector<std::string>* file);

//第二步：将文件夹里面的文件进行解析
bool AnanFunc(std::vector<std::string>* file,std::vector<docinfo_t>* docinfo);

//第三步：将解析好的文件保存在输出文件夹里面
//bool TransFunc2(std::vector<docinfo_t>* docinfo,const std::string& result);
int main()
{
	std::vector<std::string> file;
	if(!TransFunc1(original,&file))
	{
		std::cerr<<"TransFunc1 error!"<<std::endl;
		return 1;
	}
	std::vector<docinfo_t> docinfo;
	if(!AnanFunc(&file,&docinfo))
	{
		std::cerr<<"AnanFunc error!"<<std::endl;
		return 2;
	}
	
	/*if(!TransFunc2(&docinfo,result))
	{
		std::cerr<<"TransFunc2 error"<<std::endl;	
		return 3;
	}*/

	return 0;
}
bool TransFunc1(const std::string& original,std::vector<std::string>* file)
{
	namespace fs=boost::filesystem;
	fs::path original_path(original);
	if(!fs::exists(original))
	{
		std::cerr<<"path not exits!"<<std::endl;
		return false;
	}
	fs::recursive_directory_iterator finish;
	for(fs::recursive_directory_iterator iterator(original_path);iterator!=finish;iterator++)
	{
		if(!fs::is_regular_file(*iterator))
		{
			continue;
		}
		if(iterator->path().extension()!=".html")
		{
			continue;
		}
		std::cout<<iterator->path().string()<<std::endl;
		file->push_back(iterator->path().string());
	i}
	return true;
}
bool AnanFunc(std::vector<std::string>* File,std::vector<docinfo_t>* docinfo)
{
	for(const std::string &file:File)
	{
		const std::string result;
	//1.读取指定文件夹里面的文件
		if(!until::FileTool::ReadFile(file,&result))
		{
			continue;
		}
	//2.将读取的文件解析title
		DocInfo docInfo;
		if(!parserTitle(result,&docInfo.title))
		{
			continue;
		}
	//3.将读取的文件解析content
		if(!parserContent(result,&docInfo.content))
		{
			continue;
		}
	//4.将读取的文件解析url
		if(!parserUrl(result,&docInfo.url))
		{
			continue;
		}
		docDug(docInfo);
		break;
	}
}
bool parserTitle(const std::string& result,const std::string* title)
{
	std::size_t titleStart=result.find("<title>");
	if(titleStart==std::string::npos)
	{
		return false;
	}
	std::size titleFinish=result.find("</title>");
	if(titleFinish==std::string::npos)
	{
		return false;
	}
	titleStart+=std::string("<title>").size();
	if(titleStart>titleFinish)
	{
		return false;
	}
	*title+=result.substr(titleStart,titleFinish);
	return true;
}
bool parserContent(const std::string& file,std::string* content)
{
	//设置一个状态机，将文件里面的内容分成标签与非标签
	enum status
	{
		LABLE,
		CONTENT
	}
	enum status s=LABLE;
	//遍历网页文件里面的每一个字符
	for(char c:file)
	{
		switch(s)
		{
			case LABLE:
				if(c=='>')
					s=CONTENT;
				break;
			case CONTENT:
				if(c=='<')
					s=LABLE;
				else
				{
					if(c=='\n')
						c=' ';
					content.push_back(c);
				}
				break;
			default:
				break;
		}
	}
	return true;
}
bool parserUrl(const std::string& file,std::string* url)
{
	const std::string majorUrl="https://www.boost.org/doc/libs/1_78_0/doc/html";
	const std::string minorUrl=file.substr(original.size());
	*url=majorUrl+minorUrl;
	return true;
}
void docDug(docinfo_t doc)
{
	std::cout<<"title:"<<doc.title<<std::endl;
	std::cout<<"content:"<<doc.content<<std::endl;
	std::cout<<"url:"<<doc.url<<std::endl;
}



