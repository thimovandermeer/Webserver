# Webserv
Created a webserver which is able to handle http requests. Written in C++ from scratch and yes we stole some ideas from nginx.

## How it works
First you have to run make to create the webserver executable. After that you can run the runserver script using bash. 
As soon as the Server is up and running you can start the testing script. For the testing we have also written a script which is called runtest.sh. If you run this script while the server is up the server will be tested for more or less 10 minutes doing every possible request you can imagine. Using the config file you can setup different ports and locations. I recommend using postman to test individual get requests on our server :)

https://user-images.githubusercontent.com/57872522/125160010-066d8580-e17b-11eb-9833-c775a3dd9694.mov

If you run the two scripts the webserv will be compiled and the test program will be set as well. After that the tester will be busy for 10 minutes more or less testing every possible flaw in the server. You will notice when the tester is done :)

## Resources

[Server setup](https://medium.com/from-the-scratch/http-server-what-do-you-need-to-know-to-build-a-simple-http-server-from-scratch-d1ef8945e4fa)

[HTTP Standard](https://www.rfc-editor.org/rfc/rfc2616.html)

[Select](https://www.lowtek.com/sockets/select.html)

[Non Blocking](https://github.com/cclaude42/webserv)

[CGI getting started](http://www.mnuwer.dbasedeveloper.co.uk/dlearn/web/session01.htm)

[CGI Documentation](http://www.wijata.com/cgi/cgispec.html#4.0)
