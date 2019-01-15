#include<unistd.h>
#include<iostream>
#include<cstring>
#include<sys/types.h>
#include<sys/stat.h>
#include<dirent.h>
#include<stdlib.h>
#include<fcntl.h>
#include<queue>
#include<ftw.h>
#include<stdio.h>
#include<signal.h>
#include<string>
#include<vector>
using namespace std;

//----------------------- 函数定义头-------------------------------
void main_menu(); // 主菜单
void parse_user_input_str_cmd(std::string& input_str_cmd); // 解析用户输入字符串
void exec_cmds(std::vector<std::string>& cmds); // 执行命令

//----------------------- 函数实现 -------------------------------

void main_menu() {
	cout << "*******************************************" << endl;
	cout << "Please select the function:" << endl;
	cout << "- pwd " << endl;
	cout << "- dir <dirname>" << endl;
	cout << "- cd <dirname or path> " << endl;
	cout << "- newdir <dirname> " << endl;
	cout << "- deldir <dirname> " << endl;
	cout << "- help " << endl;
	cout << "- exit " << endl;
	cout << "*******************************************" << endl;
}

void parse_user_input_str_cmd(std::string& input_str_cmd, std::vector<const char *>& cmds) {
	std::size_t pos = input_str_cmd.find(" ");
	std::string str_cmds;
	while (pos != std::string::npos) {
		str_cmds = input_str_cmd.substr(0, pos);
		cmds.push_back(str_cmds.c_str());
		input_str_cmd.erase(0, pos + 1);
		pos = input_str_cmd.find(" ");
	}

	cmds.push_back(input_str_cmd.c_str());
}

void exec_cmds(std::vector<const char *>& cmds) {
	cmds.push_back(NULL);
	char **command = const_cast<char **>(&cmds[0]);
	pid_t pid = fork();

	if (pid < 0) {
		printf("fork error!");
	}
	else if (pid == 0) {
		if (execvp(command[0], command) < 0) {
			printf("exec error!");
			exit(1);
		}
	}
	else {
		//wait(NULL);
	}
}

int main(int argc, char *argv[])
{
	main_menu();

	while (true) {
		std::string input_str_cmd; // 用户输入的命令字符串
		std::vector<const char *> vec_cmds; // 命令及其参数集合

		std::cout << "[wuwenjie@]$";
		fflush(stdout);
		std::getline(std::cin, input_str_cmd);
		if (input_str_cmd.compare("exit") == 0) {
			break;
		}
		else if(input_str_cmd.empty()) {
			continue;
		}

		parse_user_input_str_cmd(input_str_cmd, vec_cmds);
		exec_cmds(vec_cmds);
	}

	return 0;
}