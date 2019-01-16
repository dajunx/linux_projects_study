//#include <stdio.h>
//#include <stdlib.h>
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

//----------------------- ��������ͷ-------------------------------
void main_menu(); // ���˵�
void parse_user_input_str_cmd(std::string& input_str_cmd); // �����û������ַ���
void exchange_user_cmd(std::string& user_cmd); // ת���û��Զ�������
void exec_cmds(std::vector<std::string>& cmds); // ִ������

//----------------------- ����ʵ�� -------------------------------

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

void exchange_user_cmd(std::string& user_cmd) {
	if (user_cmd.compare("list") == 0) {
		user_cmd = "ls";
	}
	else {
		// ʲôҲ����
	}
}

void parse_user_input_str_cmd(std::string& input_str_cmd, std::vector<const char *>& cmds) {
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
		std::string input_str_cmd; // �û�����������ַ���
		std::vector<const char *> vec_cmds; // ������������

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