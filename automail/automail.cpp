// mailing-list-cpp
// https://github.com/minajune/mailing-list-cpp
//

#include "pch.h"
#include <iostream>

std::vector<std::string> mail_list;

int main()
{
	std::string mail_to_send = "a3.msg";
	std::string script_name = "script.ps1";
	std::ifstream in("input.txt");
	if (!in.good())
		return 0;
	std::cout << "\rParsing input file...";
	while (!in.eof())
	{
		std::string cur;
		if (in >> cur)
			if (cur.find('@') != std::string::npos)
			{
				mail_list.push_back(cur);
				std::cout << "\rParsing input file... (" << std::to_string(mail_list.size()) << " address read)";
			}
	}

	std::cout << std::endl;

	for (int i = 0; i < mail_list.size(); i++)
	{
		std::cout << "Sending mail to: " << std::quoted(mail_list.at(i)) << std::endl;

		std::ofstream script(script_name);
		script << "$outlook = New-Object -comObject Outlook.Application" << std::endl
			   << "$mail = $outlook.Session.OpenSharedItem(" << std::quoted(mail_to_send) << ")" << std::endl
			   << "$mail.Forward()" << std::endl
			   << "$mail.Recipients.Add(" << std::quoted(mail_list.at(i)) << ")" << std::endl
			   << "$mail.Send()" << std::endl
			   << "[System.Runtime.InteropServices.Marshal]::ReleaseComObject($mail) | Out-Null" << std::endl;
		script.close();
		std::stringstream f;
		f << "-ExecutionPolicy Bypass -File " << std::quoted(script_name);
		std::string params = f.str(),
		system(std::string("powershell " + params).c_str());
	}
	std::cout << "Program finished." << std::endl;
	std::getchar();
	return 0;
}

