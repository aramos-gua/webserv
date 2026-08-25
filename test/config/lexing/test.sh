#!/usr/bin/env bash

export GREEN="\033[32m"
export RED="\033[31m"
export DEFAULT="\033[0m"

run_error_test() {
	if [ "$CONFIG_FILE" ]; then
		CONFIG_FILE="../../../test/config/lexing/config-files/$CONFIG_FILE"
	fi
	export ACTUAL="$(../../../bin/config-lexing $CONFIG_FILE 2>&1)"
	cmp -s <(echo "$EXPECTED") <(echo "$ACTUAL") \
		&& (echo -en "[${GREEN}PASS${DEFAULT}] " && echo "$TEST_NAME") \
		|| (echo -en "[${RED}FAIL${DEFAULT}] " && echo "$TEST_NAME" \
			&& echo "$ACTUAL")
}

echo "*** ERROR TESTS ***"
echo -n "Run test suite? [Y/n] "
read -n 1 ANSWER
echo

if [ "$ANSWER" != "n" ]; then
	export TEST_NAME="Missing config file"
	export CONFIG_FILE=""
	export EXPECTED="\
Usage: ../../../bin/config-lexing <path to config file>"
	run_error_test

	export TEST_NAME="Bad config file: nonexistent"
	export CONFIG_FILE="nonexistent.conf"
	export EXPECTED="\
An error occurred during execution:
Couldn't open config file \"../../../test/config/lexing/config-files/nonexistent.conf\" for reading.
Unable to continue."
	run_error_test

	export TEST_NAME="Bad config file: escape character followed by EOF"
	export CONFIG_FILE="nearly-empty.conf"
	export EXPECTED="\
- TerminatingConfigToken
An error occurred during execution:
Unexpected end of file after escape character at line 1, character 1 of config file.
Unable to continue."
	run_error_test

	export TEST_NAME="Bad config file: unterminated quote"
	export CONFIG_FILE="no-servers.conf"
	export EXPECTED="\
- WordConfigToken             property_0
- WordConfigToken             Hi
- SpecialCharacterConfigToken ;
- WordConfigToken             property_1
- WordConfigToken             Hello\ there
- SpecialCharacterConfigToken ;
- WordConfigToken             property_2
- WordConfigToken             Nice
- WordConfigToken             to
- WordConfigToken             meet
- WordConfigToken             you
- SpecialCharacterConfigToken ;
- WordConfigToken             property_3
- WordConfigToken             'How are you today'
- SpecialCharacterConfigToken ;
- WordConfigToken             property_4
- WordConfigToken             \"I am well thank you\"
- SpecialCharacterConfigToken ;
- WordConfigToken             property_5
- WordConfigToken             \"I think; therefore I am\"
- SpecialCharacterConfigToken ;
- WordConfigToken             property_6
- WordConfigToken             \"This should be spread across
			two lines\"
- SpecialCharacterConfigToken ;
- WordConfigToken             property_7
- WordConfigToken             \"This should end up on 			one line\"
- SpecialCharacterConfigToken ;
- WordConfigToken             property_8
- WordConfigToken             \"I know what I'm doing\"
- SpecialCharacterConfigToken ;
- WordConfigToken             property_9
- WordConfigToken             \"I have a feeling I'm about to be interrupted-
- TerminatingConfigToken
An error occurred during execution:
Opening quote at line 15, character 12 of config file was not closed.
Unable to continue."
	run_error_test
fi

run_success_test() {
	CONFIG_FILE="../../../test/config/lexing/config-files/$CONFIG_FILE"
	export ACTUAL="$(../../../bin/config-lexing $CONFIG_FILE 2>&1)"
	cmp -s <(echo "$EXPECTED") <(echo "$ACTUAL") \
		&& (echo -en "[${GREEN}PASS${DEFAULT}] " && echo "$TEST_NAME") \
		|| (echo -en "[${RED}FAIL${DEFAULT}] " && echo "$TEST_NAME" \
			&& echo "$ACTUAL")
}

echo "*** SUCCESS TESTS ***"
echo -n "Run test suite? [Y/n] "
read -n 1 ANSWER
echo

if [ "$ANSWER" != "n" ]; then
	export TEST_NAME="Config file with one server"
	export CONFIG_FILE="one-server.conf"
	export EXPECTED="\
- WordConfigToken             user
- WordConfigToken             www-data
- SpecialCharacterConfigToken ;
- WordConfigToken             server
- SpecialCharacterConfigToken {
- WordConfigToken             server_name
- WordConfigToken             www.server.com
- WordConfigToken             server.com
- SpecialCharacterConfigToken ;
- WordConfigToken             listen
- WordConfigToken             80
- SpecialCharacterConfigToken ;
- WordConfigToken             root
- WordConfigToken             /var/www/html/
- SpecialCharacterConfigToken ;
- WordConfigToken             location
- WordConfigToken             /
- SpecialCharacterConfigToken {
- WordConfigToken             try_files
- WordConfigToken             \$uri
- WordConfigToken             =404
- SpecialCharacterConfigToken ;
- SpecialCharacterConfigToken }
- SpecialCharacterConfigToken }
- TerminatingConfigToken"
	run_success_test

	export TEST_NAME="Config file with two servers"
	export CONFIG_FILE="two-servers.conf"
	export EXPECTED="\
- WordConfigToken             server
- SpecialCharacterConfigToken {
- WordConfigToken             server_name
- WordConfigToken             www.server.com
- WordConfigToken             server.com
- SpecialCharacterConfigToken ;
- WordConfigToken             listen
- WordConfigToken             80
- SpecialCharacterConfigToken ;
- WordConfigToken             root
- WordConfigToken             /var/www/html/
- SpecialCharacterConfigToken ;
- WordConfigToken             location
- WordConfigToken             /
- SpecialCharacterConfigToken {
- WordConfigToken             try_files
- WordConfigToken             \$uri
- WordConfigToken             =404
- SpecialCharacterConfigToken ;
- SpecialCharacterConfigToken }
- SpecialCharacterConfigToken }
- WordConfigToken             server
- SpecialCharacterConfigToken {
- WordConfigToken             server_name
- WordConfigToken             blog.server.com
- SpecialCharacterConfigToken ;
- WordConfigToken             listen
- WordConfigToken             8080
- SpecialCharacterConfigToken ;
- WordConfigToken             root
- WordConfigToken             /opt/blog
- SpecialCharacterConfigToken ;
- WordConfigToken             location
- WordConfigToken             /
- SpecialCharacterConfigToken {
- WordConfigToken             try_files
- WordConfigToken             \$uri
- WordConfigToken             =404
- SpecialCharacterConfigToken ;
- SpecialCharacterConfigToken }
- SpecialCharacterConfigToken }
- TerminatingConfigToken"
	run_success_test

	export TEST_NAME="Extended config file"
	export CONFIG_FILE="wordpress.conf"
	export EXPECTED="\
- WordConfigToken             server
- SpecialCharacterConfigToken {
- WordConfigToken             server_name
- WordConfigToken             localhost
- WordConfigToken             127.0.0.1
- SpecialCharacterConfigToken ;
- WordConfigToken             listen
- WordConfigToken             80
- SpecialCharacterConfigToken ;
- WordConfigToken             access_log
- WordConfigToken             /dev/stdout
- SpecialCharacterConfigToken ;
- WordConfigToken             error_log
- WordConfigToken             /dev/stderr
- SpecialCharacterConfigToken ;
- WordConfigToken             root
- WordConfigToken             /var/www/html/
- SpecialCharacterConfigToken ;
- WordConfigToken             index
- WordConfigToken             index.php
- WordConfigToken             index.html
- SpecialCharacterConfigToken ;
- WordConfigToken             absolute_redirect
- WordConfigToken             off
- SpecialCharacterConfigToken ;
- WordConfigToken             location
- WordConfigToken             /
- SpecialCharacterConfigToken {
- WordConfigToken             try_files
- WordConfigToken             \$uri
- WordConfigToken             \$uri/
- WordConfigToken             =404
- SpecialCharacterConfigToken ;
- SpecialCharacterConfigToken }
- WordConfigToken             location
- WordConfigToken             =
- WordConfigToken             /wordpress
- SpecialCharacterConfigToken {
- WordConfigToken             return
- WordConfigToken             302
- WordConfigToken             /wordpress/\$is_args\$args
- SpecialCharacterConfigToken ;
- SpecialCharacterConfigToken }
- WordConfigToken             location
- WordConfigToken             /wordpress/
- SpecialCharacterConfigToken {
- WordConfigToken             alias
- WordConfigToken             /var/www/localhost/wordpress/
- SpecialCharacterConfigToken ;
- WordConfigToken             add_header
- WordConfigToken             Access-Control-Allow-Origin
- WordConfigToken             \"*\"
- SpecialCharacterConfigToken ;
- WordConfigToken             try_files
- WordConfigToken             \$uri
- WordConfigToken             \$uri/
- WordConfigToken             /wordpress/index.php\$is_args\$args
- WordConfigToken             /wordpress/?error=404
- SpecialCharacterConfigToken ;
- SpecialCharacterConfigToken }
- WordConfigToken             location
- WordConfigToken             =
- WordConfigToken             /wordpress/wp-admin
- SpecialCharacterConfigToken {
- WordConfigToken             return
- WordConfigToken             302
- WordConfigToken             /wordpress/wp-admin/\$is_args\$args
- SpecialCharacterConfigToken ;
- SpecialCharacterConfigToken }
- WordConfigToken             location
- WordConfigToken             ~
- WordConfigToken             ^/wordpress/((?:index|wp-login|wp-cron|wp-comments-post|wp-admin/(?!(?:install|install-helper|setup-config)\.php)[^/]+)\.php)(/.*)?$
- SpecialCharacterConfigToken {
- WordConfigToken             try_files
- WordConfigToken             /wordpress/\$1
- WordConfigToken             /wordpress/?error=404
- SpecialCharacterConfigToken ;
- WordConfigToken             include
- WordConfigToken             fastcgi_params
- SpecialCharacterConfigToken ;
- WordConfigToken             fastcgi_param
- WordConfigToken             SCRIPT_FILENAME
- WordConfigToken             \$document_root/wordpress/\$1
- SpecialCharacterConfigToken ;
- WordConfigToken             fastcgi_param
- WordConfigToken             SCRIPT_NAME
- WordConfigToken             /wordpress/\$1
- SpecialCharacterConfigToken ;
- WordConfigToken             fastcgi_param
- WordConfigToken             PATH_INFO
- WordConfigToken             \$2
- SpecialCharacterConfigToken ;
- WordConfigToken             fastcgi_pass
- WordConfigToken             wordpress:9000
- SpecialCharacterConfigToken ;
- SpecialCharacterConfigToken }
- WordConfigToken             location
- WordConfigToken             ~
- WordConfigToken             ^/wordpress/(.+\.php)(/.*)?$
- SpecialCharacterConfigToken {
- WordConfigToken             rewrite
- WordConfigToken             ^(.*)$
- WordConfigToken             /wordpress/?error=404?
- WordConfigToken             last
- SpecialCharacterConfigToken ;
- SpecialCharacterConfigToken }
- SpecialCharacterConfigToken }
- TerminatingConfigToken"
	run_success_test
fi
