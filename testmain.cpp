#include <iostream>
#include <cstdlib>
#include <cstring>
#include <sys/utsname.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <stdio.h>

#define PORT_NR 8069

std::string	g_name;
bool		g_leaks = false;

void	leaksExit(const std::string &err_msg, int code)
{
	std::cerr << "Error: " << err_msg << std::endl;
	perror(NULL);
	if (g_leaks)
	{
		// should only execute on Mac
		std::string s = "leaks " + g_name + " >> tmp.txt && grep \"total leaked bytes\" tmp.txt && rm tmp.txt";
		std::cerr << s << std::endl;
		system(s.c_str());
	}
	exit(code);
}

void	setLeaks(int ac, char **av)
{
	if (!strcmp(av[1], "leaks"))
	{
		utsname name; // NOLINT(cppcoreguidelines-pro-type-member-init)
		uname(&name);
		if (!strcmp(name.sysname, "Darwin"))
			g_leaks = true;
		else
			std::cerr << "leaks only works on MacOS" << std::endl;
	}
	if (ac != 2 || strcmp(av[1], "leaks") != 0)
		leaksExit("wrong nr of arguments", 1);
}

bool	checkend(std::string &hdr)
{
	unsigned long i;

	i = hdr.length() - 1;
	if (hdr[i - 3] == '\r' && hdr[i - 2] == '\n' && hdr[i - 1] == '\r' && hdr[i] == '\n')
		return (true);
	return (false);
}

void	serverStuff()
{
	int					listenfd;
	int					ret;
	struct sockaddr_in	addr = {};

	listenfd = socket(PF_INET, SOCK_STREAM, 0);
	if (listenfd < 0)
		leaksExit("socket error", listenfd);
	bzero(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(PORT_NR);
	// clear port if in use; if program quits now, socket might still be in use
	int n = 1;
	ret = setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &n, sizeof(n));
	if (ret < 0)
		leaksExit("setsockopt error", ret);
	// end of clearing port
	ret = bind(listenfd, (sockaddr*)&addr, sizeof(addr));
	if (ret < 0)
		leaksExit("bind error", ret);
	ret = listen(listenfd, 10);
	if (ret < 0)
		leaksExit("listen error", ret);

	// this loop will read 1 entire header and send a super basic response

	while (true)
	{
		int 	connectfd;
		char	buf[101];
		std::string hdr;
		buf[100] = 0;
		std::cout << "listening on " << PORT_NR << std::endl;
		connectfd = accept(listenfd, (sockaddr*)NULL, NULL);
		if (connectfd < 0)
			leaksExit("accept error", connectfd);
		ret = 1;
		while (ret > 0)
		{
			ret = read(connectfd, buf, 100);
			if (ret != 100)
				buf[ret] = 0;
			hdr += buf;
			if (checkend(hdr))
				break ;
		}
		// here the entire header is stored in the string 'hdr'
		std::cout << hdr << std::endl;

		// this is just most basic response that is easy to see
		std::string bf = "HTTP/1.0 200 OK\r\n\r\nHello";
		write(connectfd, bf.c_str(), bf.length());
		close(connectfd);
	}
	close(listenfd);
}

int		main(int ac, char **av)
{
	g_name = strrchr(av[0], '/') + 1;
	if (ac != 1)
		setLeaks(ac, av);
	std::cout << "sup ik ben een server :)" << std::endl;

	serverStuff();
}
