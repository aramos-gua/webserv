#!/usr/bin/env python3
import os

# A CGI script MUST print headers first, then a blank line, then the body.
print("Content-Type: text/plain")
print()  # blank line = end of headers
print("Hello from CGI!")
print("REQUEST_METHOD =", os.environ.get("REQUEST_METHOD"))
print("QUERY_STRING   =", os.environ.get("QUERY_STRING"))
print("SCRIPT_NAME    =", os.environ.get("SCRIPT_NAME"))
