#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cstring>
//#include <dirent.h>
//#include <fcntl.h>
//#include <ftw.h>
//#include <signal.h>
#include <string>
#include <vector>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

using namespace std;

//----------------------- 函数定义头-------------------------------
void main_menu(); // 主菜单
void parse_user_input_str_cmd(std::string& input_str_cmd); // 解析用户输入字符串
void exchange_user_cmd(std::string& user_cmd); // 转换用户自定义命令
void exec_cmds(std::vector<std::string>& cmds); // 执行命令
void handle_cd_cmd(std::string& user_cd_cmd); // [特殊]执行cd命令

//----------------------- 函数实现 -------------------------------

void main_menu() {
	cout << "*******************************************" << endl;
	cout << "Please select the function:" << endl;
	cout << "- pwd " << endl;
	cout << "- list <dirname>" << endl;
	cout << "- cd <dirname or path> " << endl;
	cout << "- mkdir <dirname> " << endl;
	cout << "- rmdir <dirname> " << endl;
	cout << "- help " << endl;
	cout << "- exit " << endl;
	cout << "*******************************************" << endl;
}

void exchange_user_cmd(std::string& user_cmd) {
	if (user_cmd.compare("list") == 0) {
		user_cmd = "ls";
	} else {
		// 什么也不做
	}
}

void handle_cd_cmd(std::string& user_cd_cmd) {
	std::size_t pos = user_cd_cmd.find(" ");
	std::string str_cmds;
	if (pos == std::string::npos) {
		// 执行cd 命令，不做任何处理
		return;
	}

	str_cmds = user_cd_cmd.substr(0, pos);
	user_cd_cmd.erase(0, pos + 1);

	char current_path[50];
	getcwd(current_path, sizeof(current_path)); //获取当前路径
	std::string str_current_path(current_path);
	if (user_cd_cmd.compare("..") == 0) {
		// 返回上级目录
		pos = str_current_path.find_last_of("\/");
		str_current_path = str_current_path.substr(0, pos);
		//strcpy(current_path, str_current_path.c_str());
		chdir(str_current_path.c_str());
	} else if (user_cd_cmd.compare(".") == 0) {
		// 执行cd .命令，不做任何处理
		return;
	}
}

void parse_user_input_str_cmd(std::string& input_str_cmd, std::vector<const char *>& cmds) {
	// cd 特殊处理
	if (input_str_cmd.find("cd") != std::string::npos) {
		handle_cd_cmd(input_str_cmd);
		return;
	}

	std::size_t pos = input_str_cmd.find(" ");
	std::string str_cmds;
	while (pos != std::string::npos) {
		str_cmds = input_str_cmd.substr(0, pos);
		exchange_user_cmd(str_cmds);
		cmds.push_back(str_cmds.c_str());
		input_str_cmd.erase(0, pos + 1);
		pos = input_str_cmd.find(" ");
	}

	exchange_user_cmd(input_str_cmd);
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
		int ret = 0;
		if (ret = execvp(command[0], command) < 0) {
			printf("exec error, ret:%d", ret);
			exit(1);
		}
	}
	else {
		//wait(NULL);
		sleep(1);
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