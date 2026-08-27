/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi_handler.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aramos <contact@aramos.dev>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/28 00:55:10 by aramos            #+#    #+#             */
/*   Updated: 2026/08/28 00:57:40 by aramos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

//	Hardcoded stuff
//   - path to the CGI script
//   - path to the interpreter (python3)
//   - request method (GET)
//   - query string
//   - request body (empty, since GET)
static char** buildEnvArray(const std::vector<std::string>& env)
{
    char** envp = new char*[env.size() + 1];
    for (size_t i = 0; i < env.size(); ++i)
    {
        envp[i] = new char[env[i].size() + 1];
        std::strcpy(envp[i], env[i].c_str());
    }
    envp[env.size()] = NULL;
    return envp;
}

static void freeEnvArray(char** envp)
{
    for (size_t i = 0; envp[i] != NULL; ++i)
        delete[] envp[i];
    delete[] envp;
}

// requestBody   -> bytes to feed to the script's stdin (empty for GET)
// scriptPath    -> hardcoded path to the .py file for now
// interpreter   -> hardcoded path to python3 for now
// queryString   -> hardcoded fake query string for now
std::string runCGI(const std::string& requestBody,
                    const std::string& scriptPath,
                    const std::string& interpreter,
                    const std::string& queryString)
{
    int inPipe[2];  //child reads on stdin
    int outPipe[2]; //parent reads

    if (pipe(inPipe) == -1 || pipe(outPipe) == -1)
    {
        std::cerr << "pipe() failed: " << std::strerror(errno) << std::endl;
        return "";
    }

    pid_t pid = fork();
    if (pid == -1)
    {
        std::cerr << "fork() failed: " << std::strerror(errno) << std::endl;
        return "";
    }

    if (pid == 0)
    {
        //CHILD PROCESS
        dup2(inPipe[0], STDIN_FILENO);
        dup2(outPipe[1], STDOUT_FILENO);

        close(inPipe[0]);
        close(inPipe[1]);
        close(outPipe[0]);
        close(outPipe[1]);

        std::vector<std::string> env;
        env.push_back("REQUEST_METHOD=GET");
        env.push_back("SCRIPT_NAME=" + scriptPath);
        env.push_back("QUERY_STRING=" + queryString);
        {
            std::ostringstream lenStream;
            lenStream << requestBody.size();
            env.push_back("CONTENT_LENGTH=" + lenStream.str());
        }
        env.push_back("CONTENT_TYPE=text/plain");
        env.push_back("SERVER_PROTOCOL=HTTP/1.0");
        env.push_back("GATEWAY_INTERFACE=CGI/1.1");
        env.push_back("SERVER_SOFTWARE=webserv/0.1");
        env.push_back("REDIRECT_STATUS=200"); // needed by some php-cgi builds

        char** envp = buildEnvArray(env);

        char* argv[3];
        argv[0] = const_cast<char*>(interpreter.c_str());
        argv[1] = const_cast<char*>(scriptPath.c_str());
        argv[2] = NULL;

        execve(interpreter.c_str(), argv, envp);

        std::cerr << "execve failed: " << std::strerror(errno) << std::endl;
        freeEnvArray(envp);
        _exit(1);
    }

    //PARENT PROCESS
    close(inPipe[0]);
	close(outPipe[1]);

    if (!requestBody.empty())
        write(inPipe[1], requestBody.c_str(), requestBody.size());
    close(inPipe[1]);
    std::string output;
    char buf[4096];
    ssize_t n;
    while ((n = read(outPipe[0], buf, sizeof(buf))) > 0)
        output.append(buf, n);
    close(outPipe[0]);
    int status;
    waitpid(pid, &status, 0);

    return output;
}

int main()
{
    std::string scriptPath  = "./cgi-bin/test.py";
    std::string interpreter = "/usr/bin/python3";
    std::string queryString = "name=webserv";
    std::string requestBody = "";

    std::string result = runCGI(requestBody, scriptPath, interpreter, queryString);

    if (result.empty())
    {
        std::cerr << "Got nothing back from CGI. Did the script run? "
                     "Check path/permissions/interpreter." << std::endl;
        return 1;
    }

    std::cout << "----- RAW CGI OUTPUT -----" << std::endl;
    std::cout << result << std::endl;
    std::cout << "---------------------------" << std::endl;
    std::cout << "(I hope this helps somehow)" << std::endl;
    return 0;
}
