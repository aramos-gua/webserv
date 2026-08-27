#!/usr/bin/env bash

export GREEN="\033[32m"
export RED="\033[31m"
export DEFAULT="\033[0m"

report_pass() {
	echo -en "[${GREEN}PASS${DEFAULT}] " && echo "$TEST_NAME"
}

report_fail() {
	echo -en "[${RED}FAIL${DEFAULT}] " && echo "$TEST_NAME"
}

run_usage_test() {
	export ACTUAL="$($BINARY $OPTIONS 2>&1)"
	if cmp -s <(echo "$EXPECTED") <(echo "$ACTUAL"); then
		report_pass
	else
		report_fail && echo "$ACTUAL"
	fi
	export OPTIONS=""
}
