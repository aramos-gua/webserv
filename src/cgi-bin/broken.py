#!/usr/bin/env python3
# cgi-bin/broken.py — deliberately crashes before producing valid CGI output.
# Wired to the "Trigger a 500" button on the homepage, to show live at eval
# that a broken CGI script doesn't crash or hang the server itself — it
# should just come back as a clean HTTP 500 while the rest of the site
# keeps working normally.
raise RuntimeError("Simulated CGI failure for eval demo")
